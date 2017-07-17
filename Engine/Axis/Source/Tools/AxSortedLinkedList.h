#pragma once

#include "AxLinkedList.h"
#include "AxBinaryTree.h"

template <typename T> class AxSortedLinkedList
{
private:
	AxBinaryTree<void*> tree;
	AxLinkedList<T> list;

	int keyBitCount, mostSignificantBit;

public:

	AxSortedLinkedList(int keysBitCount)
	{
		this->keyBitCount = keysBitCount;
		this->mostSignificantBit = 1 << (this->keyBitCount - 1);
	};

	virtual ~AxSortedLinkedList(void)
	{
	};

	void Add(unsigned long long key, T value)
	{
		AxBinaryTree<void*> *bitBranch = &this->tree;
		int bitIndex = 0;
		int bit;

		// Search for leaf matching the path of the key's bits
		while (bitIndex < this->keyBitCount)
		{
			bit = key & this->mostSignificantBit;

			if (bit == 0)
			{
				if (bitBranch->left != 0)
					bitBranch = bitBranch->left;
				else
					break;
			}
			else
			{
				if (bitBranch->right != 0)
					bitBranch = bitBranch->right;
				else
					break;
			}

			key <<= 1;
			bitIndex++;
		}

		// If such path does not exist, create it and at the end, put a leaf pointing to the list node
		if (bitIndex < this->keyBitCount)
		{
			void *newNode;

			if (this->list.count == 0)
			{
				this->list.Add(value);
				newNode = this->list.first;
			}
			else
			{
				if (bit == 0)
				{
					AxBinaryTree<void*> *nextBiggest = bitBranch->FindLeftmostLeaf();
					this->list.InsertBefore(nextBiggest->value, value);
					newNode = this->list.GetNodePrevious(nextBiggest->value);				
				}
				else
				{
					AxBinaryTree<void*> *nextSmallest = bitBranch->FindRightmostLeaf();
					this->list.InsertAfter(nextSmallest->value, value);
					newNode = this->list.GetNodeNext(nextSmallest->value);
				}
			}


			while (bitIndex < this->keyBitCount)
			{
				if (bit == 0)
				{
					bitBranch->AddLeft(0);
					bitBranch = bitBranch->left;
				}
				else
				{
					bitBranch->AddRight(0);
					bitBranch = bitBranch->right;
				}

				key <<= 1;
				bitIndex++;
				bit = key & this->mostSignificantBit;
			}

			bitBranch->value = newNode;
		}
		// If such path exists, add the list node at its end
		else
		{
			int x = 0;
			return;
		}
	};

	void *GetFirstNode()
	{
		if (this->list.count == 0)
			return 0;

		return this->list.first;
	}

	void *GetNextNode(void *node)
	{
		if (node == 0)
			return 0;

		return ((AxLinkedListNode<T>*)node)->next;
	}

	T GetNodeValue(void *node)
	{
		return this->list.GetNodeValue(node);
	}
};

