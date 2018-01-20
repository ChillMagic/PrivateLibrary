# PrivateLibrary
My private C++ library (C++ headers) for many projects.

## Usage

Put it to a directory ( example 'prilib' ).

Then:

```
mkdir build
cmake ..
make
```

Then, put `libprilib.a` to your project, and get it referred.

```
gcc -Lprilib/include -lprilib XXXX
```

Or use CMake, ...

If you want to change the name of namespace PriLib, you can use `cmake .. -DPRILIB=XXXX` .

If you want to a header file which include all headers, you can use 'prilib.h' (It was made by 'create-header.py').
