#include "linkedList.hpp"

#include <iostream>
#include <stdint.h>


template <class T> const char* LinkedList<T>::MException::errStrings[] = {
	"dequeue/pop() failed: list is empty!",				// when dequeue() or pop() is called and list is empty.
	"prev() failed: index points to head of the list!",	// when prev() is called and list index is at the head of the list.
	"next() failed: index points to tail of the list!",	// when next() is called and list index is at the tail of the list.
	"enqueue/push/set() failed: list is full!",			// when enqueue(), push() or set() is called and itemCount == UINT32_MAX.
	"get() failed: index out of range!"					// when get() is called with index outside the lists range.
};


template <class T> LinkedList<T>::LinkedList() : itemCount(0) 
{
	try {
		pHead = new struct Node<T>();
	} catch (const std::bad_alloc& exc) {
		std::cerr << exc.what() << std::endl;
		throw exc;
	}
	
	pHead->pPrev = NULL;
	pHead->pNext = NULL;
	pTail = pHead;
	pIndex = pHead;
}


template <class T> LinkedList<T>::LinkedList(const LinkedList& obj) : LinkedList()
{
	this->push(obj.get(0));
	while (obj.hasNext())
		this->push(obj.next());
}


template <class T> LinkedList<T>::~LinkedList() 
{
	do {
		try {
			(void) this->pop();
		} catch (const LinkedList<T>::MException& e) {
			break;
		}
	} while (true);

	delete pHead;
}


template <class T> void LinkedList<T>::push(T item)
{
	if (itemCount == UINT32_MAX)
	{
		throw MException(MException::errCodes::ERR_LIST_FULL);
		return;
	}
	else if (itemCount != 0)
	{
		try {
			pTail->pNext = new struct Node<T>();
		} catch (const std::bad_alloc& exc) {
			std::cerr << exc.what() << std::endl;
			throw exc;
			return;
		}	
		pTail->pNext->pPrev = pTail;
		pTail = pTail->pNext;
	}
	
	itemCount++;	
	pTail->item = item;
	pIndex = pTail;
	
	return;
}


template <class T> T LinkedList<T>::pop()
{
	const T oldItem = pTail->item;

	if (itemCount > 1)
	{
		pTail = pTail->pPrev;
		delete pTail->pNext;
		pTail->pNext = NULL;
	}
	else if (itemCount == 0)
	{
		throw MException(MException::errCodes::ERR_EMPTY_LIST);
		return T();
	}
	
	itemCount--;
	pIndex = pTail;
	return oldItem;
}


template <class T> void LinkedList<T>::enqueue(T item)
{
	if (itemCount == UINT32_MAX)
	{
		throw MException(MException::errCodes::ERR_LIST_FULL);
		return;
	}
	else if (itemCount != 0)
	{
		Node<T>* newNode = new struct Node<T>();
	
		newNode->pPrev = NULL;
		newNode->pNext = pHead;
		pHead->pPrev = newNode;
		pHead = newNode;
	}

	pHead->item = item;
	pIndex = pHead;
	itemCount++;
}
	
	
template <class T> T LinkedList<T>::dequeue()
{
	const T oldItem = pHead->item;
	
	if (itemCount > 1)
	{
		pHead = pHead->pNext;
		delete pHead->pPrev;
		pHead->pPrev = NULL;
	}
	else if (itemCount == 0)
	{
		throw MException(MException::errCodes::ERR_EMPTY_LIST);
		return T();
	}
	
	itemCount--;
	pIndex = pHead;
	return oldItem;
}
	

template <class T> bool LinkedList<T>::set(T item, uint32_t index)
{
	if (itemCount == UINT32_MAX)
	{
		throw MException(MException::errCodes::ERR_LIST_FULL);
		return false;
	}
	
	try {
		(void) this->get(index);
	} catch (const LinkedList<T>::MException& e) {
		std::cerr << "LinkedList(" << this << ") Error in set() > " << e.what() << std::endl;
		return false;
	}

	Node<T>* prevNode = pIndex->pPrev;
	try {
		pIndex->pPrev = new struct Node<T>();
	} catch (const std::bad_alloc& exc) {
		std::cerr << exc.what() << std::endl;
		throw exc;
		return false;
	}
	pIndex->pPrev->pNext = pIndex;
	pIndex->pPrev->item = item;
	pIndex->pPrev->pPrev = prevNode;
	pIndex->pPrev->pPrev->pNext = pIndex->pPrev;
	pIndex = pIndex->pPrev;

	itemCount++;
	return true;
}
	
	
template <class T> T LinkedList<T>::get(uint32_t index) const
{
	if (index >= itemCount)
	{
		throw MException(MException::errCodes::INDEX_OUT_OF_RANGE);
		return T();
	}

	pIndex = pHead;
	for (uint32_t i = 0; i < index; i++)
		(void) this->next();
	
	return pIndex->item;
}


template <class T> bool LinkedList<T>::replace(T item, uint32_t index)
{
	try {
		(void) this->get(index);
	} catch (const LinkedList<T>::MException& e) {
		std::cerr << "LinkedList(" << this << ") Error in replace() > " << e.what() << std::endl;
		return false;
	}

	pIndex->item = item;
	return true;
}


template <class T> bool LinkedList<T>::remove(uint32_t index)
{
	try {
		(void) this->get(index);
	} catch (const LinkedList<T>::MException& e) {
		std::cerr << "LinkedList(" << this << ") Error in remove() > " << e.what() << std::endl;
		return false;
	}

	const Node<T>* oldNode = pIndex;
	pIndex->pPrev->pNext = pIndex->pNext;
	pIndex->pPrev->pNext->pPrev = pIndex->pPrev;
	pIndex = pIndex->pPrev;

	itemCount--;
	delete oldNode;
	return true;
}


template <class T> T LinkedList<T>::next() const
{
	if (pIndex->pNext != NULL)
	{
		pIndex = pIndex->pNext;
		return pIndex->item;
	}
	
	throw MException(MException::errCodes::ERR_NO_NEXT);
	return T();
}


template <class T> T LinkedList<T>::prev() const
{
	if (pIndex->pPrev != NULL)
	{
		pIndex = pIndex->pPrev;
		return pIndex->item;
	}
	
	throw MException(MException::errCodes::ERR_NO_PREV);
	return NULL;
}


template <class T> bool LinkedList<T>::hasNext() const
{
	return (pIndex->pNext != NULL);
}


template <class T> bool LinkedList<T>::hasPrev() const
{
	return (pIndex->pPrev != NULL);
}


template <class T> uint32_t LinkedList<T>::count() const
{
	return itemCount;
}
