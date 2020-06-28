/*
Reference: https://en.wikipedia.org/wiki/AVL_tree
*/

#ifndef MY_AVL_MAP
#define MY_AVL_MAP

#include <iostream>
#include <cstddef>
#include <utility>
#include <stack>
#include <algorithm>

//tmp
#include <queue>

using namespace std;

template<class K, class V>
class MyAVLMap {
	// Description of the node
	class Node {
	private:
		Node *parent;
		Node *l_child;
		Node *r_child;

		int height;

		friend class MyAVLMap;

	public:
		pair<K,V> value;

		Node() {}
		Node(K k, V v) {
			value = make_pair(k,v);
			height = 1;
			parent = nullptr;
			l_child = nullptr;
			r_child = nullptr;
		}

		Node(K k, V v, Node *p) {
			value = make_pair(k,v);
			height = 1;
			parent = p;
			l_child = nullptr;
			r_child = nullptr;
		}

		Node(pair<K,V> v) {
			value = v;
			height = 1;
			parent = nullptr;
			l_child = nullptr;
			r_child = nullptr;
		}

		Node(pair<K,V> v, Node* p) {
			value = v;
			height = 1;
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

		// If positive, its right side heavier. If negative, its left side heavier. 0 = balanced
		int balance_factor() {
			int l_child_height = l_child!=nullptr ? l_child->height : 0;
			int r_child_height = r_child!=nullptr ? r_child->height : 0;
		
			return r_child_height-l_child_height;
		}
	};

	// Variables for the AVL
	Node* root;
	int _size;

	// right child changes place with parent
	void right_rotate(Node* n) {
		Node* child = n->r_child;
		Node* hanging_node = child->l_child;
		Node* p = n->parent;

		// cout << "node: " << n->value.first << "\tr_child: " << child->value.first << endl;

		n->r_child = hanging_node;
		if (p!=nullptr) {
			if (p->l_child == n)
				p->l_child = child;
			else
				p->r_child = child;
		}
		else root = child;
		n->parent = child;

		if (hanging_node != nullptr)
			hanging_node->parent = n;

		child->l_child = n;
		child->parent = p;

		// Update height
		n->height = max((n->l_child != nullptr ? n->l_child->height : 0), (hanging_node != nullptr ? hanging_node->height : 0)) + 1;
		child->height = max(n->height, (child->r_child != nullptr ? child->r_child->height : 0)) + 1;
	}

	// left child changes place with parent
	void left_rotate(Node* n) {
		Node* child = n->l_child;
		Node* hanging_node = child->r_child;
		Node* p = n->parent;

		n->l_child = hanging_node;
		if (p!=nullptr) {
			if (p->l_child == n)
				p->l_child = child;
			else
				p->r_child = child;
		}
		else root = child;
		n->parent = child;

		if (hanging_node != nullptr)
			hanging_node->parent = n;

		child->r_child = n;
		child->parent = p;

		// Update height
		n->height = max((n->r_child != nullptr ? n->r_child->height : 0), (hanging_node != nullptr ? hanging_node->height : 0)) + 1;
		child->height = max(n->height, (child->l_child != nullptr ? child->l_child->height : 0)) + 1;
	}
	

	// Private functions for the AVL
	void rebalancing(Node *n) {
		// cout << size() << "rebalancing with factor: " << n->balance_factor() << "\tnode: " << n->value.first << endl;
		int b = n->balance_factor();

		if (b==2) {
			// cout << "b==2" << endl;
			// right right
			if (n->r_child->balance_factor() >= 0 ) {
				// cout << "right rotate" << endl;
				right_rotate(n);
			}
			// right left
			else {
				// cout << "rightleft rotate" << endl;
				left_rotate(n->r_child);
				right_rotate(n);
			}
		}
		else if (b==-2) {
			// cout << "b==-2" << endl;
			// left left
			if (n->l_child->balance_factor() <= 0) {
				// cout << "left rotate" << endl;
				left_rotate(n);
			}
			// left right
			else {
				// cout << "leftright rotate" << endl;
				right_rotate(n->l_child);
				left_rotate(n);
			}
		}

	}

public:
	class iterator {
	public:
		Node* it;

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

		pair<K,V>& operator*() const {
			return it->value;
		}

		bool operator!= (const iterator &b) {
			return it!=b.it;
		}

		bool operator== (const iterator &b) {
			return it==b.it;
		}
	};

	// Empty constructor
	MyAVLMap() {
		root = nullptr;
		_size = 0;
	}

	// Destructor
	~MyAVLMap() {
		clear();
	}

	// If is empty or not
	bool empty() {
		return _size==0;
	}

	// Return the size
	int size() {
		return _size;
	}

