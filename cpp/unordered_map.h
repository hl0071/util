//
// Created by hujianzhe on 18-7-24
//

#ifndef UTIL_CPP_UNORDERED_MAP_H
#define	UTIL_CPP_UNORDERED_MAP_H

#include "cpp_compiler_define.h"
#if __CPP_VERSION >= 2011
#include <unordered_map>
#else
#include "../c/datastruct/hashtable.h"
#include <stddef.h>
#include <utility>
#include <string>
namespace std {
template <typename K, typename V>
class unordered_map {
public:
	typedef K			key_type;
	typedef pair<K, V>	value_type;
	typedef struct Xnode : public hashtable_node_t {
		value_type v;

		Xnode(void) {
			key = (void*)&v.first;
		}
	} Xnode;

	class iterator {
	friend class unordered_map;
	public:
		iterator(hashtable_node_t* p = NULL) : x(p) {}
		iterator(const iterator& i) : x(i.x) {}
		iterator& operator=(const iterator& i) {
			x = i.x;
			return *this;
		}

		bool operator==(const iterator& i) const {
			return i.x == x;
		}

		bool operator!=(const iterator& i) const {
			return !operator==(i);
		}

		value_type* operator->(void) const {
			return &((Xnode*)x)->v;
		}
		value_type& operator*(void) const {
			return ((Xnode*)x)->v;
		}
		iterator& operator++(void) {
			x = hashtable_next_node(x);
			return *this;
		}
		iterator operator++(int unused) {
			iterator it = *this;
			x = hashtable_next_node(x);
			return it;
		}

	private:
		hashtable_node_t* x;
	};
	typedef iterator	const_iterator;

private:
	static int keycmp(hashtable_node_t* _n, void* key) {
		return ((Xnode*)_n)->v.first != *(key_type*)key;
	}

	static unsigned int __key_hash(const std::string& s) {
		const char* str = s.empty() ? "" : s.c_str();
		/* BKDR hash */
		unsigned int seed = 131;
		unsigned int hash = 0;
		while (*str) {
			hash = hash * seed + (*str++);
		}
		return hash & 0x7fffffff;
	}
	static unsigned int __key_hash(size_t n) { return n; }
	static unsigned int __key_hash(const void* p) { return (unsigned int)(size_t)p; }

	static unsigned int keyhash(void* key) {
		return __key_hash(*(key_type*)key);
	}

public:
	unordered_map(void) :
		m_size(0)
	{
		hashtable_init(&m_table, m_buckets, sizeof(m_buckets) / sizeof(m_buckets[0]), keycmp, keyhash);
	}

	~unordered_map(void) { clear(); }

	void clear(void) {
		hashtable_node_t *cur, *next;
		for (cur = hashtable_first_node(&m_table); cur; cur = next) {
			next = hashtable_next_node(cur);
			delete ((Xnode*)cur);
		}
		hashtable_init(&m_table, m_buckets, sizeof(m_buckets) / sizeof(m_buckets[0]), keycmp, keyhash);
		m_size = 0;
	}

	size_t size(void) const { return m_size; };
	bool empty(void) const { return hashtable_first_node((hashtable_t*)&m_table) == NULL; }

	V& operator[](const key_type& k) {
		hashtable_node_t* n = hashtable_search_key(&m_table, (void*)&k);
		if (n) {
			return ((Xnode*)n)->v.second;
		}
		Xnode* xnode = new Xnode();
		xnode->v.first = k;
		hashtable_insert_node(&m_table, xnode);
		++m_size;
		return xnode->v.second;
	}
	
	void erase(iterator iter) {
		--m_size;
		hashtable_remove_node(&m_table, iter.x);
		delete (Xnode*)(iter.x);
	}

	size_t erase(const key_type& k) {
		hashtable_node_t* node = hashtable_search_key(&m_table, (void*)&k);
		if (node) {
			hashtable_remove_node(&m_table, node);
			delete (Xnode*)node;
			--m_size;
			return 1;
		}
		return 0;
	}

	iterator find(const key_type& k) const {
		hashtable_node_t* node = hashtable_search_key((hashtable_t*)&m_table, (void*)&k);
		return iterator(node);
	}

	pair<iterator, bool> insert(const value_type& vt) {
		hashtable_node_t* n = hashtable_search_key(&m_table, (void*)&vt.first);
		if (n) {
			return pair<iterator, bool>(iterator(n), false);
		}
		Xnode* xnode = new Xnode();
		xnode->v.first = vt.first;
		hashtable_insert_node(&m_table, xnode);
		++m_size;
		return pair<iterator, bool>(iterator(xnode), true);
	}

	iterator begin(void) const {
		return iterator(hashtable_first_node((hashtable_t*)&m_table));
	}
	iterator end(void) const {
		return iterator();
	}

private:
	hashtable_t m_table;
	hashtable_node_t* m_buckets[11];
	size_t m_size;
};
}
#endif

#endif
