/*
* BinarySearchTree.h
* This file contains the declarations and definitions for the members of the BinarySearchTree class
* 
* @author Trent Haines
* @date 11/13/19
*/

#ifndef BINARYSEARCHTREE_H_
#define BINARYSEARCHTREE_H_

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <stack>


// forward declaration of the template class binarySearchTree
template<class DataType>
class binarySearchTree;

// treeNode class
template <class DataType>
class treeNode
{
	friend class binarySearchTree<DataType>;
	// the rest of the treeNode class declaration goes here
private:
	DataType dataType; // data member 
	treeNode<DataType>* rightC; // pointer for the right child of the tree node
	treeNode<DataType>* leftC; //pointer for the left child of the tree node


public:
	treeNode(); // constructor
	treeNode(const treeNode& other); //copy constructor
	treeNode(const DataType& newItem); //overloaded constructor
	

protected:

	treeNode* right() const //returns the right child of the node
	{
		return rightC;
	}
	treeNode* left() const //returns the left child of the node
	{
		return leftC;
	}

	void right(treeNode* rightNode) //sets the contents of the right child
	{
		rightC = rightNode;
	}
	void left(treeNode* leftNode) // sets the contnts of the left child
	{
		leftC = leftNode;
	}


	const DataType& data() const //returns the data contained by the node
	{
		return dataType; 
	}


};

// binarySearchTree class
template <class DataType>
class binarySearchTree
{
	// Your binarySearchTree goes here

private:
	treeNode<DataType>* root; //root node
	int nodeCount; //counts the number of nodes in the tree

public:
	bool empty() const //returns a boolean indicating if the tree is empty or not
	{
		bool ans = (root == nullptr);
		return ans;
	}

	std::size_t size() const //returns the total number of nodes in the tree
	{
		return nodeCount;
	}

	void print() const // displays the contents of the binary tree by key order
	{
		print(std::cout);
	}

	void print(std::ostream& out) const; //overload for printing

	void debug(std::ostream& out) const; // displays the contents of the tree for debugging purposes

	bool find(const DataType& searchItem, void (*foundNode)(const DataType&)); //returns true if the item is found, false if not. The first parameter gets passed to the find member function and the second parameter is the addres that will be called if found
	bool find(const DataType& searchItem); // called by find

	bool erase(const DataType& deleteItem); // erases the passed item, returns true if found, false if not

	void insert(const DataType& newItem); //inserts the item into the binary tree. If already in the tree, it replaces the old entry
	void insert(const DataType& newItem, void (*duplicateItemFound)(DataType& existingItem, const DataType& newItem)); // called if a duplicate item is found when the insert function is called

	void update(DataType& existingItem, const DataType& newItem); //updates the tree if a duplicate is found

	void traverse(void (*itemFound)(const DataType& foundItem)) const; // traverses through the tree and calls the itemFound for each item and passes its reference


	binarySearchTree(); //default constructor
	binarySearchTree(const binarySearchTree& other); // copy constructor

	binarySearchTree& operator=(const binarySearchTree& other); // assignment operator

	void copy(treeNode<DataType>* node); //copy function

	void deleteAll(); //deletes all of the entries
	void deleteAll(treeNode<DataType>* node); //deletes each node
	virtual ~binarySearchTree(); //destructor, utilizes deleteAll()

};


// treeNode Implementation Begins

template<class DataType> //default constructor implementation
treeNode<DataType>::treeNode() : dataType(), rightC(nullptr), leftC(nullptr)
{

}

template<class DataType> //copy constructor implementation
treeNode<DataType>::treeNode(const treeNode& other) : dataType(other.dataType), rightC(other.rightC), leftC(other.leftC)
{

}

template<class DataType> //overloaded constructor implementation
treeNode<DataType>::treeNode(const DataType& newItem) : dataType(newItem), rightC(nullptr), leftC(nullptr)
{

}

// treeNode Implementation Ends


// binarySearchTree Implementation Begins

