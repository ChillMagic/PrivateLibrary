// hashstringpool.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_HASHSTRINGPOOL_H_
#define _PRILIB_HASHSTRINGPOOL_H_
#include "macro.h"
#include "stringview.h"
#include "explicittype.h"
#include <cassert>
#include <map>
#include <functional>

PRILIB_BEGIN
class HashStringPool
{
public:
	DefineExplicitTypeWithValue(HashID, size_t, 0);

	class HashIDGetter {
	public:
		explicit HashIDGetter(const HashID &hashId, const HashStringPool &hashStringPool)
				: hashId(hashId), hashStringPool(hashStringPool) {}
		const std::string& operator()() {
			return hashStringPool.get(hashId);
		}

	private:
		const HashID &hashId;
		const HashStringPool &hashStringPool;
	};

public:
	explicit HashStringPool() = default;

	const std::string& get(const HashID &id) const {
		return _data.at(id.data);
	}

	bool has(const HashID &id) const {
		return _data.find(id.data) != _data.end();
	}

	HashIDGetter getIDGetter(const HashID &id) const {
		return HashIDGetter(id, *this);
	}

	HashID getID(const std::string &value) const {
		return HashID(_getHash(value));
	}

	HashID insert(const std::string &value) {
		size_t id = getID(value).data;
		if (_data.find(id) == _data.end()) {
			_data.insert({ id, value });
		}
		else {
			assert(_data.at(id) == value);
		}
		return HashID(id);
	}

	HashID insert(const StringViewRange &value) {
		return insert(value.toString());
	}

private:
	std::hash<std::string> _getHash;
	std::map<size_t, std::string> _data;
};

inline bool operator<(const HashStringPool::HashID &lhs, const HashStringPool::HashID &rhs) {
	return lhs.data < rhs.data;
}
PRILIB_END

#endif
