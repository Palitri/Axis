#pragma once

#include "..\AxGlobals.h"

#include "AxMem.h"

template <typename T> class AxLinkedListNode
{
public:
	AxLinkedListNode *previous, *next;
	T value;
};

template <typename T> class AxLinkedList
{
public:
	AxLinkedListNode<T> *last, *first;
	int count;

	AxLinkedList(void)
	{
		this->first = 0;
		this->last = 0;

		this->count = 0;
	};

	virtual ~AxLinkedList(void)
	{
		this->Clear();
	};
	
	void Add(T value)
	{
		this->InsertAfter(this->last, value);
	};

	bool IsEmpty()
	{
		return this->last == 0;
	}

	void Clear()
	{
		while (this->last != 0)
			this->RemoveNode(this->last);
	}

	void *FindFirst(T value)
	{
		AxLinkedListNode<T> *node = this->first;

		while (node != 0)
		{
			if (AxMem::Compare(&node->value, &value, sizeof(T)) == 0)
				return node;

			node = node->next;
		}

		return 0;
	};

	void *FindLast(T value)
	{
		AxLinkedListNode<T> *node = this->last;

		while (node != 0)
		{
			if (AxMem::Compare(&node->value, &value, sizeof(T)) == 0)
				return node;

			node = node->previous;
		}

		return 0;
	};

	virtual void InsertBefore(void *node, T value)
	{
		AxLinkedListNode<T> *listNode = (AxLinkedListNode<T>*) node;
		
		AxLinkedListNode<T> *newNode = new AxLinkedListNode<T>();
		newNode->value = value;

		newNode->next = listNode;
		if (listNode != 0)
		{
			newNode->previous = listNode->previous;
			if (listNode->previous != 0)
				listNode->previous->next = newNode;
			else
				this->first = newNode;
			listNode->previous = newNode;
		}
		else
		{
			newNode->previous = 0;
			this->last = newNode;
			this->first = newNode;
		}

		this->count++;
	}

	virtual void InsertAfter(void *node, T value)
	{
		AxLinkedListNode<T> *listNode = (AxLinkedListNode<T>*) node;
		
		AxLinkedListNode<T> *newNode = new AxLinkedListNode<T>();
		newNode->value = value;

		newNode->previous = listNode;
		if (listNode != 0)
		{
			newNode->next = listNode->next;
			if (listNode->next != 0)
				listNode->next->previous = newNode;
			else
				this->last = newNode;
			listNode->next = newNode;
		}
		else
		{
			newNode->next = 0;
			this->last = newNode;
			this->first = newNode;
		}

		this->count++;
	}

	virtual void RemoveNode(void *node)
	{
		AxLinkedListNode<T> *listNode = (AxLinkedListNode<T>*) node;
		
		if (listNode->previous != 0)
			listNode->previous->next = listNode->next;
		else
			this->first = listNode->next;

		if (listNode->next != 0)
			listNode->next->previous = listNode->previous;
		else
			this->last = listNode->previous;

		delete listNode;

		this->count--;
	}

	void *GetNodeNext(void *node)
	{
		if (node != 0)
			return ((AxLinkedListNode<T>*)node)->next;
		else
			return this->first;
	}

	void *GetNodePrevious(void *node)
	{
		if (node != 0)
			return ((AxLinkedListNode<T>*)node)->previous;
		else
			return this->last;
	}

	T &GetNodeValue(void *node)
	{
		return ((AxLinkedListNode<T>*)node)->value;
	}

	void SetNodeValue(void *node, T value)
	{
		((AxLinkedListNode<T>*)node)->value = value;
	}
};