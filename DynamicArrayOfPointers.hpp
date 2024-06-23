#pragma once
#include <iostream>

//credit Angeld55 github - with slight changes

namespace HelperFunctions
{
	inline unsigned int getNextPowerOfTwo(unsigned int n)
	{
		unsigned int step = 1;

		while ((n >> step) > 0) {
			n |= n >> step;
			step *= 2;
		}

		return n + 1;
	}
}


template<class A>
class DynamicArrayOfPointers
{
	A** _data = nullptr;
	size_t _capacity = 0;
	size_t _count = 0;

	void free();
	void copyFrom(const DynamicArrayOfPointers& other);
	void resize(size_t newCap);

	void moveFrom(DynamicArrayOfPointers&& other);

	int getFirstFreeIndex() const; //Better solution will be to store a variable with the first free index
public:
	DynamicArrayOfPointers();
	DynamicArrayOfPointers(size_t capacity);
	DynamicArrayOfPointers(const DynamicArrayOfPointers& other);
	DynamicArrayOfPointers& operator=(const DynamicArrayOfPointers& other);
	~DynamicArrayOfPointers();

	DynamicArrayOfPointers(DynamicArrayOfPointers&& other) noexcept;
	DynamicArrayOfPointers& operator=(DynamicArrayOfPointers&& other) noexcept;

	void addAtFirstFreeIndex(A& current);
	void addAtFirstFreeIndex(A&& obj);

	bool containsAt(unsigned ind) const;
	//void popBack();
	void removeAt(size_t index);

	void setAtIndex(A& obj, size_t index);
	void setAtIndex(A&& obj, size_t index);


	size_t getSize() const;
	const A& operator[](size_t index) const;
	A& operator[](size_t index);
};

template <class A>
void DynamicArrayOfPointers<A>::free()
{
	delete[] _data;
}

template <class A>
void DynamicArrayOfPointers<A>::copyFrom(const DynamicArrayOfPointers& other)
{
	_count = other._count;
	_capacity = other._capacity;

	_data = new A * [_capacity];
	for (int i = 0; i < _capacity; i++)
	{
		//if (other._data[i] == nullptr)
		//	_data[i] = nullptr;
		//else
		//	_data[i] = new A(*other._data[i]); //To reuse the A box
		_data[i] = other._data[i];
	}
}

template <class A>
void DynamicArrayOfPointers<A>::resize(size_t newCap)
{
	A** newDataPtr = new A * [newCap] {nullptr};

	for (int i = 0; i < _capacity; i++)
		newDataPtr[i] = _data[i];

	delete[] _data;
	_data = newDataPtr;

	_capacity = newCap;


}

template <class A>
void DynamicArrayOfPointers<A>::moveFrom(DynamicArrayOfPointers&& other)
{
	_data = other._data;
	other._data = nullptr;

	_capacity = other._capacity;
	_count = other._count;

	other._count = 0;
}

template <class A>
int DynamicArrayOfPointers<A>::getFirstFreeIndex() const
{
	for (int i = 0; i < _capacity; i++)
		if (_data[i] == nullptr)
			return i;
	return -1;
}

template <class A>
DynamicArrayOfPointers<A>::DynamicArrayOfPointers()
{
	_capacity = 8;
	_data = new A * [_capacity] {nullptr};
}

template <class A>
DynamicArrayOfPointers<A>::DynamicArrayOfPointers(size_t capacity)
{
	_capacity = HelperFunctions::getNextPowerOfTwo(capacity);
	_data = new A * [capacity]{nullptr};
}

template <class A>
DynamicArrayOfPointers<A>::DynamicArrayOfPointers(const DynamicArrayOfPointers& other)
{
	copyFrom(other);
}

template <class A>
DynamicArrayOfPointers<A>& DynamicArrayOfPointers<A>::operator=(const DynamicArrayOfPointers& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

template <class A>
DynamicArrayOfPointers<A>::~DynamicArrayOfPointers()
{
	free();
}

template <class A>
DynamicArrayOfPointers<A>::DynamicArrayOfPointers(DynamicArrayOfPointers&& other) noexcept
{
	moveFrom(std::move(other));
}

template <class A>
DynamicArrayOfPointers<A>& DynamicArrayOfPointers<A>::operator=(DynamicArrayOfPointers&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

template <class A>
void DynamicArrayOfPointers<A>::addAtFirstFreeIndex(A& current)
{

	if (_count >= _capacity)
		resize(_capacity * 2);

	int ind = getFirstFreeIndex();
	if (ind == -1)
		return;

	_data[ind] = &current;
	_count++;
}

template <class A>
void DynamicArrayOfPointers<A>::addAtFirstFreeIndex(A&& obj)
{
	if (_count >= _capacity)
		resize(_capacity * 2);

	int ind = getFirstFreeIndex();
	if (ind == -1)
		return;

	_data[ind] = std::move(&obj);  //move ctor
	_count++;
}

template <class A>
bool DynamicArrayOfPointers<A>::containsAt(unsigned ind) const
{
	return ind < _capacity && _data[ind] != nullptr;
}

template <class A>
void DynamicArrayOfPointers<A>::removeAt(size_t index)
{
	if (index >= _capacity)
		return;

	//delete _data[index];
	std::swap(_data[index], _data[_count - 1]);
	_data[_count - 1] = nullptr;
	_count--;

	/*if (_count == _capacity / 4 && _capacity / 2 > 8)
		resize(_capacity / 2);*/
}

template <class A>
void DynamicArrayOfPointers<A>::setAtIndex(A& obj, size_t index)
{

	if (index >= _capacity)
		resize(_capacity * 2);

	/*
	delete _data[index];
	_data[index] = new A(obj);
	*/

	if (_data[index] != nullptr)
		_data[index]->operator=(obj);
	else
	{
		_data[index] = &obj;
		_count++;
	}
}

template <class A>
void DynamicArrayOfPointers<A>::setAtIndex(A&& obj, size_t index)
{
	if (_data[index] != nullptr)
		*_data[index] = obj;
	else
	{
		_data[index] = (std::move(&obj)); //move c-tor
		_count++;
	}
}

template <class A>
const A& DynamicArrayOfPointers<A>::operator[](size_t index) const
{
	return *_data[index]; //should be carefull for the empty places

}

template <class A>
A& DynamicArrayOfPointers<A>::operator[](size_t index)
{
	return *_data[index]; //should be carefull for the empty places

}

template <class A>
size_t DynamicArrayOfPointers<A>::getSize() const
{
	return _count;

}
