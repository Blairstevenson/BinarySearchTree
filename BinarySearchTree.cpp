#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
using namespace std;

/* This is the node of our tree */
template <class T>
class TreeNode
{
private:
	T* value;
	int height;
	TreeNode<T>* left;
	TreeNode<T>* right;
public:
	TreeNode<T>(T*);
	TreeNode<T>* getLeft();
	void setLeft(TreeNode<T>*);
	TreeNode<T>* getRight();
	void setRight(TreeNode<T>*);
	T* getValue();
	void setValue(T*);
	int getHeight();
	void setHeight(int);
};

/* This is our tree */
template <class T>
class Tree
{
private:
	TreeNode<T>* root; // point to first item
	int count;
	// recursive add function
	TreeNode<T>* Add(TreeNode<T>*, T*);
	// recursive delete function
	TreeNode<T>* Delete(TreeNode<T>*, T);
	// helper functions
	void UpdateHeight(TreeNode<T>*); // update  height of a node after its children has been updated
	int HeightOf(TreeNode<T>*);
	int BalanceFactor(TreeNode<T>*);
	TreeNode<T>* MinNode(TreeNode<T>*);
	TreeNode<T>* RemoveMin(TreeNode<T>*);
	int IdeaHeight(); // calculate height of full binary tree with the same number of elements as our tree
	// rotation
	TreeNode<T>* RotateRight(TreeNode<T>*);
	TreeNode<T>* RotateLeft(TreeNode<T>*);
	// balancing
	TreeNode<T>* Balance(TreeNode<T>*);
	// printing
	void Print(TreeNode<T>*); // print recursive
public:
	Tree<T>();
	// functions to add data into the tree
	// please note that T must properly implement/override operator <
	// in order for these functions  to work
	void Add(T*);
	void Add(T);
	void Delete(T*);
	void Delete(T);
	int Height();
	T Maximum();
	T Minimum();
	// print the entire list, for testing purpose
	void Print();
};

/* Functions to test our AVL tree */
void TestATree()
{
	Tree<int> tree;
	int quit = 0;
	while (!quit)
	{
		cout << "Select operation:" << endl;
		cout << " 1. Add a value" << endl;
		cout << " 2. Delete a value" << endl;
		cout << " 3. Find maximum value" << endl;
		cout << " 4. Quit" << endl;
		char select = '0';
		while ('1' > select || select > '4')
		{
			select = getchar();
		}
		int value;
		switch (select)
		{
		case '1':
			cout << "Input value: ";
			cin >> value;
			tree.Add(value);
			tree.Print();
			break;
		case '2':
			if (tree.Height() < 0)
			{
				cout << "Tree has no value" << endl << endl;
			}
			else
			{
				cout << "Input value: ";
				cin >> value;
				tree.Delete(value);
				tree.Print();
			}
			break;
		case '3':
			if (tree.Height() < 0)
			{
				cout << "Tree has no value" << endl << endl;
			}
			else
			{
				cout << "Max value: " << tree.Maximum() << endl << endl;
			}
			break;
		default:
			quit = 1;
			break;
		}
	}
}

void TestAList()
{
	Tree<int> tree;
	int count;
	cout << "Number of elements: ";
	cin >> count;
	int* array = new int[count];
	for (int i = 0; i < count; i++)
	{
		array[i] = rand() % 100;
	}
	clock_t t1, t2;
	t1 = clock();
	// start counting time
	for (int i = 0; i < count; i++)
	{
		tree.Add(array[i]);
	}
	// finish counting time
	t2 = clock();
	float diff((float)t2 - (float)t1);
	cout << "Result tree: " << endl;
	tree.Print();
	cout << "Runtime: " << diff << " ms" << endl << endl;

	delete[] array;
}

