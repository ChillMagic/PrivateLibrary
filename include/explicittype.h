// explicittype.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_EXPLICITTYPE_H_
#define _PRILIB_EXPLICITTYPE_H_
#include "macro.h"

PRILIB_BEGIN
template <typename _Ty, _Ty... _DfV>
class ExplicitType;

template <typename _Ty>
class ExplicitType<_Ty>
{
public:
	using Type = _Ty;

	explicit ExplicitType(const _Ty &data)
		: data(data) {}

	explicit ExplicitType(_Ty &&data)
		: data(data) {}

	_Ty data;
};

template <typename _Ty, _Ty _DfV>
class ExplicitType<_Ty, _DfV> : public ExplicitType<_Ty>
{
public:
	explicit ExplicitType(const _Ty &data)
		: ExplicitType<_Ty>(data) {}

	explicit ExplicitType(_Ty &&data)
		: ExplicitType<_Ty>(data) {}

	ExplicitType()
		: ExplicitType<_Ty>(_DfV) {}
};
PRILIB_END

#endif
