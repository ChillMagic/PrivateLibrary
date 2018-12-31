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

	explicit ExplicitType() {}

	_Ty data;
};

template <typename _Ty, _Ty _DfV>
class ExplicitType<_Ty, _DfV> : public ExplicitType<_Ty>
{
public:
	explicit ExplicitType(const _Ty &data)
		: ExplicitType<_Ty>(data) {}

	explicit ExplicitType(_Ty &&data)
		: ExplicitType<_Ty>(std::move(data)) {}

	explicit ExplicitType()
		: ExplicitType<_Ty>(_DfV) {}
};

#define DefineExplicitType(TypeName, Type) \
class TypeName : public ::PRILIB::ExplicitType<Type> { \
public: \
	explicit TypeName(const Type &data) : ::PRILIB::ExplicitType<Type>(data) {} \
	explicit TypeName(Type &&data) : ::PRILIB::ExplicitType<Type>(std::move(data)) {} \
	explicit TypeName() : ::PRILIB::ExplicitType<Type>() {} \
};

#define DefineExplicitTypeWithValue(TypeName, Type, Value) \
class TypeName : public ::PRILIB::ExplicitType<Type, Value> { \
public: \
	explicit TypeName(const Type &data) : ::PRILIB::ExplicitType<Type, Value>(data) {} \
	explicit TypeName(Type &&data) : ::PRILIB::ExplicitType<Type, Value>(std::move(data)) {} \
	explicit TypeName() : ::PRILIB::ExplicitType<Type, Value>(Value) {} \
};

PRILIB_END

#endif
