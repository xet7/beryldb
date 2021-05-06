
namespace brld
{

template <typename T, typename Tag>
class BERYL_node_list_NAME
{
 public:
	class iterator : public std::iterator<std::bidirectional_iterator_tag, T*>
	{
		T* curr;

	 public:
		iterator(T* i = NULL)
			: curr(i)
		{
		}

		iterator& operator++()
		{
			curr = curr->node_list_node<T, Tag>::ptr_next;
			return *this;
		}

		iterator operator++(int)
		{
			iterator ret(*this);
			operator++();
			return ret;
		}

		iterator& operator--()
		{
			curr = curr->node_list_node<T, Tag>::ptr_prev;
			return *this;
		}

		iterator operator--(int)
		{
			iterator ret(*this);
			operator--();
			return ret;
		}

		bool operator==(const iterator& other) const { return (curr == other.curr); }
		bool operator!=(const iterator& other) const { return (curr != other.curr); }
		T* operator*() const { return curr; }
	};

	typedef iterator const_iterator;

	BERYL_node_list_NAME()
		: listhead(NULL)
#ifdef BERYL_node_list_HAS_TAIL
		, listtail(NULL)
#endif
		, listsize(0)
	{
	}

	bool empty() const
	{
		return (size() == 0);
	}

	size_t size() const
	{
		return listsize;
	}

	iterator begin() const
	{
		return iterator(listhead);
	}

	iterator end() const
	{
		return iterator();
	}

	void pop_front()
	{
		erase(listhead);
	}

	T* front() const
	{
		return listhead;
	}

	void push_front(T* x)
	{
		if (listsize++)
		{
			x->node_list_node<T, Tag>::ptr_next = listhead;
			listhead->node_list_node<T, Tag>::ptr_prev = x;
		}
#ifdef BERYL_node_list_HAS_TAIL
		else
			listtail = x;
#endif
		listhead = x;
	}

#ifdef BERYL_node_list_HAS_TAIL
	T* back() const
	{
		return listtail;
	}

	void push_back(T* x)
	{
		if (listsize++)
		{
			x->node_list_node<T, Tag>::ptr_prev = listtail;
			listtail->node_list_node<T, Tag>::ptr_next = x;
		}
		else
			listhead = x;
		listtail = x;
	}

	void pop_back()
	{
		erase(listtail);
	}
#endif

	void erase(const iterator& it)
	{
		erase(*it);
	}

	void erase(T* x)
	{
		if (listhead == x)
			listhead = x->node_list_node<T, Tag>::ptr_next;
#ifdef BERYL_node_list_HAS_TAIL
		if (listtail == x)
			listtail = x->node_list_node<T, Tag>::ptr_prev;
#endif
		x->node_list_node<T, Tag>::unlink();
		listsize--;
	}

 private:
	T* listhead;
#ifdef BERYL_node_list_HAS_TAIL
	T* listtail;
#endif
	size_t listsize;
};

} 