int main()
{
	int quit = 0;
	while (!quit)
	{
		cout << "Select test mode:" << endl;
		cout << " 1. Play around with a tree" << endl;
		cout << " 2. Add a set of value into a tree and show execution time" << endl;
		cout << " 3. Quit" << endl;
		char select = '0';
		while ('1' > select || select > '3')
		{
			select = getchar();
		}
		int value;
		switch (select)
		{
		case '1':
			TestATree();
			break;
		case '2':
			TestAList();
			break;
		default:
			quit = 1;
			break;
		}
	}
	return 0;
}

/*********************************/
/* TreeNode function declaration */
/*********************************/
template <class T>
TreeNode<T>::TreeNode<T>(T* value)
{
	this->value = value;
	this->height = 0; // height of leaf is 0
	this->right = nullptr;
	this->left = nullptr;
}

template <class T>
TreeNode<T>* TreeNode<T>::getLeft()
{
	return this->left;
}

template <class T>
void TreeNode<T>::setLeft(TreeNode<T>* left)
{
	this->left = left;
}

template <class T>
TreeNode<T>* TreeNode<T>::getRight()
{
	return this->right;
}

template <class T>
void TreeNode<T>::setRight(TreeNode<T>* right)
{
	this->right = right;
}

template <class T>
T* TreeNode<T>::getValue()
{
	return this->value;
}

template <class T>
void TreeNode<T>::setValue(T* value)
{
	this->value = value;
}

template <class T>
int TreeNode<T>::getHeight()
{
	return this->height;
}

template <class T>
void TreeNode<T>::setHeight(int height)
{
	this->height = height;
}

/*********************************/
/* AVL Tree function declaration */
/*********************************/
template <class T>
Tree<T>::Tree<T>()
{
	// remember to initialize our pointer
	this->root = nullptr;
	this->count = 0;
}

/* Add */
template <class T>
void Tree<T>::Add(T* data)
{
	this->root = this->Add(this->root, data);
}

template <class T>
void Tree<T>::Add(T data)
{
	T* dataPtr = new T(data);
	this->Add(dataPtr);
}

// the recursive add function
template <class T>
TreeNode<T>* Tree<T>::Add(TreeNode<T>* node, T* value)
{
	if (node == nullptr)
	{
		// create new leaf
		this->count++; // count the number of item in the tree
		return new TreeNode<T>(value);
	}
	if (*value < *(node->getValue()))
	{
		// smaller value to the left
		node->setLeft(this->Add(node->getLeft(), value));
	}
	else
	{
		// bigger value to the right
		node->setRight(this->Add(node->getRight(), value));
	}
	return this->Balance(node);
}

/* Helper functions */
template <class T>
void Tree<T>::UpdateHeight(TreeNode<T>* node)
{
	// update height of a node given that height of its children are correct
	int hl = this->HeightOf(node->getLeft());
	int hr = this->HeightOf(node->getRight());
	node->setHeight((hl > hr ? hl : hr) + 1);
}

template <class T>
int Tree<T>::Height()
{
	return this->HeightOf(this->root);
}

template <class T>
int Tree<T>::HeightOf(TreeNode<T>* node)
{
	// height of null must be -1 in order to make height of leaf zero
	return node == nullptr ? -1 : node->getHeight();
}

template <class T>
int Tree<T>::BalanceFactor(TreeNode<T>* node)
{
	return this->HeightOf(node->getRight()) - this->HeightOf(node->getLeft());
}

template <class T>
TreeNode<T>* Tree<T>::MinNode(TreeNode<T>* node)
{
	// find the left most node
	while (node->getLeft() != nullptr)
	{
		node = node->getLeft();
	}
	return node;
}

template <class T>
TreeNode<T>* Tree<T>::RemoveMin(TreeNode<T>* node)
{
	// remove left most node and re-balance tree afterward
	if (node->getLeft() == nullptr)
	{
		return node->getRight();
	}
	node->setLeft(this->RemoveMin(node->getLeft()));
	return this->Balance(node);
}

