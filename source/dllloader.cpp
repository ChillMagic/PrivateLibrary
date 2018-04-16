#include "dllloader.h"
#include <memory>
#include <memory>
#include <type_traits>
#include <string>

#if (PRILIB_OS == PRILIB_OS_WINDOWS)
#	include <Windows.h>
#elif (PRILIB_OS == PRILIB_OS_LINUX)
#	include <dlfcn.h>
#endif

PRILIB_BEGIN
class DLLLoader::DLLPointer
{
public:
#if (PRILIB_OS == PRILIB_OS_WINDOWS)
	using Pointer = HINSTANCE;
	using Data = typename std::remove_pointer<Pointer>::type;
#elif (PRILIB_OS == PRILIB_OS_LINUX)
	using Pointer = void*;
	using Data = void;
#endif

public:
	DLLPointer(Pointer ptr)
		: data(ptr) {}

	operator Pointer() {
		return data;
	}

	Pointer data;
};

static DLLLoader::DLLPointer* dllcreate(DLLLoader::Path name, DLLLoader::Mode flag, DLLLoader::ErrorCode &errcode) {
	bool error = false;
	errcode = (DLLLoader::ErrorCode)(0);

#if (PRILIB_OS == PRILIB_OS_WINDOWS)
	auto ptr = LoadLibrary(name);
	if (ptr == nullptr) {
		error = true;
		errcode = GetLastError();
	}
#elif (PRILIB_OS == PRILIB_OS_LINUX)
	int nflag;
	switch (flag) {
	case DLLLoader::Mode::Lazy: nflag = RTLD_LAZY; break;
	case DLLLoader::Mode::Now: nflag = RTLD_NOW; break;
	case DLLLoader::Mode::Global: nflag = RTLD_GLOBAL; break;
	case DLLLoader::Mode::Local: nflag = RTLD_LOCAL; break;
	default:
		nflag = flag;
	}
	auto ptr = dlopen(name, nflag);
	if (ptr == nullptr || (errcode = dlerror()) != nullptr) {
		error = true;
		errcode = dlerror();
	}
#endif

	return error ? nullptr : new DLLLoader::DLLPointer(ptr);
}

static void dllclose(DLLLoader::DLLPointer *_data) {
	if (_data == nullptr) return;
	DLLLoader::DLLPointer::Pointer data = *_data;
	delete _data;

	if (data == nullptr) return;

#if (PRILIB_OS == PRILIB_OS_WINDOWS)
	FreeLibrary(data);
#elif (PRILIB_OS == PRILIB_OS_LINUX)
	dlclose(data);
#endif
}

static void* dllgetfunc(DLLLoader::DLLPointer data, const char *func_name) {
#if (PRILIB_OS == PRILIB_OS_WINDOWS)
	return (void*)GetProcAddress(data, func_name);
#elif (PRILIB_OS == PRILIB_OS_LINUX)
	return dlsym(data, func_name);
#endif
}

// Class DLLLoader

DLLLoader::DLLLoader(DLLLoader::Path name, Mode flag)
	: _data(dllcreate(name, flag, _errcode), dllclose) {}


void DLLLoader::open(DLLLoader::Path name, Mode flag) {
	DLLPointer *ptr = dllcreate(name, flag, _errcode);
	this->_data = std::shared_ptr<DLLPointer>(ptr, dllclose);
}

void DLLLoader::close() {
	dllclose(_data.get());
	this->_data = nullptr;
}

void* DLLLoader::_get(const char *func_name) const {
	return reinterpret_cast<void*>(dllgetfunc(*_data, func_name));
}
PRILIB_END