template<class DataType> // implementation of debug function, used for user testing
void binarySearchTree<DataType>::debug(std::ostream& out) const
{
	treeNode<DataType>* current = root;
	treeNode<DataType>* temp;
	unsigned int i = 1;
	out << "DEBUG STARTED" << std::endl;
	if (root == nullptr)
	{
		out << "Tree has no nodes";
		return;
	}
	else
	{
		std::setw(15);
		out << "root = " << root << std::endl;
		out << "there are: " << size() << " nodes" << std::endl;
		//print left to right 
		while (current != nullptr)
		{
			if (current->left() == nullptr)
			{
				out << "node " << std::setw(5) << i << " = " << std::setw(15) << current << std::endl;
				out << "right = " << current->right() << std::endl;
				out << "left = " << current->left() << std::endl;
				out << "value = " << current->data() << std::endl;

				current = current->right(); // traverses
			}
			else
			{
				temp = current->left(); // searches the left half

				while (!(temp->right() == nullptr || temp->right() == current))
				{
					temp = temp->right(); // moves right
				}
				if (temp->right() == current)
				{
					temp->right(nullptr);
					out << "node " << std::setw(5) << i << " = " << std::setw(15) << current << std::endl;
					out << "right = " << current->right() << std::endl;
					out << "left = " << current->left() << std::endl;
					out << "value = " << current->data() << std::endl;
					current = current->right();
				}
				else 
				{
					temp->right(current);
					current = current->left();
				}
			}
			i++;
		}
	}

}



template<class DataType>
void binarySearchTree<DataType>::print(std::ostream& out) const
{
	treeNode<DataType>* current = root;
	treeNode<DataType>* temp;


	if (root == nullptr)
		return;
	while (current != nullptr)
	{
		if (current->left() == nullptr)
		{
			out << current->data() << std::endl;

			current = current->right(); // traverses
		}
		else
		{
			temp = current->left(); // searches the left half

			while (!(temp->right() == nullptr || temp->right() == current))
			{
				temp = temp->right(); // moves right
			}
			if (temp->right() == current) //goes back
			{
				temp->right(nullptr);
				out << current->data() << std::endl;
				current = current->right();
			}
			else //goes to the child
			{
				temp->right(current);
				current = current->left();
			}
		}
	}

}
 
template <class DataType> // implementation of the find method that has the static method call 
bool binarySearchTree<DataType>::find(const DataType& searchItem, void (*foundNode)(const DataType&))
{
	treeNode<DataType>* current = root;
	while (current != nullptr)
	{
		if (current->data() == searchItem) // calls the static function when item is foudn
		{
			(*foundNode)(searchItem);
			return true;
		}
		else 
		{
			if (searchItem > current->data())
				current = current->right();
			else
				current = current->left();
		}
	}
	return false;
}

template <class DataType> // implementation of the find method, called by other function
bool binarySearchTree<DataType>::find(const DataType& searchItem)
{
	treeNode<DataType>* current = root;
	while (current != nullptr)
	{
		if (current->data() == searchItem)
		{
			return true;
		}
		else
		{
			if (searchItem > current->data())
				current = current->right();
			else
				current = current->left();
		}
	}
	return false;
}

template<class DataType> //implementation of the erase function, removes the passed node from the tree
bool binarySearchTree<DataType>::erase(const DataType& deleteItem)
{
	treeNode<DataType>* current = root;
	treeNode<DataType>* predecessor = nullptr;

	if (root != nullptr && root->data() == deleteItem)
	{
		root = nullptr; //delete the root node
		return true;
	}
	else
	{
		while(current != nullptr)
		{
			if (current->data() == deleteItem)
			{
				if (current->right() == nullptr)
				{
					if (current->left() == nullptr)
					{
						if (predecessor == nullptr)
							root = nullptr;
						else if (predecessor->left() == current)
							predecessor->left(nullptr);
						else
							predecessor->right(nullptr);
					}
					else
					{
						if (predecessor == nullptr)
							root = current->left();
						else if (predecessor->left() == current)
							predecessor->left(current->left());
						else
							predecessor->right(current->right());
					}
				}
				else
				{
					if (current->left() == nullptr)
					{
						if (predecessor == nullptr)
							root = current->right();
						else if (predecessor->left() == current)
							predecessor->left(current->right());
						else
							predecessor->right(current->right());

					}
					else
					{
						treeNode<DataType>* successor = current->right();
						while (successor->left() != nullptr)
						{
							successor = successor->left();
						}
						erase(successor->data()); //remove temp data
					}
					nodeCount--;
					return true;
				}
			}
			else if (deleteItem > current->data())
			{
				predecessor = current;
				current = current->right();
			}
			else
			{
				predecessor = current;
				current = current->left();
			}
		}
		return false;
	}
}

