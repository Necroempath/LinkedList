#pragma once
#include <iostream>
template <typename>
class LinkedList;

template <typename T>
class Node
{
	T _value;
	Node<T>* _next;
	Node<T>* _prev;

	friend class LinkedList<T>;
public:
	Node(const T& value) : _next(nullptr), _prev(nullptr), _value(value) {};

	T Value() const { return _value; }

	Node<T>* Next() const { return _next; }; 

	Node<T>* Prev() const { return _prev; };

	void SetValue(const T& value) { _value = value; };
};

template <typename T>
class LinkedList
{
	Node<T>* _head;
	Node<T>* _tail;
	size_t _len;

	Node<T>* Find(const Node<T>& node) const
	{
		Node<T>* current = _head;

		while (current)
		{
			if (current->Value() == node.Value())	return current;

			current = current->Next();
		}

		return nullptr;
	}

	Node<T>* Find(int index) const
	{
		if (index < 0 || index >= _len) return nullptr;

		Node<T>* current = _head;

		for (size_t i = 0; i < index; i++)
		{
			current = current->Next();
		}

		return current;
	}

public:
	class Iterator
	{
		Node<T>* _current;
		
	public:
		Iterator(Node<T>* begin) : _current(begin) {};

		bool operator==(const Iterator& other) const { return _current == other._current; }

		bool operator!=(const Iterator& other) const { return _current != other._current; }

		Node<T>& operator*() const { return *_current; }

		Iterator& operator++()
		{
			if (!_current) return *this;

			_current = _current->Next();

			return *this;
		}

		Iterator operator++(int)
		{
			if (!_current) return *this;

			auto temp = *this;

			_current = _current->Next();

			return temp;
		}

		Iterator& operator--()
		{
			if (!_current) return *this;

			_current = _current->Prev();

			return *this;
		}

		Iterator operator--(int)
		{
			if (!_current) return *this;

			auto temp = *this;

			_current = _current->Prev();

			return temp;
		}

		Iterator operator+(int index) const
		{
			auto temp = _current;

			for (size_t i = 0; i < index && temp; i++)
			{
				temp = temp->Next();
			}

			return Iterator(temp);
		}

		Iterator& operator+=(int index)
		{
			*this = *this + index;

			return *this;
		}

		Iterator operator-(int index) const
		{
			auto temp = _current;

			for (size_t i = 0; i < index && temp; i++)
			{
				temp = temp->Prev();
			}

			return Iterator(temp);
		}

		Iterator& operator-=(int index)
		{
			*this = *this - index;

			return *this;
		}
	};

	Iterator begin()
	{
		return Iterator(_head);
	}

	Iterator end()
	{
		return Iterator(_tail->Next());
	}

	LinkedList() : _head{ nullptr }, _tail{ nullptr }, _len(0) {};

	LinkedList(const LinkedList<T>& other)
	{
		Node<T>* current = other._head;

		while (current)
		{
			this->InsertAtEnd(current->Value());
			current = current->Next();
		}
	}

	LinkedList(LinkedList<T>&& other) noexcept : _head(other._head), _tail(other._tail), _len(other._len) 
	{
		other._head = nullptr;
		other._tail = nullptr;
		other._len = 0;
	}

	LinkedList<T>& operator=(const LinkedList<T>& other)
	{
		if (this == &other) return *this;

		this->Clear();

		Node<T>* current = other._head;

		while (current)
		{
			this->InsertAtEnd(current->Value());
			current = current->Next();
		}

		return *this;
	}

	LinkedList& operator=(LinkedList<T>&& other) noexcept
	{
		if (this == &other) return *this;

		this->Clear();

		_head = other._head;
		_tail = other._tail;
		_len = other._len;

		other._head = nullptr;
		other._tail = nullptr;
		other._len = 0;

		return *this;
	}

	const Node<T>* operator[](int index) const
	{
		try
		{
			Node<T>* node = Find(index);

			if (!node) throw std::out_of_range("Index out of range");

			return node;
		}
		catch (const std::out_of_range& e)
		{
			std::cerr << e.what();

			return nullptr;
		}
	}

	void Clear()
	{
		if (!_len) return;

		Node<T>* current = _head;

		while (current)
		{
			_head = _head->Next();
			delete current;
			current = _head;
		}

		_head = _tail = nullptr;
		_len = 0;
	}

	LinkedList<T>& InsertAtEnd(const T& value)
	{
		Node<T>* node = new Node<T>(value);

		if (!_len)
		{
			_head = _tail = node;
		}
		else
		{
			_tail->_next = node;
			node->_prev = _tail;
			_tail = node;
		}

		_len++;

		return *this;
	}

	LinkedList<T>& InsertToBegin(const T& value)
	{
		Node<T>* node = new Node<T>(value);

		if (!_len)
		{
			_head = _tail = node;
		}
		else
		{
			_head->_prev = node;
			node->_next = _head;
			_head = node;
		}

		_len++;

		return *this;
	}

	void PopFront()
	{
		if (_len < 2) Clear(); return;

		_head = _head->Next();
		delete _head->Prev();
		_head->_prev = nullptr;

		_len--;
	}

	void PopBack()
	{
		if (_len < 2) Clear(); return;

		_tail = _tail->Prev();
		delete _tail->Next();
		_tail->_next = nullptr;

		_len--;
	}

	bool Empty() const { return !_len; }

	void InsertAfter(const T& value, const Node<T>& node)
	{
		Node<T>* desired = Find(node);

		if (!desired) return;

		if (_tail == desired) { InsertAtEnd(value); return; }

		Node<T>* new_node = new Node<T>(value);

		desired->_next->_prev = new_node;
		new_node->_next = desired->Next();
		desired->_next = new_node;
		new_node->_prev = desired;

		_len++;
	}

	void Remove(const Node<T>& node)
	{
		Node<T>* desired = Find(node);

		if (!desired) return;

		if (_head == desired) { PopFront();		return; }
		else if (_tail == desired) { PopBack();		return; }

		desired->_prev->_next = desired->Next();
		desired->_next->_prev = desired->Prev();

		delete desired;

		_len--;
	}

	//void Reverse() //alternative way
	//{
	//	if (_len < 2) return;

	//	LinkedList<T> list;
	//	Node<T>* current = _tail;

	//	while (current)
	//	{
	//		list.InsertAtEnd(current->GetValue());
	//		current = current->Prev();
	//	}
	//	
	//	*this = std::move(list);
	//}

	void Reverse()
	{
		if (_len < 2) return;

		Node<T>* current = _head;

		while (current)
		{
			Node<T>* temp = current->Next();

			current->_next = current->Prev();
			current->_prev = temp;

			current = current->Prev();
		}

		Node<T>* temp = _tail;

		_tail = _head;
		_head = temp;
	}
	size_t GetLen() const { return _len; };

	~LinkedList()
	{
		Clear();
	}
};