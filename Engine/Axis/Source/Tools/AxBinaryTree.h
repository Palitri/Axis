#pragma once

template <typename T> struct AxBinaryTree
{
	AxBinaryTree<T> *parent, *left, *right;
	T value;

	AxBinaryTree(void)
	{
		this->parent = 0;
		this->left = 0;
		this->right = 0;
	};

	AxBinaryTree(AxBinaryTree<T> *parent, T value)
	{
		this->parent = parent;
		this->value = value;
		this->left = 0;
		this->right = 0;
	};

	~AxBinaryTree(void)
	{
		if (this->left != 0)
			delete this->left;

		if (this->right != 0)
			delete this->right;
	};

	// Traverses the tree up in the hierarchy, searching for a parent branch with another child. In other words, searches for the closest ancestor where lineage splits
	AxBinaryTree<T> *FindSplittingParent(AxBinaryTree<T> *branch)
	{
		AxBinaryTree<T> *result = this->parent;

		while (result != 0)
		{
			if ((branch->parent->left != 0) && (branch->parent->right != 0))
				break;
		}

		return result;
	};

	//static AxBinaryTree<T> *FindLeftmostLeaf(AxBinaryTree<T> *branch)
	//{
	//	if (branch->left != 0)
	//		return AxBinaryTree<T>::FindLeftmostLeaf(branch->left);

	//	if (branch->right != 0)
	//		return AxBinaryTree<T>::FindLeftmostLeaf(branch->right);

	//	return branch;
	//};

	//static AxBinaryTree<T> *FindRightmostLeaf(AxBinaryTree<T> *branch)
	//{
	//	if (branch->right != 0)
	//		return AxBinaryTree<T>::FindRightmostLeaf(branch->right);

	//	if (branch->left != 0)
	//		return AxBinaryTree<T>::FindRightmostLeaf(branch->left);

	//	return branch;
	//};

	AxBinaryTree<T> *FindLeftmostLeaf()
	{
		AxBinaryTree<T> *result = this;
		
		for (;;)
		{
			if (result->left != 0)
				result = result->left;

			else if (result->right != 0)
				result = result->right;

			else 
				break;
		}

		return result;
	}	
	
	AxBinaryTree<T> *FindRightmostLeaf()
	{
		AxBinaryTree<T> *result = this;
		
		for (;;)
		{
			if (result->right != 0)
				result = result->right;

			else if (result->left != 0)
				result = result->left;

			else
				break;
		}

		return result;
	}

	bool AddLeft(T value)
	{
		if (this->left != 0)
			return false;

		this->left = new AxBinaryTree<T>(this, value);
	};	
	
	bool AddRight(T value)
	{
		if (this->right != 0)
			return false;

		this->right = new AxBinaryTree<T>(this, value);
	};

	void RemoveLeft()
	{
		if (this->left == 0)
			return;

		delete this->left;
		this->left = 0;
	};

	void RemoveRight()
	{
		if (this->right == 0)
			return;

		delete this->right;
		this->right = 0;
	};
};