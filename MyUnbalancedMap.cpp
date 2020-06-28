#ifndef MY_UNBALANCED_MAP
#define MY_UNBALANCED_MAP

#include <utility>

using namespace std;

template <class K, class T>
class MyUnbalancedMap {
public:

	class Node {
		public:
		pair<K, T> value;
		Node *parent;
		Node *l_child;
		Node *r_child;
	
		Node () {
			value = nullptr;
			parent = nullptr;
			l_child = nullptr;
			r_child = nullptr;
		}

		Node (K k, T v) {
			value = make_pair(k,v);
			parent = nullptr;
			l_child = nullptr;
			r_child = nullptr;
		}

		Node (K k, T v, Node *p) {
			value = make_pair(k,v);
			parent = p;
			l_child = nullptr;
			r_child = nullptr;
		}

		Node (pair<K,T> v) {
			value = v;
			// value.second = v.second;
			parent = nullptr;
			l_child = nullptr;
			r_child = nullptr;
		}

		Node (pair<K,T> v, Node *p) {
			value = v;
			parent = p;
			l_child = nullptr;
			r_child = nullptr;
		}

		Node *lower_bound() {
			// If the current node is the minimum, return self
			if (l_child == nullptr) return this;

			Node *lower, *l;
			l=l_child;

			while (l!=nullptr) {
				lower=l;
				l = l->l_child;
			}

			return lower;
		}

		Node *upper_bound() {
			// If the current node is the maximum, return self
			if (r_child == nullptr) return this;

			Node *upper, *r;
			r = r_child;

			do {
				upper = r;
				r = r->r_child;
			} while (r!=nullptr);

			return upper;
		}
	};

	// For iteration and return purpouse
	class iterator {
	public:
		Node *it;

		iterator() {
			it = nullptr;
		}

		iterator(Node *i) {
			it = i;
		}

		// prefix operator ++
		iterator operator++ () {
			if (it->r_child != nullptr) {
				it = it->r_child->lower_bound();
				return *this;	
			}
			else {
				Node *parent = it->parent;
				while (parent!=nullptr) {
					// If its right child
					if (parent->r_child == it) {
						it = parent;
						parent = parent->parent;
					}
					// If its left child
					else {
						it = parent;
						return *this;
					}
				}

				it = nullptr;
				return *this;
			} 
		}

		// postfix operator ++
		iterator operator++ ( int ) {
			iterator r(it);
				if (it->r_child != nullptr) {
				it = it->r_child->lower_bound();
				return r;	
			}
			else {
				Node *parent = it->parent;
				while (parent!=nullptr) {
					// If its right child
					if (parent->r_child == it) {
						it = parent;
						parent = parent->parent;
					}
					// If its left child
					else {
						it = parent;
						return r;
					}
				}

				it = nullptr;
				return r;
			} 
		}

		// ITERATORS ARE NOT MEANT TO GO BACKWARDS, UNLESS YOU CREATE A REVERSE_ITERATOR
		// // prefix operator --
		// iterator operator-- () {
		// 	it = it->l_child;
		// 	return iterator(it);
		// }

		// // postfix operator --
		// iterator operator-- ( int ) {
		// 	iterator r(it);
		// 	it = it->l_child;
		// 	return r;
		// }

		pair<K,T>& operator*() const {
			return it->value;
		}

		bool operator!= (const iterator &b) {
			return it!=b.it;
		}

		bool operator== (const iterator &b) {
			return it==b.it;
		}
	};

private:
	Node *root;
	unsigned int _size;

public:

	MyUnbalancedMap() {
		root = nullptr;
		_size = 0;
	}

	~MyUnbalancedMap() {
		// clear();
	}

	// Insert function
	void insert(pair<K,T> new_value) {
		if (root == nullptr) {
			_size++;
			root = new Node(new_value);
			return;
		}

		Node *current = root;
		Node *parent;

		while (current!=nullptr) {
			// NO DUPLICATES
			if (current->value.first == new_value.first) return;
			
			// UPDATE PARENT
			parent = current;

			// UPDATE CURRENT
			if (new_value.first < current->value.first) current = current->l_child;
			else current = current->r_child;
		}

		if (new_value.first < parent->value.first) parent->l_child = new Node(new_value, parent);
		else parent->r_child = new Node(new_value, parent);
		_size++;
		// (*current) = new Node<T>(new_value);
	}

	void insert(K new_k, T new_v) {
		insert(make_pair(new_k, new_v));
	}

	// Find function
	iterator find(K key) {
		Node *current = root;

		while (current!=nullptr) {
			if (current->value.first == key) return iterator(current);
			else if (key<current->value.first) current = current->l_child;
			else current = current->r_child;
		}

		return iterator(nullptr);
	}

	// Size function
	unsigned int size() {
		return _size;
	}

	// Returns the minimum key
	iterator min() {
		if (root == nullptr) return iterator(root);
		return iterator(root->lower_bound());
	}

	// Returns the maximum key
	iterator max() {
		if (root == nullptr) return iterator(root);
		return iterator(root->upper_bound());
	}

	iterator end() {
		return iterator(nullptr);
	}

	iterator begin() {
		return iterator(root->lower_bound());
	}

	// Overloading of operator [] for accessing data
	T &operator[] (K i) {
		iterator found = find(i);
		return found.it->value.second;
	}

	void recursive_depth_search(Node *n, unsigned &d, unsigned c=0){
		c += 1;
		// update max depth
		if (d<c) d=c;

		if (n->l_child != nullptr) recursive_depth_search(n->l_child, d, c);
		if (n->r_child != nullptr) recursive_depth_search(n->r_child, d, c);
	}

	unsigned int depth() {
		unsigned d=0, c=0;
		

		recursive_depth_search(root, d);
		return d;
	}

	unsigned long long size_of() {
		unsigned long long mem_size = 0;

		mem_size += sizeof(*this);
		mem_size += sizeof(Node)*_size;

		return mem_size;
	}

};

#endif


///////////////// TESTING AREA /////////////////////

// #include <iostream>
// #include <map>
// #include <vector>
// #include <string>
// #include <ctime>
// #include <cstdlib>

// int main() {
// 	MyUnbalancedMap<int, int> m1;
// 	map<int,int> m2;
// 	// srand(time(0));

// 	int n=100, r;

// 	for (int i=0; i<n; i++) {
// 		r = rand()%n;
// 		m1.insert(i, r+200);
// 		m2.insert(make_pair(i, r+200));
// 	}

// 	for (auto it=m2.begin(); it!=m2.end(); it++) {
// 		cout << it->first << "\t" << it->second << endl;
// 	}

// 	cout << endl;

// 	for (auto &it : m1) {
// 		cout << it.first << "\t" << it.second << endl;
// 	}

// 	cout << m1.depth() << endl;

// 	return 0;
// }
