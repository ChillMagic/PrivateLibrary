// matrix.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_MATRIX_H_
#define _PRILIB_MATRIX_H_
#include "macro.h"
#include "range.h"
#include "dyarray.h"
#include <numeric>
#include <algorithm>

PRILIB_BEGIN
template <typename T, size_t N, typename Sequence>
struct MatrixBase;

template <typename T, size_t N, size_t... Idx>
struct MatrixBase<T, N, std::index_sequence<Idx...>>
{
public:
	constexpr static size_t Dimension = N;

private:
	template <size_t N> struct Type_size_t { using Type = size_t; };

public:
	MatrixBase(typename Type_size_t<Idx>::Type... size)
		: _sizes{ size... }, _data(_get_size()) {}

	T& operator()(typename Type_size_t<Idx>::Type... id) {
		return _data[_get_index(id...)];
	}
	const T& operator()(typename Type_size_t<Idx>::Type... id) const {
		return _data[_get_index(id...)];
	}

	size_t size(size_t dim) const {
		assert(dim < Dimension);
		return _sizes[dim];
	}
	size_t capacity() const {
		return _data.size();
	}
	static constexpr size_t dimension() {
		return Dimension;
	}

private:
	size_t _sizes[N];
	dyarray<T> _data;

	// Basic functions :
	size_t _get_size() const {
		return std::accumulate(std::begin(_sizes), std::end(_sizes), size_t(1), std::multiplies<size_t>());
	}

	size_t _get_index(typename Type_size_t<Idx>::Type... id) {
		assert(_check_index(id...));
		return __sum(__get_id_mul<Idx>(id)...);
	}

	bool _check_index(typename Type_size_t<Idx>::Type... id) {
		size_t ids[Dimension] = { id... };
		return std::all_of(range(Dimension).begin(), range(Dimension).end(), [&](size_t i) { return ids[i] < _sizes[i]; });
	}

	// Using in _get_index :
	template <size_t RN> size_t __get_size_product() { return _sizes[RN + 1] * __get_size_product<RN + 1>(); }
	template <> size_t __get_size_product<Dimension - 1>() { return 1; }
	template <size_t N> size_t __get_id_mul(size_t id) { return id * __get_size_product<N>(); }
	template <typename... Args> size_t __sum(size_t i, Args... args) { return i + __sum(args...); }
	size_t __sum(size_t i) { return i; }
};

template <typename T, size_t N>
using Matrix = MatrixBase<T, N, std::make_index_sequence<N>>;

PRILIB_END

#endif
