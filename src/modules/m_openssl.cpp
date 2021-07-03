/*
 * BerylDB - A lightweight database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 - Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

/// $CompilerFlags: locate_compiler_flags("openssl" "")
/// $LinkerFlags: find_linker_flags("openssl" "-lssl -lcrypto")

/// $PackageInfo: system_depends("darwin") openssl pkg-config
/// $PackageInfo: system_depends("arch") openssl pkgconf
/// $PackageInfo: system_depends("centos") openssl-devel pkgconfig
/// $PackageInfo: system_depends("debian") libssl-dev openssl pkg-config
/// $PackageInfo: system_depends("ubuntu") libssl-dev openssl pkg-config


#include "beryl.h"
#include "queues.h"
#include "modules/ssl.h"

#ifdef __GNUC__
# pragma GCC diagnostic push
#endif


#if defined __GNUC__ && defined LIBRESSL_VERSION_NUMBER
# warning LibreSSL support will be discontinued in the future. Consider using the ssl_gnutls or ssl_mbedtls modules instead.
#endif

#if defined __clang__
# pragma clang diagnostic ignored "-Wc++11-long-long"
#elif defined __GNUC__
# pragma GCC diagnostic ignored "-Wlong-long"
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/dh.h>

#ifdef __GNUC__
# pragma GCC diagnostic pop
#endif

#ifdef _WIN32
# pragma comment(lib, "ssleay32.lib")
# pragma comment(lib, "libeay32.lib")
#endif

#if ((defined LIBRESSL_VERSION_NUMBER) || (OPENSSL_VERSION_NUMBER < 0x10100000L))

# define BIO_get_data(BIO) BIO->ptr
# define BIO_set_data(BIO, VALUE) BIO->ptr = VALUE;
# define BIO_set_init(BIO, VALUE) BIO->init = VALUE;

# define OpenSSL_version SSLeay_version
# define X509_getm_notAfter X509_get_notAfter
# define X509_getm_notBefore X509_get_notBefore
# define OPENSSL_init_ssl(OPTIONS, SETTINGS) \
	SSL_library_init(); \
	SSL_load_error_strings();

# define OPENSSL_VERSION SSLEAY_VERSION

#else
# define BERYL_OPENSSL_OPAQUE_BIO
#endif

enum issl_status { ISSL_NONE, IS_SSL_CONNECTING, ISSL_OPEN };

static bool SelfSigned = false;
static int exdataindex;

char* get_error()
{
	return ERR_error_string(ERR_get_error(), NULL);
}

static int OnVerify(int preverify_ok, X509_STORE_CTX* ctx);
static void StaticssldataCallback(const SSL* ssl, int where, int rc);

namespace OpenSSL
{
	class Exception : public ModuleException
	{
		public:

			Exception(const std::string& reason) : ModuleException(reason) 
			{ 
			
			}
	};

	class DHParams
	{
		DH* dh;

	 public:

		DHParams(const std::string& filename)
		{
			BIO* dhpfile = BIO_new_file(filename.c_str(), "r");
			
			if (dhpfile == NULL)
			{
				throw Exception("Unable to open DH file: " + filename);
			}

			dh = PEM_read_bio_DHparams(dhpfile, NULL, NULL, NULL);
			BIO_free(dhpfile);

			if (!dh)
			{
				throw Exception("Unable to read dh data from: " + filename);
			}
		}

		~DHParams()
		{
			DH_free(dh);
		}

		DH* get()
		{
			return dh;
		}
	};

	class Context
	{
		SSL_CTX* const ctx;
		long ctx_options;

	 public:
		Context(SSL_CTX* context)
			: ctx(context)
		{
			long opts = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION | SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_SINGLE_DH_USE;
#ifdef SSL_OP_SINGLE_ECDH_USE
			opts |= SSL_OP_SINGLE_ECDH_USE;
#endif
#ifdef SSL_OP_NO_TICKET
			opts |= SSL_OP_NO_TICKET;
#endif

			ctx_options = SSL_CTX_set_options(ctx, opts);

			long mode = SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER;
#ifdef SSL_MODE_RELEASE_BUFFERS
			mode |= SSL_MODE_RELEASE_BUFFERS;
#endif
			SSL_CTX_set_mode(ctx, mode);
			SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
			SSL_CTX_set_session_cache_mode(ctx, SSL_SESS_CACHE_OFF);
			SSL_CTX_set_info_callback(ctx, StaticssldataCallback);
		}

		~Context()
		{
			SSL_CTX_free(ctx);
		}

		bool SetDH(DHParams& dh)
		{
			ERR_clear_error();
			return (SSL_CTX_set_tmp_dh(ctx, dh.get()) >= 0);
		}

#ifndef OPENSSL_NO_ECDH
		void SetECDH(const std::string& curvename)
		{
			int nid = OBJ_sn2nid(curvename.c_str());

			if (nid == 0)
			{
				throw Exception("Unknown curve: " + curvename);
			}

			EC_KEY* eckey = EC_KEY_new_by_curve_name(nid);

			if (!eckey)
			{
				throw Exception("Unable to create EC key object");
			}

			ERR_clear_error();
			bool ret = (SSL_CTX_set_tmp_ecdh(ctx, eckey) >= 0);
			EC_KEY_free(eckey);
			
			if (!ret)
			{
				throw Exception("Couldn't set ECDH parameters");
			}
		}
#endif

		bool SetCiphers(const std::string& ciphers)
		{
			ERR_clear_error();
			return SSL_CTX_set_cipher_list(ctx, ciphers.c_str());
		}

		bool SetCerts(const std::string& filename)
		{
			ERR_clear_error();
			return SSL_CTX_use_certificate_chain_file(ctx, filename.c_str());
		}

		bool SetPrivateKey(const std::string& filename)
		{
			ERR_clear_error();
			return SSL_CTX_use_PrivateKey_file(ctx, filename.c_str(), SSL_FILETYPE_PEM);
		}

		bool SetCA(const std::string& filename)
		{
			ERR_clear_error();
			return SSL_CTX_load_verify_locations(ctx, filename.c_str(), 0);
		}

		void SetCRL(const std::string& crlfile, const std::string& crlpath, const std::string& crlmode)
		{
			if (crlfile.empty() && crlpath.empty())
			{
				return;
			}

			unsigned long crlflags = X509_V_FLAG_CRL_CHECK;

			if (stdhelpers::string::equalsci(crlmode, "chain"))
			{
				crlflags |= X509_V_FLAG_CRL_CHECK_ALL;
			}
			else if (!stdhelpers::string::equalsci(crlmode, "leaf"))
			{
				throw ModuleException("Unknown mode '" + crlmode + "'; expected either 'chain' (default) or 'leaf'");
			}

			X509_STORE* store = SSL_CTX_get_cert_store(ctx);

			if (!store)
			{
				throw ModuleException("Unable to retrieve X509_STORE from TLS.");
			}

			ERR_clear_error();

			if (!X509_STORE_load_locations(store,crlfile.empty() ? NULL : crlfile.c_str(), crlpath.empty() ? NULL : crlpath.c_str()))
			{
				int err = ERR_get_error();
				throw ModuleException("Unable to load CRL file '" + crlfile + "' or CRL path '" + crlpath + "': '" + (err ? ERR_error_string(err, NULL) : "unknown") + "'");
			}

			if (X509_STORE_set_flags(store, crlflags) != 1)
			{
				throw ModuleException("Unable to set X509 CRL flags");
			}
		}

		long GetDefaultContextOptions() const
		{
			return ctx_options;
		}

		long SetRawContextOptions(long setoptions, long clearoptions)
		{
			SSL_CTX_clear_options(ctx, SSL_CTX_get_options(ctx));
			SSL_CTX_set_options(ctx, ctx_options | setoptions);
			return SSL_CTX_clear_options(ctx, clearoptions);
		}

		void SetVerifyCert()
		{
			SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE, OnVerify);
		}

		SSL* InitSession()
		{
			SSL* sess = SSL_new(ctx);
			SSL_set_accept_state(sess); 
			return sess;
		}

		SSL* InitClientSession()
		{
			SSL* sess = SSL_new(ctx);
			SSL_set_connect_state(sess); 
			return sess;
		}
	};

	class Profile
	{

		const std::string name;

		DHParams dh;

		Context ctx;
		Context clictx;

		const EVP_MD* digest;
	
		std::string lasterr;

		const bool allowrenego;

		const unsigned int outrecsize;

		static int error_callback(const char* str, size_t len, void* u)
		{
			Profile* profile = reinterpret_cast<Profile*>(u);
			profile->lasterr = std::string(str, len - 1);
			return 0;
		}

		void SetContextOptions(const std::string& ctxname, config_rule* tag, Context& context)
		{
			long setoptions = tag->as_int(ctxname + "setoptions", 0);
			long clearoptions = tag->as_int(ctxname + "clearoptions", 0);

#ifdef SSL_OP_NO_COMPRESSION

			if (!tag->as_bool("compression", false))
			{
				setoptions |= SSL_OP_NO_COMPRESSION;
			}
#endif

			if (!tag->as_bool("tlsv1", false))
			{
				setoptions |= SSL_OP_NO_TLSv1;
			}

#ifdef SSL_OP_NO_TLSv1_1

			if (!tag->as_bool("tlsv11", true))
			{
				setoptions |= SSL_OP_NO_TLSv1_1;
			}
#endif

#ifdef SSL_OP_NO_TLSv1_2
			if (!tag->as_bool("tlsv12", true))
			{
				setoptions |= SSL_OP_NO_TLSv1_2;
			}
#endif

			if (!setoptions && !clearoptions)
			{
				return; 
			}

			long final = context.SetRawContextOptions(setoptions, clearoptions);
		}

	 public:
		Profile(const std::string& profilename, config_rule* tag)
			: name(profilename)
			, dh(Kernel->Config->Paths.PrependConfig(tag->as_string("dhfile", "dhparams.pem", 1)))
			, ctx(SSL_CTX_new(SSLv23_server_method()))
			, clictx(SSL_CTX_new(SSLv23_client_method()))
			, allowrenego(tag->as_bool("renegotiation")) 
			, outrecsize(tag->as_uint("outrecsize", 2048, 512, 16384))
		{
			if ((!ctx.SetDH(dh)) || (!clictx.SetDH(dh)))
			{
				throw Exception("Couldn't set DH parameters");
			}

			const std::string hash = tag->as_string("hash", "md5", 1);
			digest = EVP_get_digestbyname(hash.c_str());

			if (digest == NULL)
			{
				throw Exception("Unknown hash type " + hash);
			}

			std::string ciphers = tag->as_string("ciphers");

			if (!ciphers.empty())
			{
				if ((!ctx.SetCiphers(ciphers)) || (!clictx.SetCiphers(ciphers)))
				{
					ERR_print_errors_cb(error_callback, this);
					throw Exception("Can't set cipher list to \"" + ciphers + "\" " + lasterr);
				}
			}

#ifndef OPENSSL_NO_ECDH
			const std::string curvename = tag->as_string("ecdhcurve", "prime256v1", 1);
			if (!curvename.empty())
				ctx.SetECDH(curvename);
#endif

			SetContextOptions("server", tag, ctx);
			SetContextOptions("client", tag, clictx);

			std::string filename = Kernel->Config->Paths.PrependConfig(tag->as_string("certfile", "cert.pem", 1));
			
			if ((!ctx.SetCerts(filename)) || (!clictx.SetCerts(filename)))
			{
				ERR_print_errors_cb(error_callback, this);
				throw Exception("Can't read certificate file: " + lasterr);
			}

			filename = Kernel->Config->Paths.PrependConfig(tag->as_string("keyfile", "key.pem", 1));
			
			if ((!ctx.SetPrivateKey(filename)) || (!clictx.SetPrivateKey(filename)))
			{
				ERR_print_errors_cb(error_callback, this);
				throw Exception("Can't read key file: " + lasterr);
			}

			filename = Kernel->Config->Paths.PrependConfig(tag->as_string("cafile", "ca.pem", 1));
			
			if ((!ctx.SetCA(filename)) || (!clictx.SetCA(filename)))
			{
				ERR_print_errors_cb(error_callback, this);
			}

			const std::string crlfile = tag->as_string("crlfile");
			const std::string crlpath = tag->as_string("crlpath");
			const std::string crlmode = tag->as_string("crlmode", "chain", 1);
			ctx.SetCRL(crlfile, crlpath, crlmode);

			clictx.SetVerifyCert();

			if (tag->as_bool("cert_required", true))
			{
				ctx.SetVerifyCert();
			}
		}

		const std::string& GetName() const 
		{ 
			return name; 
		}

		SSL* InitSession() 
		{ 
			return ctx.InitSession(); 
		}

		SSL* InitClientSession() 
		{ 
			return clictx.InitClientSession(); 
		}

		const EVP_MD* GetDigest() 
		{ 
			return digest; 
		}

		bool AllowRenegotiation() const 
		{ 
			return allowrenego; 
		}

		unsigned int GetOutgoingRecordSize() const 
		{ 
			return outrecsize; 
		}
	};

	namespace BIOMethod
	{
		static int create(BIO* bio)
		{
			BIO_set_init(bio, 1);
			return 1;
		}

		static int destroy(BIO* bio)
		{
			return 1;
		}

		static long ctrl(BIO* bio, int cmd, long num, void* ptr)
		{
			if (cmd == BIO_CTRL_FLUSH)
			{
				return 1;
			}
			
			return 0;
		}

		static int read(BIO* bio, char* buf, int len);
		static int write(BIO* bio, const char* buf, int len);

#ifdef BERYL_OPENSSL_OPAQUE_BIO
		static BIO_METHOD* alloc()
		{
			BIO_METHOD* meth = BIO_meth_new(100 | BIO_TYPE_SOURCE_SINK, "inspircd");
			BIO_meth_set_write(meth, OpenSSL::BIOMethod::write);
			BIO_meth_set_read(meth, OpenSSL::BIOMethod::read);
			BIO_meth_set_ctrl(meth, OpenSSL::BIOMethod::ctrl);
			BIO_meth_set_create(meth, OpenSSL::BIOMethod::create);
			BIO_meth_set_destroy(meth, OpenSSL::BIOMethod::destroy);
			return meth;
		}
#endif
	}
}

#ifndef BERYL_OPENSSL_OPAQUE_BIO
static BIO_METHOD biomethods =
{
	(100 | BIO_TYPE_SOURCE_SINK),
	"beryl",
	OpenSSL::BIOMethod::write,
	OpenSSL::BIOMethod::read,
	NULL, 
	NULL, 
	OpenSSL::BIOMethod::ctrl,
	OpenSSL::BIOMethod::create,
	OpenSSL::BIOMethod::destroy, 
	NULL 
};
#else
static BIO_METHOD* biomethods;
#endif

static int OnVerify(int preverify_ok, X509_STORE_CTX *ctx)
{
	int ve = X509_STORE_CTX_get_error(ctx);
	SelfSigned = (ve == X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT);

	return 1;
}

class OpenSSLQueue : public SSLQueue
{
 private:
	SSL* sess;
	issl_status status;
	bool data_to_write;

	int Handshake(StreamSocket* user)
	{
		ERR_clear_error();
		int ret = SSL_do_handshake(sess);
		
		if (ret < 0)
		{
			int err = SSL_get_error(sess, ret);

			if (err == SSL_ERROR_WANT_READ)
			{
				SocketPool::EventSwitch(user, Q_REQ_POOL_READ | Q_NO_WRITE);
				this->status = IS_SSL_CONNECTING;
				return 0;
			}
			else if (err == SSL_ERROR_WANT_WRITE)
			{
				SocketPool::EventSwitch(user, Q_NO_READ | Q_SINGLE_WRITE);
				this->status = IS_SSL_CONNECTING;
				return 0;
			}
			else
			{
				CloseSession();
				return -1;
			}
		}
		else if (ret > 0)
		{
			CheckCertificate();
			status = ISSL_OPEN;

			SocketPool::EventSwitch(user, Q_REQ_POOL_READ | Q_NO_WRITE | Q_ADD_WRITE_TRIAL);

			return 1;
		}
		else if (ret == 0)
		{
			CloseSession();
		}
		return -1;
	}

	void CloseSession()
	{
		if (sess)
		{
			SSL_shutdown(sess);
			SSL_free(sess);
		}
		sess = NULL;
		certificate = NULL;
		status = ISSL_NONE;
	}

	void CheckCertificate()
	{
		X509* cert;
		SSLCertificate* certinfo = new SSLCertificate;
		this->certificate = certinfo;
		unsigned int n;
		unsigned char md[EVP_MAX_MD_SIZE];

		cert = SSL_get_peer_certificate(sess);

		if (!cert)
		{
			certinfo->error = "Could not get peer certificate: "+std::string(get_error());
			return;
		}

		certinfo->invalid = (SSL_get_verify_result(sess) != X509_V_OK);

		if (!SelfSigned)
		{
			certinfo->unknownsigner = false;
			certinfo->trusted = true;
		}
		else
		{
			certinfo->unknownsigner = true;
			certinfo->trusted = false;
		}

		char buf[512];
		X509_NAME_oneline(X509_get_subject_name(cert), buf, sizeof(buf));
		certinfo->dn = buf;

		if (certinfo->dn.find_first_of("\r\n") != std::string::npos)
		{
			certinfo->dn.clear();
		}

		X509_NAME_oneline(X509_get_issuer_name(cert), buf, sizeof(buf));
		certinfo->issuer = buf;
		
		if (certinfo->issuer.find_first_of("\r\n") != std::string::npos)
		{
			certinfo->issuer.clear();
		}

		if (!X509_digest(cert, GetProfile().GetDigest(), md, &n))
		{
			certinfo->error = "Out of memory generating fingerprint";
		}
		else
		{
			certinfo->fingerprint = bin_to_hex(md, n);
		}

		if ((ASN1_UTCTIME_cmp_time_t(X509_getm_notAfter(cert), Kernel->Now()) == -1) || (ASN1_UTCTIME_cmp_time_t(X509_getm_notBefore(cert), Kernel->Now()) == 0))
		{
			certinfo->error = "Not activated nor expired.";
		}

		X509_free(cert);
	}

	void ssldataCallback(int where, int rc)
	{
		if ((where & SSL_CB_HANDSHAKE_START) && (status == ISSL_OPEN))
		{
			if (GetProfile().AllowRenegotiation())
			{
				return;
			}

			status = ISSL_NONE;
			BIO* bio = SSL_get_rbio(sess);
			EventHandler* ehandler = static_cast<StreamSocket*>(BIO_get_data(bio));
			SocketPool::Shutdown(ehandler, 2);
		}
	}

	bool CheckRetry(StreamSocket* sock)
	{
		if (status != ISSL_NONE)
		{
			return true;
		}

		CloseSession();
		sock->SetError("Renegotiation NOT allowed.");
		return false;
	}

	int PrepareIO(StreamSocket* sock)
	{
		if (status == ISSL_OPEN)
		{
			return 1;
		}
		else if (status == IS_SSL_CONNECTING)
		{
			return Handshake(sock);
		}

		CloseSession();
		return -1;
	}

	friend void StaticssldataCallback(const SSL* ssl, int where, int rc);

 public:
 
	OpenSSLQueue(QueueProvider* hookprov, StreamSocket* sock, SSL* session)
		: SSLQueue(hookprov)
		, sess(session)
		, status(ISSL_NONE)
		, data_to_write(false)
	{

#ifdef BERYL_OPENSSL_OPAQUE_BIO
		BIO* bio = BIO_new(biomethods);
#else
		BIO* bio = BIO_new(&biomethods);
#endif
		BIO_set_data(bio, sock);
		SSL_set_bio(sess, bio, bio);

		SSL_set_ex_data(sess, exdataindex, this);
		sock->AddIOQueue(this);
		Handshake(sock);
	}

	void OnStreamSocketClose(StreamSocket* user)
	{
		CloseSession();
	}

	int OnStreamSocketRead(StreamSocket* user, std::string& recvq)
	{
		int prepret = PrepareIO(user);

		if (prepret <= 0)
		{
			return prepret;
		}

		{
			ERR_clear_error();
			char* buffer = Kernel->GetPendingBuffer();
			int ret = SSL_read(sess, buffer, BUFFERSIZE);

			if (!CheckRetry(user))
			{
				return -1;
			}

			if (ret > 0)
			{
				recvq.append(buffer, ret);
				int mask = 0;
				if (SSL_pending(sess) > 0)
					mask |= Q_ADD_TRIAL_READ;
				if (data_to_write)
					mask |= Q_REQ_POOL_READ | Q_SINGLE_WRITE;
				if (mask != 0)
					SocketPool::EventSwitch(user, mask);
				return 1;
			}
			else if (ret == 0)
			{
				CloseSession();
				user->SetError("Connection closed");
				return -1;
			}
			else 
			{
				int err = SSL_get_error(sess, ret);

				if (err == SSL_ERROR_WANT_READ)
				{
					SocketPool::EventSwitch(user, Q_REQ_POOL_READ);
					return 0;
				}
				else if (err == SSL_ERROR_WANT_WRITE)
				{
					SocketPool::EventSwitch(user, Q_NO_READ | Q_SINGLE_WRITE);
					return 0;
				}
				else
				{
					CloseSession();
					return -1;
				}
			}
		}
	}

	int OnStreamSocketWrite(StreamSocket* user, StreamSocket::send_queue& sendq)
	{
		int prepret = PrepareIO(user);

		if (prepret <= 0)
		{
			return prepret;
		}

		data_to_write = true;

		while (!sendq.empty())
		{
			ERR_clear_error();
			QuickDispatch(sendq, GetProfile().GetOutgoingRecordSize());
			const StreamSocket::send_queue::Element& buffer = sendq.front();
			int ret = SSL_write(sess, buffer.data(), buffer.size());

			if (!CheckRetry(user))
			{
				return -1;
			}

			if (ret == (int)buffer.length())
			{
				sendq.pop_front();
			}
			else if (ret > 0)
			{
				sendq.erase_front(ret);
				SocketPool::EventSwitch(user, Q_SINGLE_WRITE);
				return 0;
			}
			else if (ret == 0)
			{
				CloseSession();
				return -1;
			}
			else 
			{
				int err = SSL_get_error(sess, ret);

				if (err == SSL_ERROR_WANT_WRITE)
				{
					SocketPool::EventSwitch(user, Q_SINGLE_WRITE);
					return 0;
				}
				else if (err == SSL_ERROR_WANT_READ)
				{
					SocketPool::EventSwitch(user, Q_REQ_POOL_READ);
					return 0;
				}
				else
				{
					CloseSession();
					return -1;
				}
			}
		}

		data_to_write = false;
		SocketPool::EventSwitch(user, Q_REQ_POOL_READ | Q_NO_WRITE);
		return 1;
	}

	void GetCiphersuite(std::string& out)
	{
		if (!HandshakeFinished())
		{
			return;
		}
		
		out.append(SSL_get_version(sess)).push_back('-');
		out.append(SSL_get_cipher(sess));
	}

	bool GetServerName(std::string& out)
	{
		const char* name = SSL_get_servername(sess, TLSEXT_NAMETYPE_host_name);
		
		if (!name)
		{
			return false;
		}

		out.append(name);
		return true;
	}

	bool HandshakeFinished() const 
	{ 
		return (status == ISSL_OPEN); 
	}

	OpenSSL::Profile& GetProfile();
};

static void StaticssldataCallback(const SSL* ssl, int where, int rc)
{
	OpenSSLQueue* hook = static_cast<OpenSSLQueue*>(SSL_get_ex_data(ssl, exdataindex));
	hook->ssldataCallback(where, rc);
}

static int OpenSSL::BIOMethod::write(BIO* bio, const char* buffer, int size)
{
	BIO_clear_retry_flags(bio);

	StreamSocket* sock = static_cast<StreamSocket*>(BIO_get_data(bio));

	if (sock->GetEventMask() & Q_WRITE_BLOCK)
	{
		BIO_set_retry_write(bio);
		return -1;
	}

	int ret = SocketPool::Send(sock, buffer, size, 0);

	if ((ret < size) && ((ret > 0) || (SocketPool::IgnoreError())))
	{
		SocketPool::EventSwitch(sock, Q_WRITE_BLOCK);
		BIO_set_retry_write(bio);
	}

	return ret;
}

static int OpenSSL::BIOMethod::read(BIO* bio, char* buffer, int size)
{
	BIO_clear_retry_flags(bio);

	StreamSocket* sock = static_cast<StreamSocket*>(BIO_get_data(bio));

	if (sock->GetEventMask() & Q_READ_WILL_BLOCK)
	{
		BIO_set_retry_read(bio);
		return -1;
	}

	int ret = SocketPool::Recv(sock, buffer, size, 0);
	
	if ((ret < size) && ((ret > 0) || (SocketPool::IgnoreError())))
	{
		SocketPool::EventSwitch(sock, Q_READ_WILL_BLOCK);
		BIO_set_retry_read(bio);
	}

	return ret;
}

class OpenSSLQueueProvider : public SSLQueueProvider
{
	OpenSSL::Profile profile;

 public:

	OpenSSLQueueProvider(Module* mod, const std::string& profilename, config_rule* tag)
		: SSLQueueProvider(mod, profilename)
		, profile(profilename, tag)
	{
		Kernel->Modules->AttachService(*this);
	}

	~OpenSSLQueueProvider()
	{
		Kernel->Modules->UnAttachService(*this);
	}

	void OnAccept(StreamSocket* sock, engine::sockets::sockaddrs* client, engine::sockets::sockaddrs* server)
	{
		new OpenSSLQueue(this, sock, profile.InitSession());
	}

	void OnConnect(StreamSocket* sock) 
	{
		new OpenSSLQueue(this, sock, profile.InitClientSession());
	}

	OpenSSL::Profile& GetProfile() { return profile; }
};

OpenSSL::Profile& OpenSSLQueue::GetProfile()
{
	QueueProvider* hookprov = prov;
	return static_cast<OpenSSLQueueProvider*>(hookprov)->GetProfile();
}

class ModuleOpenSSL : public Module
{
	typedef std::vector<reference<OpenSSLQueueProvider> > ProfileList;

	ProfileList profiles;

	void ReadProfiles()
	{
		ProfileList newprofiles;
		MultiTag tags = Kernel->Config->GetTags("ssldata");

		if (tags.first == tags.second)
		{
			const std::string defname = "openssl";
			config_rule* tag = Kernel->Config->GetConf(defname);

			try
			{
				newprofiles.push_back(new OpenSSLQueueProvider(this, defname, tag));
			}
			catch (OpenSSL::Exception& ex)
			{
				throw ModuleException("Error while initializing the default TLS (SSL) profile  " + ex.get_reason());
			}
		}
		else
		{
			for (config_iterator i = tags.first; i != tags.second; ++i)
			{
				config_rule* tag = i->second;
				
				if (!stdhelpers::string::equalsci(tag->as_string("provider"), "openssl"))
				{
					continue;
				}

				std::string name = tag->as_string("name");
			
				if (name.empty())
				{
					continue;
				}

				reference<OpenSSLQueueProvider> prov;
				try
				{
					prov = new OpenSSLQueueProvider(this, name, tag);
				}
				catch (KernelException& ex)
				{
					throw ModuleException("Error while initializing TLS (SSL) profile \"" + name + "\" at " + tag->get_tag_location() + " - " + ex.get_reason());
				}

				newprofiles.push_back(prov);
			}
		}

		for (ProfileList::iterator i = profiles.begin(); i != profiles.end(); ++i)
		{
			OpenSSLQueueProvider& prov = **i;
			Kernel->Modules.UnAttachService(prov);
		}

		profiles.swap(newprofiles);
	}

 public:
 
	ModuleOpenSSL()
	{
		OPENSSL_init_ssl(0, NULL);
#ifdef BERYL_OPENSSL_OPAQUE_BIO
		biomethods = OpenSSL::BIOMethod::alloc();
	}

	~ModuleOpenSSL()
	{
		BIO_meth_free(biomethods);
#endif
	}

	void Initialize()
	{
		char exdatastr[] = "beryl";

		exdataindex = SSL_get_ex_new_index(0, exdatastr, NULL, NULL, NULL);
		
		if (exdataindex < 0)
		{
			throw ModuleException("Failed to register app.");
		}

		ReadProfiles();
	}

	void OnCleanup(extension_item::ExpandableType type, Expandable* item) 
	{
		if (type == extension_item::EXT_USER)
		{
			LocalUser* user = IS_LOCAL((User*)item);

			if ((user) && (user->usercon.GetModuleLink(this)))
			{
				Kernel->Clients->Disconnect(user, "OpenSSL module unloading");
			}
		}
	}

	ModuleResult OnUserReady(LocalUser* user)
	{
		const OpenSSLQueue* const ioqueue = static_cast<OpenSSLQueue*>(user->usercon.GetModuleLink(this));

		if ((ioqueue) && (!ioqueue->HandshakeFinished()))
		{
			return MOD_RES_STOP;
		}
		
		return MOD_RES_SKIP;
	}

	Version GetDescription() 
	{
                return Version("Provides SSL support to Beryl.", VF_BERYLDB|VF_CORE);

	}
};

MODULE_LOAD(ModuleOpenSSL)
