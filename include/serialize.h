
#pragma once



class ExportAPI Serializable
{
 protected:

	Serializable() 
	{ 
	
	}

 public:
	
	
	
	class ExportAPI Data
	{
	public:
		
		typedef brld::flat_map<std::string, Data> ChildMap;

		
		typedef brld::flat_map<std::string, std::string> EntryMap;

	private:
		
		ChildMap children;

		
		EntryMap entries;

	 public:
		 
		const ChildMap& GetChildren() const { return children; }
		ChildMap& GetChildren() { return children; }

		
		const EntryMap& GetEntries() const { return entries; }
		EntryMap& GetEntries() { return entries; }

		
		Data& Load(const std::string& key, Data& out);

		
		Data& Load(const std::string& key, std::string& out);

		
		template <typename T>
		Data& Load(const std::string& key, T& out)
		{
			std::string str;
			Load(key, str);

			std::stringstream ss(str);
			ss >> out;
			return *this;
		}

		
		Data& Store(const std::string& key, const Data& value);

		
		Data& Store(const std::string& key, const std::string& value);

		
		template <typename T>
		Data& Store(const std::string& key, const T& value)
		{
			return Store(key, convto_string(value));
		}
	};

	
	virtual bool Deserialize(Data& data) = 0;


	virtual bool Serialize(Data& data) = 0;
};
