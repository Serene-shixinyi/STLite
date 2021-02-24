/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"


//#include<iostream> //////////////

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {

	enum colT{RED, BLACK};

	struct Node{
		pair<const Key, T> *num;
		Node *son[2], *fa;
		colT col;
		Node() {
			num = NULL; son[0] = son[1] = fa = NULL; 
			col = RED;
		}
		Node(const pair<const Key, T> &x, Node *ls = NULL, Node *rs = NULL, Node *fa = NULL, colT c = RED): fa(fa), col(c) {
			son[0] = ls; son[1] = rs;
			num = new pair<const Key, T>(x);
		}
		~Node() {
			if(num) delete num;
		}
		/*
		Node(const Node &b) {
			std::cerr<<"Node copy construct................\n";
		}
		Node &operator= (const Node& b) {
			std::cerr<<"Node operator = ...................\n";
		}
		*/
		
	};

	Node *root, *nil;
	size_t sz;

public:

	typedef pair<const Key, T> value_type;
	
	class const_iterator;
	class iterator {
	private:
		
		map *bel;
		Node *p;

		friend class map;

	public:
		iterator(map *bel = NULL, Node *p = NULL):bel(bel), p(p) {}
		iterator(const iterator &other) {
			bel = other.bel; p = other.p;
		}
		
		iterator operator++(int) {
			if(p == NULL) throw runtime_error();
			iterator rs = *this;
			if(*this == bel->end()) {p = NULL; throw index_out_of_bound();}
			Node *pos = p, *lst = p;
			while(pos != bel->nil && (pos->son[1] == bel->nil || pos->son[1] == lst)) {
				lst = pos; pos = pos->fa;
				if(pos->son[1] != lst) {
					p = pos;
					return rs;
				}
			}
			if(pos == bel->nil) p = pos;
			else {
				pos = pos->son[1];
				while(pos->son[0] != bel->nil) pos = pos->son[0];
				p = pos;
			}
			return rs;
		}
		
		iterator & operator++() {
			if(p == NULL) throw runtime_error();
			if(*this == bel->end()) {p = NULL; throw index_out_of_bound();}
			Node *pos = p, *lst = p;
			while(pos != bel->nil && (pos->son[1] == bel->nil || pos->son[1] == lst)) {
				lst = pos; pos = pos->fa;
				if(pos->son[1] != lst) {
					p = pos;
					return *this;
				}
			}
			if(pos == bel->nil) p = pos;
			else {
				pos = pos->son[1];
				while(pos->son[0] != bel->nil) pos = pos->son[0];
				p = pos;
			}
			return *this;
		}
		
		iterator operator--(int) {
			if(p == NULL) throw runtime_error();
			iterator rs = *this;
			if(*this == bel->begin()) {p = NULL; throw index_out_of_bound();}
			Node *pos = p, *lst = p;
			while(pos != bel->nil && (pos->son[0] == bel->nil || pos->son[0] == lst)) {
				lst = pos; pos = pos->fa;
				if(pos->son[0] != lst) {
					p = pos;
					return rs;
				}
			}
			pos = pos->son[0];
			while(pos->son[1] != bel->nil) pos = pos->son[1];
			p = pos;
			return rs;
		}
		
		iterator & operator--() {
			if(p == NULL) throw runtime_error();
			if(*this == bel->begin()) {p = NULL; throw index_out_of_bound();}
			Node *pos = p, *lst = p;
			while(pos != bel->nil && (pos->son[0] == bel->nil || pos->son[0] == lst)) {
				lst = pos; pos = pos->fa;
				if(pos->son[0] != lst) {
					p = pos;
					return *this;
				}
			}
			pos = pos->son[0];
			while(pos->son[1] != bel->nil) pos = pos->son[1];
			p = pos;
			return *this;
		}
		
		value_type & operator*() const {
			if(p == NULL || *this == bel->end()) throw index_out_of_bound();
			return *(p->num);
		}
		bool operator==(const iterator &rhs) const {
			//if(p == NULL || rhs.p == NULL) throw runtime_error();
			return this->bel == rhs.bel && this->p == rhs.p;
		}
		bool operator==(const const_iterator &rhs) const {
			//if(p == NULL || rhs.p == NULL) throw runtime_error();
			return this->bel == rhs.bel && this->p == rhs.p;
		}
		
		bool operator!=(const iterator &rhs) const {
			return !operator==(rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !operator==(rhs);
		}

		value_type* operator->() const noexcept {
			return p->num;
		}
	};
	class const_iterator {
	private:
		
		const map *bel;
		Node *p;
		
		friend class map;
	public:
		const_iterator(const map *bel = NULL, Node *p = NULL):bel(bel), p(p) {}
		const_iterator(const const_iterator &other) {
			bel = other.bel; p = other.p;
		}
		const_iterator(const iterator &other){
			bel = other.bel; p = other.p;
		}

		const_iterator operator++(int) {
			if(p == NULL) throw runtime_error();
			const_iterator rs = *this;
			if(*this == bel->cend()) {p = NULL; throw index_out_of_bound();}
			Node *pos = p, *lst = p;
			while(pos != bel->nil && (pos->son[1] == bel->nil || pos->son[1] == lst)) {
				lst = pos; pos = pos->fa;
				if(pos->son[1] != lst) {
					p = pos;
					return rs;
				}
			}
			if(pos == bel->nil) p = pos;
			else {
				pos = pos->son[1];
				while(pos->son[0] != bel->nil) pos = pos->son[0];
				p = pos;
			}
			return rs;
		}

		const_iterator & operator++() {
			if(p == NULL) throw runtime_error();
			if(*this == bel->cend()) {p = NULL; throw index_out_of_bound();}
			Node *pos = p, *lst = p;
			while(pos != bel->nil && (pos->son[1] == bel->nil || pos->son[1] == lst)) {
				lst = pos; pos = pos->fa;
				if(pos->son[1] != lst) {
					p = pos;
					return *this;
				}
			}
			if(pos == bel->nil) p = pos;
			else {
				pos = pos->son[1];
				while(pos->son[0] != bel->nil) pos = pos->son[0];
				p = pos;
			}
			return *this;
		}

		const_iterator operator--(int) {
			if(p == NULL) throw runtime_error();
			const_iterator rs = *this;
			if(*this == bel->cbegin()) {p = NULL; throw index_out_of_bound();}
			Node *pos = p, *lst = p;
			while(pos != bel->nil && (pos->son[0] == bel->nil || pos->son[0] == lst)) {
				lst = pos; pos = pos->fa;
				if(pos->son[0] != lst) {
					p = pos;
					return rs;
				}
			}
			pos = pos->son[0];
			while(pos->son[1] != bel->nil) pos = pos->son[1];
			p = pos;
			return rs;
		}

		const_iterator & operator--() {
			if(p == NULL) throw runtime_error();
			if(*this == bel->cbegin()) {p = NULL; throw index_out_of_bound();}
			Node *pos = p, *lst = p;
			while(pos != bel->nil && (pos->son[0] == bel->nil || pos->son[0] == lst)) {
				lst = pos; pos = pos->fa;
				if(pos->son[0] != lst) {
					p = pos;
					return *this;
				}
			}
			pos = pos->son[0];
			while(pos->son[1] != bel->nil) pos = pos->son[1];
			p = pos;
			return *this;
		}

		value_type & operator*() const {
			if(p == NULL || *this == bel->cend()) throw index_out_of_bound();
			return *(p->num);
		}

		bool operator==(const iterator &rhs) const {
			//if(p == NULL || rhs.p == NULL) throw runtime_error();
			return this->bel == rhs.bel && this->p == rhs.p;
		}

		bool operator==(const const_iterator &rhs) const {
			//if(p == NULL || rhs.p == NULL) throw runtime_error();
			return this->bel == rhs.bel && this->p == rhs.p;
		}

		bool operator!=(const iterator &rhs) const {
			return !operator==(rhs);
		}

		bool operator!=(const const_iterator &rhs) const {
			return !operator==(rhs);
		}

		value_type* operator->() const noexcept {
			return p->num;
		}
		/*
		void debug() {
			std::cerr<<"const_iterator: "<< p << " " << ok << "\n";
		}
		*/
	};

private:

	Node *copy(Node *f, Node *t) {
		if(t->num == NULL) return nil;
		Node *pos = new Node(*(t->num), nil, nil, f, t->col);
		pos->son[0] = copy(pos, t->son[0]);
		pos->son[1] = copy(pos, t->son[1]);
		return pos;
	}

	void get_empty(Node *pos) {
		if(pos == nil) return;
		get_empty(pos->son[0]);
		get_empty(pos->son[1]);
		delete pos;
	}

	void Rotate(Node *g, int o) {//o = 0: left son become father
		Node *gg = g->fa, *p = g->son[o];
		g->son[o] = p->son[o ^ 1]; if(p->son[o ^ 1] != nil) p->son[o ^ 1]->fa = g;
		p->son[o ^ 1] = g; g->fa = p;
		p->fa = gg;
		if(gg != nil) {
			if(gg->son[0] == g) gg->son[0] = p;
			else gg->son[1] = p;
		}
		else nil->son[0] = root = p;
		// colT tmp= p->col; p->col = g->col; g->col = tmp;
	}

	bool key_cmp(const value_type *a, const Key *key) const{ //a > b
		if(a == NULL) return 1;
		return Compare()(*key, a->first);
	}
	
	bool xd(const value_type *a, const Key *key) const{
		if(a == NULL) return 0;
		return !(Compare()(*key, a->first) || Compare()(a->first, *key));
	}

	void transplant(Node *y, Node *x) {
		if(y->fa == nil) root = nil->son[0] = x;
		else {
			int o = (y->fa->son[1] == y);
			y->fa->son[o] = x;
		}
		x->fa = y->fa;
	}

	void fixup(Node *x) {
		Node *p, *t;
		while(x != root && x->col == BLACK) {
			p = x->fa;
			int o = (p->son[1] == x);
			t = p->son[o ^ 1];
			if(t->col == RED) {
				t->col = BLACK; 
				p->col = RED;
				Rotate(p, o ^ 1);
				t = p->son[o ^ 1];
			}
			if(t->son[0]->col == BLACK && t->son[1]->col == BLACK) {
				t->col = RED;
				x = p; //if p->col == RED: next step will break then p->col = BLACK
			}
			else {
				if(t->son[o ^ 1]->col == BLACK) {
					t->son[o]->col = BLACK;
					t->col = RED;
					Rotate(t, o);
					t = p->son[o ^ 1];
				}
				t->col = p->col;
				p->col = BLACK;
				t->son[o ^ 1]->col = BLACK;
				Rotate(p, o ^ 1);
				x = root;
			}
		}
		x->col = BLACK;
	}

public:
	map() {
		root = nil = new Node; nil->col = BLACK;
		nil->fa = nil->son[0] = nil->son[1] = nil;
		sz = 0;
	}
	map(const map &other) {
		root = nil = new Node; nil->col = BLACK;
		nil->fa = nil->son[0] = nil->son[1] = nil;
		if(other.sz > 0) 
			nil->son[0] = root = copy(nil, other.root);
		sz = other.sz;
	}
	map & operator=(const map &other) {
		if(&other == this) return *this;
		this->~map();
		root = nil = new Node; nil->col = BLACK;
		nil->fa = nil->son[0] = nil->son[1] = nil;
		if(other.sz > 0) 
			nil->son[0] = root = copy(nil, other.root);
		sz = other.sz;
		return *this;
	}
	~map() {
		get_empty(root);
		delete nil; root = nil = NULL;
		sz = 0;
	}
	
	T & at(const Key &key) {
		iterator rs = find(key);
		if(rs == end()) throw index_out_of_bound();
		return rs.p->num->second;
	}
	const T & at(const Key &key) const {
		const_iterator rs = find(key);
		if(rs == cend()) throw index_out_of_bound();
		return rs.p->num->second;
	}
	
	T & operator[](const Key &key) {
		Node *pos = find(key).p;
		if(pos == nil) return insert(value_type(key, T())).first.p->num->second;
		return pos->num->second;
	}
	
	const T & operator[](const Key &key) const {
		Node *pos = find(key).p;
		if(pos == nil) throw index_out_of_bound();
		return pos->num->second;
	}
	
	iterator begin() {
		Node *t = root;
		while(t->son[0] != nil) t = t->son[0];
		return iterator(this, t);
	}
	const_iterator cbegin() const {
		Node *t = root;
		while(t->son[0] != nil) t = t->son[0];
		return const_iterator(this, t);
	}
	
	iterator end() {
		return iterator(this, nil);
	}
	const_iterator cend() const {
		return const_iterator(this, nil);
	}
	
	bool empty() const { return sz == 0; }
	
	size_t size() const { return sz; }
	
	void clear() { 
		this->~map();
		root = nil = new Node; nil->col = BLACK;
		nil->fa = nil->son[0] = nil->son[1] = nil;
		sz = 0;
	}

	pair<iterator, bool> insert(const value_type &value) {
		iterator tmp = find(value.first);
		if(tmp != end()) return pair<iterator, bool> (tmp, false);
		Node *pos = root, *p = nil; ++sz; 
		while(pos != nil) {
			p = pos;
			pos = key_cmp(pos->num, &value.first)? pos->son[0] : pos->son[1];
		}
		Node *t = new Node(value, nil, nil, p);
		if(p == nil) root = nil->son[0] = t;
		else {
			if(key_cmp(p->num, &value.first)) p->son[0] = t; 
			else p->son[1] = t;
		}
		Node *x = t;
		while(x->fa->col == RED) {
			p = x->fa;
			int o = (p == p->fa->son[1]);
			Node *y = p->fa->son[o ^ 1];
			if(y->col == RED) {
				p->col = y->col = BLACK;
				p->fa->col = RED;
				x = p->fa;
			}
			else {
				if(x == p->son[o ^ 1]) {
					x = p; 
					Rotate(x, o ^ 1);
					p = x->fa;
				}
				p->col = BLACK;
				p->fa->col = RED;
				Rotate(p->fa, o);
			}
		}
		root->col = BLACK; nil->son[0] = root;
		return pair<iterator, bool> (iterator(this, t), true);
	}
	
	void erase(iterator pos) {
		if(pos.p == NULL || (pos.bel != this)) throw runtime_error();
		if(pos == end()) throw index_out_of_bound();
		--sz;
		
		Node *x = pos.p, *y = x, *z;
		colT tmp = y->col;
		if((x->son[0] != nil) && (x->son[1] != nil)) {
			y = (++iterator(this, x)).p;
			tmp = y->col; z = y->son[1];
			if(y->fa == x) z->fa = y; //z may = nil
			else {
				transplant(y, z);
				y->son[1] = x->son[1];
				y->son[1]->fa = y;
			}
			transplant(x, y);
			y->son[0] = x->son[0];
			y->son[0]->fa = y;
			y->col = x->col;
		}
		else {
			if(x->son[0] != nil) z = x->son[0];
			else z = x->son[1]; //z may = nill
			transplant(x, z);
		}
		delete x;
		if(tmp == BLACK) fixup(z);

		pos.p = NULL; nil->son[0] = root;
	}
	
	size_t count(const Key &key) const {
		if(find(key) != cend()) return 1;
		return 0;
	}
	
	iterator find(const Key &key) {
		Node *pos = root;
		while(pos != nil && (!xd(pos->num, &key))) {
			pos = key_cmp(pos->num, &key)? pos->son[0] : pos->son[1];
		}
		return iterator(this, pos);
	}
	const_iterator find(const Key &key) const {
		Node *pos = root;
		while(pos != nil && (!xd(pos->num, &key))) {
			pos = key_cmp(pos->num, &key)? pos->son[0] : pos->son[1];
		}
		return const_iterator(this, pos);
	}
	 /*
	void dfs(Node *pos) const {
		if(pos == nil) return;
		std::cerr << pos << " " << pos->fa << " " << pos->col << "    ";
		std::cerr << "(" << pos->son[0] << " " << pos->son[1] << ")  "; 
		if(pos->num) std::cerr << pos->num->first ;
		std::cerr << "\n";
		dfs(pos->son[0]);
		dfs(pos->son[1]);
	}

	void debug() const{
		//return;
		std::cerr<<"debug:\n";
		dfs(root);
	}
	 */
};

}

#endif