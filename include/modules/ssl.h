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

#pragma once

#include <string>
#include "queues.h"

class SSLCertificate : public refcountbase
{
 public:
 
	std::string dn;
	std::string issuer;
	std::string error;
	std::string fingerprint;
	bool trusted, invalid, unknownsigner, revoked;

	SSLCertificate() : trusted(false), invalid(true), unknownsigner(true), revoked(false) 
	{
	
	}

	const std::string& GetDN()
	{
		return dn;
	}

	const std::string& GetIssuer()
	{
		return issuer;
	}

	const std::string& GetError()
	{
		return error;
	}

	const std::string& GetFingerprint()
	{
		return fingerprint;
	}

	bool IsTrusted()
	{
		return trusted;
	}

	bool IsInvalid()
	{
		return invalid;
	}

	bool IsUnknownSigner()
	{
		return unknownsigner;
	}

	bool IsRevoked()
	{
		return revoked;
	}

	bool IsUsable()
	{
		return !invalid && !revoked && error.empty();
	}

	bool IsCAVerified()
	{
		return IsUsable() && trusted && !unknownsigner;
	}

	std::string GetMetaLine()
	{
		std::stringstream value;
		bool hasError = !error.empty();
		value << (IsInvalid() ? "v" : "V") << (IsTrusted() ? "T" : "t") << (IsRevoked() ? "R" : "r")
			<< (IsUnknownSigner() ? "s" : "S") << (hasError ? "E" : "e") << " ";
		if (hasError)
			value << GetError();
		else
			value << GetFingerprint() << " " << GetDN() << " " << GetIssuer();
		return value.str();
	}
};

class SSLQueueProvider : public QueueProvider
{
   public:

	SSLQueueProvider(Module* mod, const std::string& Name) : QueueProvider(mod, "ssl/" + Name, IOH_SSL)
	{
	
	}
};

class SSLQueue : public IOQueue
{
 protected:

	reference<SSLCertificate> certificate;

	static void QuickDispatch(StreamSocket::send_queue& sendq, size_t targetsize)
	{
		if ((sendq.size() <= 1) || (sendq.front().length() >= targetsize))
		{
			return;
		}

		std::string tmp;
		tmp.reserve(std::min(targetsize, sendq.bytes())+1);
		
		do
		{
			tmp.append(sendq.front());
			sendq.pop_front();
		}
		while (!sendq.empty() && tmp.length() < targetsize);
		
		sendq.push_front(tmp);
	}

 public:

	static SSLQueue* IsSSL(StreamSocket* sock)
	{
		IOQueue* const lasthook = sock->GetLastHook();
		
		if (lasthook && (lasthook->prov->type == QueueProvider::IOH_SSL))
		{
			return static_cast<SSLQueue*>(lasthook);
		}

		return NULL;
	}

	SSLQueue(QueueProvider* hookprov) : IOQueue(hookprov)
	{
	
	}

	virtual SSLCertificate* GetCertificate() const
	{
		return certificate;
	}

	virtual std::string GetFingerprint() const
	{
		SSLCertificate* cert = GetCertificate();

		if (cert && cert->IsUsable())
		{
			return cert->GetFingerprint();
		}
		
		return "";
	}

	virtual void GetCiphersuite(std::string& out) = 0;

	virtual bool GetServerName(std::string& out) = 0;
};

class SSLClientCert
{
 public:

	static SSLCertificate* GetCertificate(StreamSocket* sock)
	{
		SSLQueue* SSLQueue = SSLQueue::IsSSL(sock);

		if (!SSLQueue)
		{
			return NULL;
		}

		return SSLQueue->GetCertificate();
	}

	static std::string GetFingerprint(StreamSocket* sock)
	{
		SSLCertificate* cert = SSLClientCert::GetCertificate(sock);
		
		if (cert)
		{
			return cert->GetFingerprint();
		}
		
		return "";
	}
};

class CertificateBase : public DataProvider
{
 public:
	
	CertificateBase(Module* parent) : DataProvider(parent, "m_ssldata_api")
	{
	
	}

	virtual SSLCertificate* GetCertificate(User* user) = 0;

	virtual void SetCertificate(User* user, SSLCertificate* cert) = 0;

	std::string GetFingerprint(User* user)
	{
		SSLCertificate* cert = GetCertificate(user);

		if (cert)
		{
			return cert->GetFingerprint();
		}
		
		return "";
	}
};

class UserCertificateAPI : public ReferencedFrom<CertificateBase>
{
 public:

	UserCertificateAPI(Module* parent) : ReferencedFrom<CertificateBase>(parent, "m_ssldata_api")
	{
	
	}
};