template <class T>
int Tree<T>::IdeaHeight()
{
	return ceil(log(this->count + 1)/log(2) - 1);
}

/* Rotation functions */
template <class T>
TreeNode<T>* Tree<T>::RotateRight(TreeNode<T>* node)
{
	TreeNode<T>* temp = node->getLeft();
	node->setLeft(temp->getRight());
	temp->setRight(node);
	this->UpdateHeight(node);
	this->UpdateHeight(temp);
	return temp;
}

template <class T>
TreeNode<T>* Tree<T>::RotateLeft(TreeNode<T>* node)
{
	TreeNode<T>* temp = node->getRight();
	node->setRight(temp->getLeft());
	temp->setLeft(node);
	this->UpdateHeight(node);
	this->UpdateHeight(temp);
	return temp;
}

/* Balancing function */
template <class T>
TreeNode<T>* Tree<T>::Balance(TreeNode<T>* node)
{
	// first update height of the node
	this->UpdateHeight(node);
	if (this->BalanceFactor(node) == 2)
	{
		// right-heavy
		if (this->BalanceFactor(node->getRight()) < 0)
		{
			node->setRight(this->RotateRight(node->getRight()));
		}
		return this->RotateLeft(node);
	}
	else if (this->BalanceFactor(node) == -2)
	{
		// left-heavy
		if (this->BalanceFactor(node->getLeft()) > 0)
		{
			node->setLeft(this->RotateLeft(node->getLeft()));
		}
		return this->RotateRight(node);
	}
	return node; // no balance needed
}

/* delete */
template <class T>
void Tree<T>::Delete(T* data)
{
	this->Delete(*data);
}

template <class T>
void Tree<T>::Delete(T data)
{
	this->root = this->Delete(this->root, data);
}

template <class T>
TreeNode<T>* Tree<T>::Delete(TreeNode<T>* node, T value)
{
	if (node == nullptr)
	{
		return nullptr;
	}
	if (value < *(node->getValue()))
	{
		node->setLeft(this->Delete(node->getLeft(), value));
	}
	else if (value > *(node->getValue()))
	{
		node->setRight(this->Delete(node->getRight(), value));
	}
	else //  value == node->key
	{
		TreeNode<T>* q = node->getLeft();
		TreeNode<T>* r = node->getRight();
		delete node;
		this->count--;
		if (r == nullptr)
		{
			return q;
		}
		TreeNode<T>* min = this->MinNode(r);
		min->setRight(this->RemoveMin(r));
		min->setLeft(q);
		return this->Balance(min);
	}
	return this->Balance(node);
}

/* Min/Max */
template <class T>
T Tree<T>::Minimum()
{
	// find the left most leaf
	// the root must not be null or else this function will crash
	// in case root is null, there is no minimum value anyway
	TreeNode<T>* minNode = this->MinNode(this->root);
	return *(minNode->getValue());
}

template <class T>
T Tree<T>::Maximum()
{
	// find the right most leaf
	// the root must not be null or else this function will crash
	// in case root is null, there is no maximum value anyway
	TreeNode<T>* temp = this->root;
	while (temp->getRight() != nullptr)
	{
		temp = temp->getRight();
	}
	return *(temp->getValue());
}

/* Print */
template <class T>
void Tree<T>::Print()
{
	cout << "Height: " << this->HeightOf(this->root) << " (Idea height: " << this->IdeaHeight() << ")" << endl;
	cout << "Tree elements: ";
	if (this->count <= 200)
	{
		this->Print(this->root);
	}
	else
	{
		cout << " ...too much input (" << this->count << ")";
	}
	cout << endl << endl;
}

template <class T>
void Tree<T>::Print(TreeNode<T>* anchor)
{
	// print the tree in ascending order
	if (anchor != nullptr)
	{
		this->Print(anchor->getLeft());
		cout << *(anchor->getValue()) << " ";
		this->Print(anchor->getRight());
	}
}
