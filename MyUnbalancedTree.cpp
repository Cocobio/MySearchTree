#ifndef MY_UNBALANCED_TREE
#define MY_UNBALANCED_TREE

#include <cstddef>

template <class T>
class MyUnbalancedSet {
public:
	// the basic Node function. Without parrent
	class Node {
	public:
		T key;
		Node *parent;
		Node *l_child;
		Node *r_child;
	
		Node () {
			key = 0;
			parent = NULL;
			l_child = NULL;
			r_child = NULL;
		}

		Node (T v) {
			key = v;
			parent = NULL;
			l_child = NULL;
			r_child = NULL;
		}

		Node (T v, Node *p) {
			key = v;
			parent = p;
			l_child = NULL;
			r_child = NULL;
		}

		Node *lower_bound() {
			// If the current node is the minimum, return self
			if (l_child == NULL) return this;

			Node *lower, *l;
			l=l_child;

			while (l!=NULL) {
				lower=l;
				l = l->l_child;
			}

			return lower;
		}

		Node *upper_bound() {
			// If the current node is the maximum, return self
			if (r_child == NULL) return this;

			Node *upper, *r;
			r = r_child;

			do {
				upper = r;
				r = r->r_child;
			} while (r!=NULL);

			return upper;
		}
	};

private:
	int _size;
	Node *root;


public:
	// For iteration and return purpouse
	class iterator {
	public:
		Node *it;

		iterator() {
			it = NULL;
		}

		iterator(Node *i) {
			it = i;
		}

		// prefix operator ++
		iterator operator++ () {
			if (it->r_child != NULL) {
				it = it->r_child->lower_bound();
				return *this;	
			}
			else {
				Node *parent = it->parent;
				while (parent!=NULL) {
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

				it = NULL;
				return *this;
			} 
		}

		// postfix operator ++
		iterator operator++ ( int ) {
			iterator r(it);
				if (it->r_child != NULL) {
				it = it->r_child->lower_bound();
				return r;	
			}
			else {
				Node *parent = it->parent;
				while (parent!=NULL) {
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

				it = NULL;
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

		T& operator*() const {
			return it->key;
		}

		bool operator!= (const iterator &b) {
			return it!=b.it;
		}
	};

	// Constructor and Destructor
	MyUnbalancedBST() {
		_size = 0;
		root = NULL;
	}

	~MyUnbalancedBST() {
		clear();
	}

	// Cleaning function. So many new Nodes :(
	void clear() {

	}

	// Size function
	int size() {
		return _size;
	}

	// Returns the minimum key
	iterator lower_bound() {
		if (root == NULL) return iterator(root);
		return iterator(root->lower_bound());
	}

	// Returns the maximum key
	iterator upper_bound() {
		if (root == NULL) return iterator(root);
		return iterator(root->upper_bound());
	}

	// Insert function
	void insert(T new_value) {
		if (root == NULL) {
			_size++;
			root = new Node(new_value);
			return;
		}

		Node *current = root;
		Node *parent;

		while (current!=NULL) {
			// NO DUPLICATES
			if (current->key == new_value) return;
			
			// UPDATE PARENT
			parent = current;

			// UPDATE CURRENT
			if (new_value < current->key) current = current->l_child;
			else current = current->r_child;
		}

		if (new_value < parent->key) parent->l_child = new Node(new_value, parent);
		else parent->r_child = new Node(new_value, parent);
		_size++;
		// (*current) = new Node(new_value);
	}

	iterator end() {
		return iterator(NULL);
	}

	iterator begin() {
		return iterator(root->lower_bound());
	}

	// Find function
	iterator find(T key) {
		Node *current = root;

		while (current!=NULL) {
			// std::cout << "i: " << i++ << std::endl;
			if (current->key == key) return iterator(current);
			else if (key<current->key) current = current->l_child;
			else current = current->r_child;
		}

		return iterator(NULL);

	}

	// Overloading of operator [] for accessing data
	iterator operator [] (T i) {
		return find(i);
	}
};

#endif

// #include <cstdlib>
// #include <ctime>
// #include <vector>
// #include <iostream>

// using namespace std;

// int main() {
// 	MyUnbalancedBST<int> arbol;
// 	vector<int> v;

// 	int r;
// 	int n = 30;

// 	srand(time(0));
// 	for (int i=0; i<n; i++) {
// 		r = rand()%100;
// 		arbol.insert(r);
// 		v.push_back(r);

// 		cout << r << endl;
// 	}

// 	cout << "Arbol: " << arbol.size() << "\tVec: " << v.size() <<  endl;

// 	cout << "Values found: " << endl;

// 	for (int i=0; i<v.size(); i++) {
// 		cout << v[i] << "\t" << arbol[v[i]].it->key << endl;
// 	}

// 	cout << "Values not found: " << endl;

// 	for (int i=0; i<v.size(); i++) {
// 		if (arbol[v[i]+100] != arbol.end())
// 			cout << v[i] << "\t" << arbol[v[i]+100].it->key << endl;
// 		else cout << v[i]+100 << " not found." << endl;
// 	}

// 	cout << "Searching random values: " << endl;
// 	int rand_n = 30;

// 	for (int i=0, seed; i<rand_n; i++) {
// 		seed = rand()%100;

// 		if (arbol[seed] != arbol.end())
// 			cout << seed << "\t" << arbol[seed].it->key << endl;
// 		else cout << seed << " not found." << endl;
// 	}

// 	cout << "Lower and upper:\n" << arbol.lower_bound().it->key << endl << arbol.upper_bound().it->key << endl;

// 	cout << "Sorted: " << endl;

// 	for (auto it=arbol.begin(); it!=arbol.end(); it++)
// 		cout << it.it->key << endl;

// 	return 0;
// }