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
//#include <cassert>
//#include <iostream>
using namespace std;

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	struct Node{
		value_type *val;
		Node *ch[2], *fa;
		int col;
		~Node(){if(val != nullptr) delete val;}
		Node(value_type *_val = nullptr, int _col = 1) : val(_val), col(_col){
			ch[0] = ch[1] = fa = nullptr;
		}
		Node & operator = (Node const &other){
			if(&other == this) return *this;
			if(val != nullptr) delete val;
			if(other.val == nullptr)val = nullptr;
			else val = new value_type(*other.val);
			col = other.col;
			ch[0] = ch[1] = fa = nullptr;
			return *this;
		}
	}*root, *null;
	size_t tot;
	Compare compare;
	Node * newNode(value_type *_val = nullptr, int _col = 1){
		Node *p = new Node(_val, _col);
		p -> ch[0] = p -> ch[1] = p -> fa = null;
		return p;
	}
	/**
	 * see Bidirectional Iterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		Node *p;
		map *container;
		iterator(Node *_p = nullptr, map *_c = nullptr) : p(_p), container(_c) {}
		iterator(const iterator &other) : p(other.p), container(other.container) {}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			if(p == nullptr) throw invalid_iterator();
			iterator it = *this;
			if(p == container -> null){p = nullptr; throw invalid_iterator();}
			if(p -> ch[1] == container -> null){
				while(p == p -> fa -> ch[1])p = p -> fa;
				p = p -> fa;
			}
			else{
				p = p -> ch[1];
				while(p -> ch[0] != container -> null) p = p -> ch[0];
			}
			return it;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if(p == nullptr) throw invalid_iterator();
			if(p == container -> null){p = nullptr; throw invalid_iterator();}
			if(p -> ch[1] == container -> null){
				while(p == p -> fa -> ch[1])p = p -> fa;
				p = p -> fa;
			}
			else{
				p = p -> ch[1];
				while(p -> ch[0] != container -> null) p = p -> ch[0];
			}
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			if(p == nullptr) throw invalid_iterator();
			iterator it = *this;
			if(p -> ch[0] == container -> null){
				while(p != container -> null && p == p -> fa -> ch[0])p = p -> fa;
				if(p -> fa == container -> null){p = nullptr; throw invalid_iterator();}
				else p = p -> fa;
			}
			else{
				p = p -> ch[0];
				while(p -> ch[1] != container -> null) p = p -> ch[1];
			}
			return it;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if(p == nullptr) throw invalid_iterator();
			if(p -> ch[0] == container -> null){
				while(p != container -> null && p == p -> fa -> ch[0])p = p -> fa;
				if(p -> fa == container -> null){p = nullptr; throw invalid_iterator();}
				else p = p -> fa;
			}
			else{
				p = p -> ch[0];
				while(p -> ch[1] != container -> null) p = p -> ch[1];
			}
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			if(p == container -> null || p == nullptr) throw invalid_iterator();
			return *(p -> val);
		}
		bool operator==(const iterator &rhs) const {return p == rhs.p;}
		bool operator==(const const_iterator &rhs) const {return p == rhs.p;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return p != rhs.p;}
		bool operator!=(const const_iterator &rhs) const {return p != rhs.p;}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			if(p == container -> null || p == nullptr) throw invalid_iterator();
			return p -> val;
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
	private:
			// data members.
	public:
		Node *p;
		const map *container;
		const_iterator(Node *_p = nullptr, const map *_c = nullptr) : p(_p), container(_c) {}
		const_iterator(const const_iterator &other) : p(other.p), container(other.container) {}
		const_iterator(const iterator &other) : p(other.p), container(other.container){}
		// And other methods in iterator.
		// And other methods in iterator.
		// And other methods in iterator.
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			if(p == nullptr) throw invalid_iterator();
			const_iterator it = *this;
			if(p == container -> null){p = nullptr; throw invalid_iterator();}
			if(p -> ch[1] == container -> null){
				while(p == p -> fa -> ch[1])p = p -> fa;
				p = p -> fa;
			}
			else{
				p = p -> ch[1];
				while(p -> ch[0] != container -> null) p = p -> ch[0];
			}
			return it;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator & operator++() {
			if(p == nullptr) throw invalid_iterator();
			if(p == container -> null){p = nullptr; throw invalid_iterator();}
			if(p -> ch[1] == container -> null){
				while(p == p -> fa -> ch[1])p = p -> fa;
				p = p -> fa;
			}
			else{
				p = p -> ch[1];
				while(p -> ch[0] != container -> null) p = p -> ch[0];
			}
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			if(p == nullptr) throw invalid_iterator();
			const_iterator it = *this;
			if(p -> ch[0] == container -> null){
				while(p != container -> null && p == p -> fa -> ch[0])p = p -> fa;
				if(p == container -> null) {p = nullptr; throw invalid_iterator();}
				else p = p -> fa;
			}
			else{
				p = p -> ch[0];
				while(p -> ch[1] != container -> null) p = p -> ch[1];
			}
			return it;
		}
		/**
		 * TODO --iter
		 */
		const_iterator & operator--() {
			if(p == nullptr) throw invalid_iterator();
			if(p -> ch[0] == container -> null){
				while(p != container -> null && p == p -> fa -> ch[0])p = p -> fa;
				if(p == container -> null) {p = nullptr; throw invalid_iterator();}
				else p = p -> fa;
			}
			else{
				p = p -> ch[0];
				while(p -> ch[1] != container -> null) p = p -> ch[1];
			}
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			if(p == container -> null || p == nullptr) throw invalid_iterator();
			return *(p -> val);
		}
		bool operator==(const iterator &rhs) const {return p == rhs.p;}
		bool operator==(const const_iterator &rhs) const {return p == rhs.p;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return p != rhs.p;}
		bool operator!=(const const_iterator &rhs) const {return p != rhs.p;}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			if(p == container -> null || p == nullptr) throw invalid_iterator();
			return p -> val;
		}
	};
	/*void print(Node *p){
		if(p == null) return ;
		print(p -> ch[0]);
		printf("%d  %d  %d  %d  %d\n", p -> val -> first, p -> col, p -> fa == null ? -1 : p -> fa -> val -> first
		, p -> ch[0] == null ? -1 : p -> ch[0] -> val -> first, p -> ch[1] == null ? -1 : p -> ch[1] -> val -> first);
		print(p -> ch[1]);
		return ;
	}
	int chk(Node *p){
		if(p == null) return 1;
		int d0 = chk(p -> ch[0]), d1 = chk(p -> ch[1]);
		if(d0 != d1) printf("wrong answer on node %d\n", p -> val -> first);
		assert(d0 == d1);
		return d0 + p -> col;
	}*/ 
	Node * copy(Node *y, Node *z, Node *f){
		if(y == z) return null;
		Node *x = newNode();
		*x = *y;
		x -> fa = f;
		x -> ch[0] = copy(y -> ch[0], z, x);
		x -> ch[1] = copy(y -> ch[1], z, x);
		return x;
	}
	void del(Node *x){
		if(x == null) return ;
		del(x -> ch[0]);
		del(x -> ch[1]);
		delete x;
		return ;
	}
	/**
	 * TODO two constructors
	 */
	map() {
		null = newNode();
		null -> ch[0] = null -> ch[1] = null -> fa = null;
		root = null;
		tot = 0;
	}
	map(const map &other) {
		null = newNode();
		null -> ch[0] = null -> ch[1] = null -> fa = null;
		tot = other.tot;
		if(other.tot > 0){
			root = copy(other.root, other.null, null);
			null -> ch[0] = root;
		}
		else root = null;
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
		if(&other == this) return *this;
		del(root); delete null;
		null = newNode();
		null -> ch[0] = null -> ch[1] = null -> fa = null;
		tot = other.tot;
		if(other.tot > 0){
			root = copy(other.root, other.null, null);
			null -> ch[0] = root;
		}
		else root = null;
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() {
		if(root != null)del(root);
		delete null;
	}
	void rotate(Node *x, int d){
		Node *y = x -> ch[d];
		Node *z = y -> ch[d ^ 1];
		x -> ch[d] = z;
		if(z != null) z -> fa = x;
		y -> fa = x -> fa;
		if(x -> fa == null) root = y;
		else if(x == x -> fa -> ch[0])
			x -> fa -> ch[0] = y;
		else x -> fa -> ch[1] = y;
		y -> ch[d ^ 1] = x; x -> fa = y;
	}
	Node * _find(const Key &key) const {
		Node *p = root;
		while(p != null && (compare(p -> val -> first, key) || compare(key, p -> val -> first))){
			if(compare(key, p -> val -> first))
				p = p -> ch[0];
			else p = p -> ch[1];
		}
		return p;
	}
	void insert_fixup(Node *x){
		while(x -> fa -> col == 0){
			int d = (x -> fa == x -> fa -> fa -> ch[1]);
			Node *y = x -> fa -> fa -> ch[d ^ 1];
			if(y -> col == 0){
				x -> fa -> col = 1;
				x -> fa -> fa -> col = 0;
				y -> col = 1;
				x = x -> fa -> fa;
			}
			else{
				if(x == x -> fa -> ch[d ^ 1]){
					x = x -> fa;
					rotate(x, d ^ 1);
				}
				x -> fa -> col = 1;
				x -> fa -> fa -> col = 0;
				rotate(x -> fa -> fa, d);
			}
		}
		root -> col = 1;
	}
	Node *insert(const Key & key){
		Node *x = root, *y = null;
		while(x != null){
			y = x;
			if(compare(key, x -> val -> first))
				x = x -> ch[0];
			else x = x -> ch[1];
		}
		value_type *v = new value_type(key, T());
		Node *p = newNode(v, 0);
		p -> fa = y;
		if(y == null){root = p; null -> ch[0] = p;}
		else if(compare(key, y -> val -> first))
			y -> ch[0] = p;
		else y -> ch[1] = p;
		insert_fixup(p);
		null -> ch[0] = root;
		return p;
	}
	void transplant(Node *x, Node *y){
		if(x -> fa == null){root = y; null -> ch[0] = y;}
		else if(x == x -> fa -> ch[0]) x -> fa -> ch[0] = y;
		else x -> fa -> ch[1] = y;
		y -> fa = x -> fa;
	}
	Node * getmin(Node *x){
		Node *y = x;
		while(y -> ch[0] != null) y = y -> ch[0];
		return y;
	}
	void erase_fixup(Node *x){
		while(x != root && x -> col == 1){
			int d = (x == x -> fa -> ch[1]);
			Node *y = x -> fa -> ch[d ^ 1];
			if(y -> col == 0){
				y -> col = 1;
				x -> fa -> col = 0;
				rotate(x -> fa, d ^ 1);
				y = x -> fa -> ch[d ^ 1];
			}
			if(y -> ch[0] -> col == 1 && y -> ch[1] -> col == 1){
				y -> col = 0;
				x = x -> fa;
			}
			else{
				if(y -> ch[d ^ 1] -> col == 1){
					y -> ch[d] -> col = 1;
					y -> col = 0;
					rotate(y, d);
					y = x -> fa -> ch[d ^ 1];
				}
				y -> col = x -> fa -> col;
				x -> fa -> col = 1;
				y -> ch[d ^ 1] -> col = 1;
				rotate(x -> fa, d ^ 1);
				x = root;
			}
		}
		x -> col = 1;
	}
	void erase(Node *x){
		Node *y = x, *z;
		int ycol = y -> col;
		if(x -> ch[0] == null){
			z = x -> ch[1];
			transplant(x, x -> ch[1]);
		}
		else if(x -> ch[1] == null){
			z = x -> ch[0];
			transplant(x, x -> ch[0]);
		}
		else{
			y = getmin(x -> ch[1]);
			ycol = y -> col;
			z = y -> ch[1];
			if(y -> fa == x) z -> fa = y;
			else{
				transplant(y, y -> ch[1]);
				y -> ch[1] = x -> ch[1];
				y -> ch[1] -> fa = y;
			}
			transplant(x, y);
			y -> ch[0] = x -> ch[0];
			y -> ch[0] -> fa = y;
			y -> col = x -> col;
		}
		delete x;

		if(ycol == 1) erase_fixup(z);
		null -> ch[0] = root;
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		Node *p = _find(key);
		if(p == null) throw index_out_of_bound();
		return p -> val -> second;
	}
	const T & at(const Key &key) const {
		Node *p = _find(key);
		if(p == null) throw index_out_of_bound();
		return p -> val -> second;
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		Node *p = _find(key);
		if(p == null){
			tot++;
			return insert(key) -> val -> second;
		}
		return p -> val -> second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		Node *p = _find(key);
		if(p == null) throw index_out_of_bound();
		return p -> val -> second;
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		Node *p = root;
		while(p -> ch[0] != null) p = p -> ch[0];
		return iterator(p, this);
	}
	const_iterator cbegin() const {
		Node *p = root;
		while(p -> ch[0] != null) p = p -> ch[0];
		return const_iterator(p, this);
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {return iterator(null, this);}
	const_iterator cend() const {return const_iterator(null, this);}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {return !tot;}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {return tot;}
	/**
	 * clears the contents
	 */
	void clear() {
		if(root != null)del(root);
		delete null;
		null = newNode();
		null -> ch[0] = null -> ch[1] = null -> fa = null;
		root = null;
		tot = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		Key key = value.first;
		Node *p = _find(key);
		if(p != null) return pair<iterator, bool>(iterator(p, this), 0);
		tot++; p = insert(value.first);
		p -> val -> second = value.second;
		return pair<iterator, bool>(iterator(p, this), 1);
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		if(pos.p == null || pos.p == nullptr || pos.container != this) throw invalid_iterator();
		Node *p = _find(pos.p -> val -> first);
		if(p == null) throw index_out_of_bound();
		tot--; erase(p);
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {return (_find(key) == null) ? 0 : 1;}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {return iterator(_find(key), this);}
	const_iterator find(const Key &key) const {return const_iterator(_find(key), this);}
};

}

#endif
