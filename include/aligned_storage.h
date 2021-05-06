
#pragma once

namespace brld
{
	template <typename T> class aligned_storage;
}

template <typename T> class brld::aligned_storage
{

  private:
  
	mutable typename STR1::aligned_storage<sizeof(T), STR1::alignment_of<T>::value>::type data;

 public:

	aligned_storage()
	{
	
	}

	aligned_storage(const aligned_storage& other)
	{
	
	}

	T* operator->() const
	{
		return static_cast<T*>(static_cast<void*>(&data));
	}

	operator T*() const
	{
		return operator->();
	}
};