template <class DataType> // insert function implementation
void binarySearchTree<DataType>::insert(const DataType& newItem)
{
	if (!find(newItem))
	{
		treeNode<DataType>* nodeIn = new treeNode<DataType>(newItem);
		if (empty())
		{
			nodeIn->left(nullptr);
			nodeIn->right(nullptr);
			root = nodeIn;
			nodeCount++;
			return;
		}
		
		treeNode<DataType>* current = root;
		while (current != nullptr)
		{
			if (newItem < current->data())
			{
				if (current->left() == nullptr)
				{
					current->left(nodeIn);
					current = nullptr;
				}
				else
					current = current->left();
			}
			else
			{
				if (current->right() == nullptr)
				{
					current->right(nodeIn);
					current = nullptr;
				}
				else
					current = (current->right());
				
			}
		}
		
		nodeIn->left(nullptr);
		nodeIn->right(nullptr);
		nodeCount++;

	}
}

template <class DataType> //insert function, uses the static call or other insert function
void binarySearchTree<DataType>::insert(const DataType& newItem, void (*duplicateItemFound)(DataType& existingItem, const DataType& newItem))
{
	if (find(newItem))
	{
		DataType a = newItem;
		(*duplicateItemFound)(a, newItem);
		return;
	}
	insert(newItem);
}


template <class DataType> //updates the node, removes the existing data and adds the new data
void binarySearchTree<DataType>::update(DataType& existingItem, const DataType& newItem)
{
	erase(existingItem);
	insert(newItem);
}


template <class DataType> // implementation of traverse function, moves through the tree in order, calling the static function
void binarySearchTree<DataType>::traverse(void (*itemFound)(const DataType& foundItem)) const
{
	if (root != nullptr)
	{
		treeNode<DataType>* current = root;
		treeNode<DataType>* temp;

		while (current != nullptr)
		{
			if (current->left() == nullptr)
			{
				(*itemFound)(current->data());
				current = current->right();
			}
			else
			{
				temp = current->left();
				while (!(temp->right() == nullptr || temp->right() == current))
				{
					temp = temp->right();
				}

				if (temp->right() != nullptr)
				{
					temp->right(nullptr);
					(*itemFound)(current->data());
					current = current->right();
				}
				else
				{
					temp->right(current);
					current = current->left();
				}
			}
		}
	}
}

template<class DataType> // defuault constuctor implementation
binarySearchTree<DataType>::binarySearchTree() : root(nullptr), nodeCount(0)
{

}

template <class DataType> //copy constructor implementation
binarySearchTree<DataType>::binarySearchTree(const binarySearchTree& other)
{
	copy(other.root);
}

template <class DataType> // assignment operator implementation, assigns the contents of a tree to another
binarySearchTree<DataType>& binarySearchTree<DataType>::operator=(const binarySearchTree& other)
{
	deleteAll();
	copy(other.root);
	return *this;
}

template <class DataType> //copy function
void binarySearchTree<DataType>::copy(treeNode<DataType>* node)
{
	if (node == nullptr)
		return;
	else
	{
		insert(node->data());
		copy(node->left());
		copy(node->right());
	}
}

template<class DataType> //delete function used in destructor
void binarySearchTree<DataType>::deleteAll()
{
	deleteAll(root); //deletes the tree, overloaded/recursive function
	root = nullptr; //deletes the root in addition
}

template<class DataType>
void binarySearchTree<DataType>::deleteAll(treeNode<DataType>* node)
{
	if (node != nullptr) //zybook code, deletes to the left then right then current
	{
		deleteAll(node->left());
		node->left(nullptr);

		deleteAll(node->right());
		node->right(nullptr);

		delete node;
		nodeCount--;
	}
}


template<class DataType> //destructor
binarySearchTree<DataType>::~binarySearchTree()
{
	deleteAll();
}



#endif //BINARYSEARCHTREE_H_