	iterator begin() {
		return iterator(root->lower_bound());
	}

	iterator end() {
		return iterator(nullptr);
	}

	// Insert new element
	void insert(pair<K,V> new_value) {
		if (root == nullptr) {
			_size++;
			root = new Node(new_value);
			return;
		}

		// for iteration
		stack<Node*> path;

		Node *current = root;
		Node *parent;

		while (current!=nullptr) {
			// NO DUPLICATES
			if (current->value.first == new_value.first) return;

			// UPDATE PARENT
			parent = current;
			path.push(parent);

			// UPDATE CURRENT
			if (new_value.first < current->value.first) current = current->l_child;
			else current = current->r_child;
		}

		if (new_value.first < parent->value.first) parent->l_child = new Node(new_value, parent);
		else parent->r_child = new Node(new_value, parent);
		_size++;


		// Update heights and balance if needed
		while(path.size()) {
			parent = path.top();
			path.pop();
			parent->height = max((parent->l_child != nullptr ? parent->l_child->height : 0), (parent->r_child != nullptr ? parent->r_child->height : 0)) + 1;
			rebalancing(parent);
		}

		// while(root->parent != nullptr)
		// 	root = root->parent;
	}

	void erase() {

	}

	void clear() {

	}

	iterator find(K k) {
		// Node *current_node = root;
		// Node *next_node;

		// _TAIL_RECURSION_FIND:
		// // if found return reference
		// if (current_node->value.first==k) return iterator(current_node);

		// next_node = k<current_node->value.first ? current_node->l_child : current_node->r_child;

		// if (next_node == nullptr) return iterator(nullptr);
		// current_node = next_node;
		// goto _TAIL_RECURSION_FIND;

		/////////////////

		Node *current = root;

		while (current!=nullptr) {
			if (current->value.first == k) return iterator(current);
			else if (k<current->value.first) current = current->l_child;
			else current = current->r_child;
		}

		return iterator(nullptr);
	}

	iterator lower_bound() {

	}

	iterator upper_bound() {

	}

// Operations overload
	V &operator [] (K k) {
		iterator it = find(k);

		//Not found
		if(it.it==nullptr) {
			insert(make_pair(k,V()));
			it = find(k);
		}

		return (*it).second;
	}

	void recursive_depth_search(Node *n, unsigned &d, unsigned c=0){
		// cout << "node: " << n->value.first << "\theight: " << n->height << endl;
		// if (n->l_child == nullptr) cout << "left nullptr" << endl;
		// if (n->r_child == nullptr) cout << "right nullptr" << endl;
		c += 1;
		// update max depth
		if (d<c) d=c;

		if (n->l_child != nullptr) recursive_depth_search(n->l_child, d, c);
		if (n->r_child != nullptr) recursive_depth_search(n->r_child, d, c);
		// cout << "c: " << c << "\theight: " << n->height << endl; 

	}

	unsigned int depth() {
		unsigned d=0;

		recursive_depth_search(root, d);
		return d;
	}

	void heights() {
		queue<Node*> a, b;
		queue<Node*> *c, *d, *tmp;

		a.push(root);
		c = &a;
		d = &b;

		while(a.size() || b.size()) {
			while(c->size()) {
				Node* n = c->front();
				c->pop();
				cout << n->height << "  ";
				if (n->l_child != nullptr) d->push(n->l_child);
				if (n->r_child != nullptr) d->push(n->r_child);
			}

			tmp = c;
			c = d;
			d = tmp;

			cout << endl;
		}
	}

	unsigned long long size_of() {
		unsigned long long mem_size = 0;

		mem_size += sizeof(*this);
		mem_size += sizeof(Node)*_size;

		return mem_size;
	}
};

#endif

// #include <map>
// #include <cstdlib>
// #include <ctime>
// #include <vector>

// int main() {
// 	MyAVLMap<int,int> a;
// 	map<int,int> b;
// 	vector<int> keys;

// 	srand(time(0));

// 	int n = 100;

// 	for (int i=0, c; i<n; i++) {
// 		c = rand()%1000;
// 		keys.push_back(c);
// 		a.insert(make_pair(c, c+100));
// 		b.insert(make_pair(c, c+100));
// 	}

// 	for (auto &i : keys)
// 		if (a[i] != b[i]) cout << "ERROR!!!" << endl;

// 	cout << "=========================\ndepth:" << endl;
// 	cout << a.depth() << endl;
// 	cout << a.size() << "\t" << b.size() << endl << endl << endl;

// 	a.heights();

// 	a[1234] = 3432;
// 	cout << a[1234] << endl;

// 	int key_not_found = 45978;
// 	if (a.find(key_not_found) == a.end())
// 		cout << key_not_found << " not found." << endl;


// 	return 0;
// }