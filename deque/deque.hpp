#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu { 
	const size_t lim = 200;
template<class T>
class deque {

	struct Node{
		T *num;
		Node *lst, *nxt;
		Node() {
			num = NULL;
			lst = nxt = NULL;
		}
		Node(T x) {
			num = new T(x);
			lst = nxt = NULL;
		}
		~Node() { delete num; }
	};

	struct Block{
		Node *l, *r;
		Block *lst, *nxt;
		size_t sz;
		Block(Node *l = NULL, Node *r = NULL) : l(l), r(r) {
			lst = nxt = NULL;
			sz = 0;
		}
	};

	Block *bg, *ed;
	size_t sz;

public:
	class const_iterator;
	class iterator {
	private:
		Node *node;
		Block *block;
		deque *bel;
		bool ok;
		
		void check() const {
			if(ok == 0) throw invalid_iterator();
		}

		int cnt() const{
			int rs = 1;
			for (Node *t = node; t != block->l; t = t->lst) ++rs;
			for (Block *t = block->lst; t->lst != NULL; t = t->lst) rs += t->sz;
			return rs;
		}

		iterator find(int x) const {
			for (Block *t = bel->bg->nxt; t != NULL; t = t->nxt) {
				if(t->sz >= x || t->nxt == NULL) {
					Node *p = t->l;
					for (int i = 1; i < x; ++i) p = p->nxt; 
					return iterator(p, t, bel);
				}
				x -= t->sz;
			}
		}
	
	friend class deque;

	public:

		iterator(Node *nd = NULL, Block *bl = NULL, deque *x = NULL) : node(nd), block(bl), bel(x){ ok = 1; }
		iterator(const const_iterator &other) {
			node = other.node; block = other.block; bel = const_cast<deque*>(other.bel); ok = other.ok;
		}
		iterator(const iterator &other) {
			node = other.node; block = other.block; bel = other.bel; ok = other.ok;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			if(ok == 0) return *this;
			int now = cnt();
			if(now + n > bel->sz + 1 || now + n <= 0) {
				iterator t = *this;
				t.ok = 0;
				return t;
			} 
			return find(now + n);
		}
		iterator operator-(const int &n) const {
			return this->operator+(-n);
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			this->check(); rhs.check();
			if(bel != rhs.bel) throw invalid_iterator();
			return cnt() - rhs.cnt();
		}
		iterator& operator+=(const int &n) {
			if(ok == 0) return *this;
			int now = cnt();	
			if(now + n > bel->sz + 1 || now + n <= 0) {
				this->ok = 0;
				return *this;
			} 
			iterator t = this->find(now + n);
			*this = t;
			return *this;
		}
		iterator& operator-=(const int &n) {
			this->operator+=(-n);
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			if(node == bel->ed->r) {
				ok = 0;
				return *this;
			}
			iterator rs = *this;
			if(node == block->r) block = block->nxt;
			node = node->nxt;
			return rs;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			if(ok == 0 || node == bel->ed->r) {
				ok = 0;
				return *this;
			}
			if(node == block->r) block = block->nxt;
			node = node->nxt;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			if(ok == 0 || node == bel->bg->l->nxt) {
				ok = 0;
				return *this;
			}
			iterator rs = *this;
			if(node == block->l) block = block->lst;
			node = node->lst;
			return rs;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			if(ok == 0 || node == bel->bg->l->nxt) {
				ok = 0;
				return *this;
			}
			if(node == block->l) block = block->lst;
			node = node->lst;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const {
			check();
			if(node == bel->ed->r) throw runtime_error();
			return *(node->num); 
		}
		/**
		 * TODO it->field
		 */
		T* operator->() const noexcept { 
			check();
			if(node == bel->ed->r) throw runtime_error();
			return node->num; 
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			check(); rhs.check();
			return node == rhs.node && block == rhs.block;
		}
		bool operator==(const const_iterator &rhs) const {
			check(); rhs.check();
			return node == rhs.node && block == rhs.block;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !operator==(rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !operator==(rhs);
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
	private:
		Node *node;
		Block *block;
		const deque *bel;
		bool ok;
		
		bool check() const{
			if(ok == 0) throw invalid_iterator();
		}
		
		int cnt() const{
			int rs = 1;
			for (Node *t = node; t != block->l; t = t->lst) ++rs;
			for (Block *t = block->lst; t->lst != NULL; t = t->lst) rs += t->sz;
			return rs;
		}

		const_iterator find(int x) const {
			for (Block *t = bel->bg->nxt; t != NULL; t = t->nxt) {
				if(t->sz >= x || t->nxt == NULL) {
					Node *p = t->l;
					for (int i = 1; i < x; ++i) p = p->nxt; 
					return const_iterator(p, t, bel);
				}
				x -= t->sz;
			}
		}

	friend class deque;

	public:
		const_iterator(Node *nd = NULL, Block *bl = NULL, const deque *x = NULL) : node(nd), block(bl), bel(x){ ok = 1; }
		const_iterator(const const_iterator &other) {
			node = other.node; block = other.block; bel = other.bel; ok = other.ok;
		}
		const_iterator(const iterator &other) {
			node = other.node; block = other.block; bel = other.bel; ok = other.ok;
		}
		const_iterator operator+(const int &n) const {
			if(ok == 0) return *this;
			int now = cnt();
			if(now + n > bel->sz + 1 || now + n <= 0) {
				iterator t = *this;
				t.ok = 0;
				return t;
			} 
			return find(now + n);
		}
		const_iterator operator-(const int &n) const {
			return this->operator+(-n);
		}
		int operator-(const const_iterator &rhs) const {
			this->check(); rhs.check();
			if(bel != rhs.bel) throw invalid_iterator();
			return cnt() - rhs.cnt();
		}
		int operator-(const iterator &rhs) const {
			this->check(); rhs.check();
			if(bel != rhs.bel) throw invalid_iterator();
			return cnt() - rhs.cnt();
		}
		const_iterator& operator+=(const int &n) {
			if(ok == 0) return *this;
			int now = cnt();	
			if(now + n > bel->sz + 1 || now + n <= 0) {
				this->ok = 0;
				return *this;
			} 
			iterator t = this->find(now + n);
			*this = t;
			return *this;
		}
		const_iterator& operator-=(const int &n) {
			this->operator+=(-n);
			return *this;
		}
		const_iterator operator++(int) {
			if(node == bel->ed->r) {
				ok = 0;
				return *this;
			}
			iterator rs = *this;
			if(node == block->r) block = block->nxt;
			node = node->nxt;
			return rs;
		}
		const_iterator& operator++() {
			if(ok == 0 || node == bel->ed->r) {
				ok = 0;
				return *this;
			}
			if(node == block->r) block = block->nxt;
			node = node->nxt;
			return *this;
		}
		const_iterator operator--(int) {
			if(ok == 0 || node == bel->bg->l->nxt) {
				ok = 0;
				return *this;
			}
			iterator rs = *this;
			if(node == block->l) block = block->lst;
			node = node->lst;
			return rs;
		}
		const_iterator& operator--() {
			if(ok == 0 || node == bel->bg->l->nxt) {
				ok = 0;
				return *this;
			}
			if(node == block->l) block = block->lst;
			node = node->lst;
			return *this;
		}
		const T& operator*() const { 
			check();
			if(node == bel->ed->r) throw runtime_error();
			return *(node->num); 
		}
		const T* operator->() const noexcept {
			check();
			if(node == bel->ed->r) throw runtime_error();
			return node->num; 
		}
		bool operator==(const iterator &rhs) const {
			check(); rhs.check();
			return node == rhs.node && block == rhs.block;
		}
		bool operator==(const const_iterator &rhs) const {
			check(); rhs.check();
			return node == rhs.node && block == rhs.block;
		}
		bool operator!=(const iterator &rhs) const {
			return !operator==(rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !operator==(rhs);
		}
	};

private:
	
	void build() {
		bg = new Block; ed = new Block;
		bg->l = bg->r = new Node;
		ed->l = ed->r = new Node;
		bg->l->nxt = ed->l;
		ed->l->lst = bg->l;
		bg->nxt = new Block; ed->lst = bg->nxt;
		bg->nxt->lst = bg; ed->lst->nxt = ed;
		sz = 0;	
	}

	void copy(const deque &other) {
		//int cnt = 0;
		for(Node *t = other.bg->l->nxt; t->nxt != NULL; t = t->nxt) {
			//std::cerr << ++cnt << "\n";
			this->push_back(*(t->num));
		}
	}
	
	Block *merge(Block *A, Block *B) {
		A->r = B->r;
		A->sz += B->sz;
		A->nxt = B->nxt;
		A->nxt->lst = A;
		delete B;
		return A;
	}
	
	Block *split(Block *A) {
		Block *B = new Block(A->l, A->r);
		B->nxt = A->nxt; 
		B->nxt->lst = B;
		A->nxt = B; B->lst = A;
		size_t L = A->sz / 2, R = A->sz - L;
		Node *t = A->l;
		for (int i = 1; i < L; ++i) t = t->nxt;
		A->r = t; B->l = t->nxt;
		A->sz = L; B->sz = R;
		return B;
	}

public:

	/**
	 * TODO Constructors
	 */
	deque() {
		build();
	}
	deque(const deque &other) {
		build();
		copy(other);
	}
	/**
	 * TODO Deconstructor
	 */
	~deque() {
		for (Node *t = bg->l, *p; t != NULL; t = p) {
			p = t->nxt;
			delete t;
		}
		for (Block *t = bg, *p; t != NULL; t = p) {
			p = t->nxt;
			delete t;
		}
		sz = 0;
	}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {
		if(this == &other) return *this;
		this->~deque();
		build();
		copy(other);
		return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
		if(pos >= sz || pos < 0) throw index_out_of_bound();
		size_t x = pos + 1;
		for (Block *t = bg->nxt; t != ed; t = t->nxt) {
			if(t->sz >= x) {
				Node *p = t->l;
				for (int i = 1; i < x; ++i) p = p->nxt;
				return *(p->num);
			}
			x -= t->sz;
		}
	}
	const T & at(const size_t &pos) const {
		if(pos >= sz || pos < 0) throw index_out_of_bound();
		size_t x = pos + 1;
		for (Block *t = bg->nxt; t != ed; t = t->nxt) {
			if(t->sz >= x) {
				Node *p = t->l;
				for (int i = 1; i < x; ++i) p = p->nxt;
				return *(p->num);
			}
			x -= t->sz;
		}
	}
	T & operator[](const size_t &pos) { return at(pos); }
	const T & operator[](const size_t &pos) const {	return at(pos); }
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
		if(sz == 0) throw container_is_empty();
		return *(bg->l->nxt->num);
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
		if(sz == 0) throw container_is_empty();
		return *(ed->l->lst->num);
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() { 
		if(sz) return iterator(bg->l->nxt, bg->nxt, this);
		else return iterator(ed->l, ed, this);
	}
	const_iterator cbegin() const {
		if(sz) return const_iterator(bg->l->nxt, bg->nxt, this);
		else return const_iterator(ed->l, ed, this); 
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() { return iterator(ed->r, ed, this); }
	const_iterator cend() const { return const_iterator(ed->r, ed, this);}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const { return sz == 0; }
	/**
	 * returns the number of elements
	 */
	size_t size() const { return sz; }
	/**
	 * clears the contents
	 */
	void clear() { 
		this->~deque();
		build();
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
		pos.check();
		if(pos.bel != this) throw runtime_error();
		Block *A = pos.block;
		Node *x = new Node(value);
		if(A == ed) {
			A = A->lst;
			A->r = x;
			if(A->l == NULL) A->l = x;
		}
		else if(A->l == pos.node) A->l = x;
		x->lst = pos.node->lst; x->lst->nxt = x;
		x->nxt = pos.node; pos.node->lst = x;
		++sz;
		if((++A->sz) > 2 *lim) {
			Block *B = split(A);
			Node *t = x;
			while((t != A->l) && (t != B->l)) t = t->lst;
			if(t == A->l) return iterator(x, A, this);
			else return iterator(x, B, this);
		}
		else return iterator(x, A, this);
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
		pos.check();
		if(pos.bel != this) throw runtime_error();
		if(pos.node == ed->r) throw runtime_error();
		if(sz == 0) throw container_is_empty();
		Block *A = pos.block, *B = A->nxt, *C = A->lst;
		Node *rs = pos.node->nxt;
		pos.node->nxt->lst = pos.node->lst;
		pos.node->lst->nxt = pos.node->nxt;
		pos.ok = 0;
		--A->sz; --sz;
		if((A->sz == 0) && ((A->lst != bg) || (A->nxt != ed))) {
			C->nxt = B; B->lst = C;
			delete A; delete pos.node;
			return iterator(rs, B, this);
		}
		if(A->sz == 0) A->l = A->r = NULL;
		if(A->l == pos.node) A->l = pos.node->nxt;
		if(A->r == pos.node) A->r = pos.node->lst;
		delete pos.node;
		
		if((B != ed) && A->sz + B->sz <= lim) merge(A, B);
		else if((C != bg) && A->sz + C->sz <= lim) A = merge(C, A); 
		B = A->nxt;
		Node *t = rs;
		while((t != A->l) && (t != B->l)) t = t->lst;
		if(t == A->l) return iterator(rs, A, this);
		else return iterator(rs, B, this);
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) { insert(iterator(ed->l, ed, this), value); }
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
		if(sz == 0) throw container_is_empty();
		erase(iterator(ed->l->lst, ed->lst, this));
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) { 
		if(sz) insert(iterator(bg->l->nxt, bg->nxt, this), value);
		else insert(iterator(ed->l, ed, this), value);
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
		if(sz == 0) throw container_is_empty();
		erase(iterator(bg->l->nxt, bg->nxt, this));
	}
};

}

#endif
