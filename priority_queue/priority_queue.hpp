#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
	struct Node{
		T num;
		Node *son[2], *fa;
		int len;
		Node(const T &x):num(x){
			son[0] = son[1] = fa = NULL;
			len = 1;//
		}
		Node const &operator= (const Node &other) {
			num = other.num;
			len = other.len;//
			son[0] = son[1] = fa = NULL;
			return *this;
		}
	};
	
	size_t sz;
	Node *root;

	Node* copy(Node *f, Node *x) {
		if(x == NULL) return NULL;
		Node *pos = new Node(x->num);
		*pos = *x;
		pos->fa = f;
		pos->son[0] = copy(pos, x->son[0]);
		pos->son[1] = copy(pos, x->son[1]);
		return pos;
	}
	
	void clear(Node *pos) {
		if(pos == NULL) return;
		clear(pos->son[0]);
		clear(pos->son[1]);
		delete pos;
	}

	Node *add(Node *x, Node *y) {
		if(x == NULL) return y;
		if(y == NULL) return x;
		if(Compare()(x->num,y->num)) std::swap(x, y);
		//x->num >= y->num
		x->son[1] = add(x->son[1], y);
		x->son[1]->fa = x;
		if(x->son[0] == NULL || x->son[1]->len > x->son[0]->len) 
			std::swap(x->son[1], x->son[0]);
		if(x->son[1]) x->len = x->son[1]->len + 1;
		else x->len = 0;
		return x;
	}

public:
	/**
	 * TODO constructors
	 */
	priority_queue() {
		root = NULL; sz = 0;
	}
	priority_queue(const priority_queue &other) {
		sz = other.sz;
		root = copy(NULL, other.root);
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue() {
		clear(root);
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) {
		if(this == &other) return *this;
		clear(root);
		sz = other.sz;
		root = copy(NULL, other.root);
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
		if(empty()) throw container_is_empty();
		return root->num;
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
		++sz;
		Node *p = new Node(e);
		root = add(root, p);
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		if(empty()) throw container_is_empty();
		--sz;
		Node *x = root->son[0], *y = root->son[1];
		delete root;
		if(x == NULL && y == NULL) root = NULL;
		else root = add(x, y);
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
		return sz;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
		return sz == 0;
	}
	/**
	 * return a merged priority_queue with at least O(logn) complexity.
	 */
	void merge(priority_queue &other) {
		Node *x = root, *y = other.root;
		if(other.sz == 0) return;
		size_t nowsz = sz + other.sz;
		root = add(x, y);
		other.sz = 0; other.root = NULL;
		sz = nowsz;
	}
};

}

#endif
