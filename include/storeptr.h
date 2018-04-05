// storeptr.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_STOREPTR_H_
#define _PRILIB_STOREPTR_H_
#include "macro.h"

PRILIB_BEGIN
template <typename T>
class StorePtr
{
public:
	explicit StorePtr(T *data = nullptr)
		: _data(data) {}

	operator T&() const {
		return *_data;
	}

	T& get() const {
		return *_data;
	}

	T* operator->() const {
		return _data.get();
	}

	void clear() {
		_data = nullptr;
	}

private:
	std::shared_ptr<T> _data;
};
PRILIB_END

#endif
