// lightlist.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_LIGHTLIST_H_
#define _PRILIB_LIGHTLIST_H_
#include "macro.h"
#include "memory.h"
#include <memory>
#include <algorithm>

PRILIB_BEGIN
template <typename T>
class lightlist_creater;

template <typename T>
class lightlist
{
public:
	using value_type = T;
	using iterator = T*;
	using creater = lightlist_creater<T>;
public:
	lightlist() = default;
	lightlist(const lightlist &) = default;

	explicit lightlist(size_t capacity)
		: lightlist(capacity, Memory::alloc<T>, Memory::free<T>) {}

	template <typename AFTy, typename FFTy>
	explicit lightlist(size_t capacity, AFTy alloc, FFTy free)
		// This means lightlist can be used for value type (have no destructor).
		: _capacity(capacity), _data(alloc(_capacity), free) {}

	explicit lightlist(const std::initializer_list<T> &il)
		: lightlist(il.begin(), il.end()) {}

	template <typename Iter>
	lightlist(Iter begin, Iter end)
		: lightlist(static_cast<size_t>(end - begin)) {
		std::copy(begin, end, _PointerIterator(_data.get()));
	}

	template <typename Iter>
	lightlist(Iter begin, size_t capacity)
		: lightlist(capacity) {
		std::copy_n(begin, _capacity, _PointerIterator(_data.get()));
	}

	template <typename Iter>
	lightlist(Iter begin, Iter end, size_t capacity)
		: lightlist(capacity) {
		std::copy(begin, end, _PointerIterator(_data.get()));
	}

	template <size_t N>
	explicit lightlist(T (&arr)[N])
		: lightlist(std::begin(arr), std::end(arr)) {}

public:
	static lightlist create(T *src, size_t capacity) {
		lightlist r;
		r._capacity = capacity;
		r._data = std::shared_ptr<T>(src, [](T *ptr) {});
		return r;
	}

public:
	T* begin() { return _data.get(); }
	T* end() { return _data.get() + _capacity; }
	const T* begin() const { return _data.get(); }
	const T* end() const { return _data.get() + _capacity; }
	const T* cbegin() const { return _data.get(); }
	const T* cend() const { return _data.get() + _capacity; }

	T* get(size_t index = 0) { return _data.get() + index; }
	const T* get(size_t index = 0) const { return _data.get() + index; }

	T& operator[](size_t id) { return *get(id); }
	const T& operator[](size_t id) const { return *get(id); }

	T& at(size_t id) { assert(id < size()); return this->operator[](id); }
	const T& at(size_t id) const { assert(id < size()); return this->operator[](id); }

	T& front() { return *begin(); }
	const T& front() const { return *begin(); }
	T& back() { return *(_data.get() + _capacity - 1); }
	const T& back() const { return *(_data.get() + _capacity - 1); }

	size_t size() const { return _capacity; }
	bool empty() const { return _capacity == 0; }
	void resize(size_t nsize) { _capacity = std::min(_capacity, nsize); }
	void recapacity(size_t ncapacity) {
		size_t c = std::max(_capacity, ncapacity);
		lightlist nl(c);
		Memory::copyTo(nl._data.get(), _data.get(), _capacity);
		*this = nl;
	}

private:
	size_t _capacity = 0;
	std::shared_ptr<T> _data = nullptr;
};

template <typename T>
class lightlist_creater
{
public:
	explicit lightlist_creater(size_t n) : _count(0), _data(n) {}

	void push_back(const T &e) {
		_data[_count++] = e;
	}
	template <typename Iter>
	void insert(Iter beg, Iter end) {
		for (auto &e : rangei(beg, end)) {
			push_back(e);
		}
	}
	void clear() {
		for (size_t i : range(0, _data.size()))
			_data[i].~T();
		_count = 0;
	}
	lightlist<T>& data() {
		return _data;
	}
	const lightlist<T>& data() const {
		return _data;
	}
	lightlist<T> resize_data() const {
		lightlist<T> ndata(_data);
		ndata.resize(_count);
		return ndata;
	}
	size_t size() const {
		return _count;
	}
	size_t capacity() const {
		return _data.size();
	}

private:
	size_t _count;
	lightlist<T> _data;
};
PRILIB_END

#endif
