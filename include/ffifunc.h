// Name:    ffifunc.h
// Date:    09/15/2017
// Version: 1.0.0.0

#pragma once
#ifndef _PRILIB_FFIFUNC_H_
#define _PRILIB_FFIFUNC_H_
#include "macro.h"
#include "lightlist.h"
#if defined(_MSC_VER)
#	if !defined(FFI_BUILDING)
#		define FFI_BUILDING
#		include "libinc/ffi.h"
#		undef FFI_BUILDING
#	else
#		include "libinc/ffi.h"
#	endif
#else
#	include <ffi.h>
#endif

PRILIB_BEGIN
class FFIFunc
{
public:
	using VoidFunc = void(void);
	using ArgType = ffi_type*;
	using ArgTypeSet = lightlist<ArgType>;
	using ArgValueSet = lightlist<void*>;
public:

	template <typename _FTy>
	FFIFunc(_FTy *func, ArgType rtype, unsigned int arg_size)
		: FFIFunc(func, rtype, ArgTypeSet(arg_size)) {}

	template <typename _FTy>
	FFIFunc(_FTy *func, ArgType rtype, ArgTypeSet arg_types)
		: func((VoidFunc*)func), rtype(rtype), arg_types(arg_types) {}

	ArgValueSet init_values() {
		return ArgValueSet(arg_types.size());
	}
	ArgTypeSet& get_argtypes() {
		return arg_types;
	}

	template <typename _RTy>
	bool call(_RTy *result, void* values[]) const {
		ffi_cif cif;
		ffi_abi abi = FFI_DEFAULT_ABI;
		ffi_status state = ffi_prep_cif(&cif, abi, static_cast<unsigned int>(arg_types.size()), rtype, const_cast<ArgType*>(arg_types.get()));
		if (state != FFI_OK)
			return false;

		ffi_call(&cif, func, result, values);
		return true;
	}

	template <typename _RTy>
	bool call(_RTy *result, ArgValueSet &values) const {
		return call(result, values.get());
	}

private:
	VoidFunc* func;
	ArgTypeSet arg_types;
	ArgType rtype;
};
PRILIB_END

#endif
