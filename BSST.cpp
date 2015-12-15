#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/* AVL node inspired by http://rosettacode.org/wiki/AVL_tree#C.2B.2B */
template <class T>
class AVLnode {
public:
	T key;
	int balance;
	AVLnode *left, *right, *parent;

	AVLnode(T k, AVLnode *p) : key(k), balance(0), parent(p),
	left(NULL), right(NULL) {}

	~AVLnode() {
		delete left;
		delete right;
	}
};

/* AVL tree */
template <class T>
class AVLtree {
public:
	AVLtree(void);
	~AVLtree(void);
	bool insert(T key);
	void deleteKey(const T key);
	void printBalance();
	bool verify(T key);

private:
	AVLnode<T> *root;

	AVLnode<T>* rotateLeft          ( AVLnode<T> *a );
	AVLnode<T>* rotateRight         ( AVLnode<T> *a );
	AVLnode<T>* rotateLeftThenRight ( AVLnode<T> *n );
	AVLnode<T>* rotateRightThenLeft ( AVLnode<T> *n );
	void rebalance                  ( AVLnode<T> *n );
	int height                      ( AVLnode<T> *n );
	void setBalance                 ( AVLnode<T> *n );
	void printBalance               ( AVLnode<T> *n );
	void clearNode                  ( AVLnode<T> *n );
};

/* AVL class definition */
template <class T>
void AVLtree<T>::rebalance(AVLnode<T> *n) {
	setBalance(n);

	if (n->balance == -2) {
		if (height(n->left->left) >= height(n->left->right))
		n = rotateRight(n);
		else
		n = rotateLeftThenRight(n);
	}
	else if (n->balance == 2) {
		if (height(n->right->right) >= height(n->right->left))
		n = rotateLeft(n);
		else
		n = rotateRightThenLeft(n);
	}

	if (n->parent != NULL) {
		rebalance(n->parent);
	}
	else {
		root = n;
	}
}

template <class T>
AVLnode<T>* AVLtree<T>::rotateLeft(AVLnode<T> *a) {
	AVLnode<T> *b = a->right;
	b->parent = a->parent;
	a->right = b->left;

	if (a->right != NULL)
	a->right->parent = a;

	b->left = a;
	a->parent = b;

	if (b->parent != NULL) {
		if (b->parent->right == a) {
			b->parent->right = b;
		}
		else {
			b->parent->left = b;
		}
	}

	setBalance(a);
	setBalance(b);
	return b;
}

template <class T>
AVLnode<T>* AVLtree<T>::rotateRight(AVLnode<T> *a) {
	AVLnode<T> *b = a->left;
	b->parent = a->parent;
	a->left = b->right;

	if (a->left != NULL)
	a->left->parent = a;

	b->right = a;
	a->parent = b;

	if (b->parent != NULL) {
		if (b->parent->right == a) {
			b->parent->right = b;
		}
		else {
			b->parent->left = b;
		}
	}

	setBalance(a);
	setBalance(b);
	return b;
}

template <class T>
AVLnode<T>* AVLtree<T>::rotateLeftThenRight(AVLnode<T> *n) {
	n->left = rotateLeft(n->left);
	return rotateRight(n);
}

template <class T>
AVLnode<T>* AVLtree<T>::rotateRightThenLeft(AVLnode<T> *n) {
	n->right = rotateRight(n->right);
	return rotateLeft(n);
}

template <class T>
int AVLtree<T>::height(AVLnode<T> *n) {
	if (n == NULL)
	return -1;
	return 1 + max(height(n->left), height(n->right));
}

template <class T>
void AVLtree<T>::setBalance(AVLnode<T> *n) {
	n->balance = height(n->right) - height(n->left);
}

template <class T>
void AVLtree<T>::printBalance(AVLnode<T> *n) {
	if (n != NULL) {
		printBalance(n->left);
		cout << n->balance << " ";
		printBalance(n->right);
	}
}

template <class T>
AVLtree<T>::AVLtree(void) : root(NULL) {}

template <class T>
AVLtree<T>::~AVLtree(void) {
	delete root;
}

template <class T>
bool AVLtree<T>::insert(T key) {
	if (root == NULL) {
		root = new AVLnode<T>(key, NULL);
	}
	else {
		AVLnode<T>
		*n = root,
		*parent;

		while (true) {
			if (n->key == key)
			return false;

			parent = n;

			bool goLeft = n->key > key;
			n = goLeft ? n->left : n->right;

			if (n == NULL) {
				if (goLeft) {
					parent->left = new AVLnode<T>(key, parent);
				}
				else {
					parent->right = new AVLnode<T>(key, parent);
				}

				rebalance(parent);
				break;
			}
		}
	}

	return true;
}

template <class T>
void AVLtree<T>::deleteKey(const T delKey) {
	if (root == NULL)
	return;

	AVLnode<T>
	*n       = root,
	*parent  = root,
	*delNode = NULL,
	*child   = root;

	while (child != NULL) {
		parent = n;
		n = child;
		child = delKey >= n->key ? n->right : n->left;
		if (delKey == n->key)
		delNode = n;
	}

	if (delNode != NULL) {
		delNode->key = n->key;

		child = n->left != NULL ? n->left : n->right;

		if (root->key == delKey) {
			root = child;
		}
		else {
			if (parent->left == n) {
				parent->left = child;
			}
			else {
				parent->right = child;
			}

			rebalance(parent);
		}
	}
}

template <class T>
void AVLtree<T>::printBalance() {
	printBalance(root);
	cout << endl;
}



template <class T>
bool AVLtree<T>::verify(T searchkey) {
	if (root == NULL) return false;

	AVLnode<T>
	*n       = root,
	*parent  = root,
	*delNode = NULL,
	*child   = root;

	
	while (child != NULL) {
		parent = n;
		n = child;
		child = searchkey >= n->key ? n->right : n->left;
		if (searchkey == n->key)	return true;
	}
	return false;
}


int main(void)
{
    bool debug = true;
	AVLtree<string> t;

	cout << "Inserting dictionary.txt" << endl;
    
    string temp = "";
    string line;
    ifstream dict; //opens it
    dict.open("./dictionary.txt");
    if (dict.is_open()){
        if (debug) cout << "Dictionary open." << endl;
        while (getline(dict, line)){
            if (line == "ma") break;
        }
        t.insert("ma");
        while (getline(dict, line)){
            t.insert(line);
            if (debug) cout << "Inserted " << line << endl;
        }
        dict.close();
    }
    dict.open("./dictionary.txt");
    if (dict.is_open()){
        while (getline(dict, line)){
            if (line == "ma") break;
            t.insert(line);
            if (debug) cout << "Inserted " << line << endl;
        }
    }
    
    string userword;
    cout << "Verifying: " << endl;
	if (debug) { // 7 test words below, two should fail
	    string testWords[] = {"applesauce", "banana", "castrate", "whiskey", "marsupial", "odo", "hansolo"}; 
	    for (int i = 0; i < 7; i++){
    	    cout << "... " << testWords[i];
    	    if (t.verify(testWords[i])) cout << " is in the dictionary." << endl;
    	    else cout << " is not in the dictionary. I may have eaten it. I wuz hungry." << endl;
    	}
	}
	else {
	    cout << "What word would you like me to test (all lower-case please)?" << endl;
	    cin >> userword;
	    if (t.verify(userword)) cout << " is in the dictionary." << endl;
	    else cout << " is not in the dictionary. I may have eaten it. I wuz hungry." << endl;
	}
}