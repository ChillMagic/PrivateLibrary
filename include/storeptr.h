// Name:    storeptr.h
// Date:    01/17/2018
// Version: 1.0.0.0

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

private:
	std::shared_ptr<T> _data;
};
PRILIB_END

#endif
