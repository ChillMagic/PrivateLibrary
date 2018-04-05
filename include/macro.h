// macro.h
// * PrivateLibrary

#pragma once
#ifndef PRILIB
#define PRILIB         PriLib

#define PRILIB_BEGIN   namespace PRILIB {
#define PRILIB_END     }

//#define self (*this)

PRILIB_BEGIN PRILIB_END

// Operating System Macro : PRILIB_OS

#if (!defined(PRILIB_OS))

#	define PRILIB_OS_UNIX 1
#	define PRILIB_OS_LINUX 2
#	define PRILIB_OS_WINDOWS 3
#	define PRILIB_OS_OTHERS 4

#	define PRILIB_OS_IS_UNIX(OS) ((PRILIB_OS == PRILIB_OS_LINUX) || (PRILIB_OS == PRILIB_OS_UNIX))

#	if (defined _WIN32)
#		define PRILIB_OS PRILIB_OS_WINDOWS
#	elif (defined(__linux__) || defined(__linux))
#		define PRILIB_OS PRILIB_OS_LINUX  
#	else
#		define PRILIB_OS PRILIB_OS_OTHERS
#	endif

#endif

#if (!defined(PRILIB_ARCH))

#	define PRILIB_ARCH_x64 1
#	define PRILIB_ARCH_x86 2
#	define PRILIB_ARCH_ARM 3
#	define PRILIB_ARCH_OTHERS 4

#	if (defined(__x86_64__) || defined(_M_X64) || defined(_WIN64))
#		define PRILIB_ARCH PRILIB_ARCH_x64
#	elif (defined(__i386) || defined(_M_IX86) || defined(_WIN32) || defined(WIN32))
#		define PRILIB_ARCH PRILIB_ARCH_x86
#	elif (defined(_CC_ARM _))
#		define PRILIB_ARCH PRILIB_ARCH_ARM
#	else
#		define PRILIB_ARCH PRILIB_ARCH_OTHERS
#	endif

#endif

#if (!defined(PRILIB_COMPILER))

#	define PRILIB_COMPILER_GCC    1
#	define PRILIB_COMPILER_CLANG  2
#	define PRILIB_COMPILER_MSVC   3
#	define PRILIB_COMPILER_OTHERS 4

#	if (defined(__clang__))  // Clang include '__GNUC__' or '_MSC_VER'
#		define PRILIB_COMPILER PRILIB_COMPILER_CLANG
#	elif (defined(__GNUC__))
#		define PRILIB_COMPILER PRILIB_COMPILER_GCC
#	elif (defined(_MSC_VER))
#		define PRILIB_COMPILER PRILIB_COMPILER_MSVC
#	else
#		define PRILIB_COMPILER PRILIB_COMPILER_OTHERS
#	endif

#	if (PRILIB_OS == PRILIB_OS_WINDOWS)
#		if (defined(_MSC_VER))
#			define PRILIB_COMPILER_IS_MSVC   true
#			define PRILIB_COMPILER_IS_MINGW  false
#			define PRILIB_COMPILER_IS_CYGWIN false
#		elif (defined(__MINGW32__))
#			define PRILIB_COMPILER_IS_MSVC   false
#			define PRILIB_COMPILER_IS_MINGW  true
#			define PRILIB_COMPILER_IS_CYGWIN false
#		elif (defined(__CYGWIN__))
#			define PRILIB_COMPILER_IS_MSVC   false
#			define PRILIB_COMPILER_IS_MINGW  false
#			define PRILIB_COMPILER_IS_CYGWIN true
#		else
#			define PRILIB_COMPILER_IS_MSVC   false
#			define PRILIB_COMPILER_IS_MINGW  false
#			define PRILIB_COMPILER_IS_CYGWIN false
#		endif
#	else
#	endif

#endif

#endif
