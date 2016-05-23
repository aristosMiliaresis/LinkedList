#include "linkedList.hpp"

#include <iostream>


template <class T> const char* LinkedList<T>::LinkedListExcept::errStrings[] = {
	"removeAtCurrentIndex/dequeue/pop() failed: list is empty!",
	"prev() failed: index points to head of the list!",
	"next() failed: index points to tail of the list!",
	"insertAtCurrentIndex/enqueue/push() failed: list is full!", 
	"get() failed: index out of range!"	
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


template <class T> LinkedList<T>::~LinkedList() 
{
	this->clear();
	delete pHead;
}


template <class T> LinkedList<T>::LinkedList(const LinkedList<T>& obj) : LinkedList()
{
	try {
		this->push(obj.get(0));
	} catch (const LinkedList<T>::LinkedListExcept& e) {
		std::cerr << this << " -> LinkedList<T>(): Error in LinkedList<T>(" << &obj << ") > " << e.what() << std::endl;
		return;
	}

	while (obj.hasNext())
		this->push(obj.next());
}


template <class T> void LinkedList<T>::push(T item)
{
	if (itemCount == UINT32_MAX)
	{
		throw LinkedListExcept(LinkedListExcept::errCodes::ERR_LIST_FULL);
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

	pIndex = pTail;
	try {
		this->removeAtCurrentIndex();
	} catch (const LinkedList<T>::LinkedListExcept& e) {
		std::cerr << this << " -> LinkedList<T>(): Error in pop() > " << e.what() << std::endl;
		throw e;
		return T();
	}
	
	return oldItem;
}


template <class T> T LinkedList<T>::getLast() const
{
	return pTail->item;
}


template <class T> void LinkedList<T>::enqueue(T item)
{
	pIndex = pHead;
	try {
		this->insertAtCurrentIndex(item);
	} catch (const LinkedList<T>::LinkedListExcept& e) {
		std::cerr << this << " -> LinkedList<T>(): Error in enqueue() > " << e.what() << std::endl;
		throw e;
	}

	return;
}
	
	
template <class T> T LinkedList<T>::dequeue()
{
	const T oldItem = pHead->item;
	
	pIndex = pHead;
	try {
		this->removeAtCurrentIndex();
	} catch (const LinkedList<T>::LinkedListExcept& e) {
		std::cerr << this << " -> LinkedList<T>(): Error in dequeue() > " << e.what() << std::endl;
		throw e;
		return T();
	}
	
	return oldItem;
}


template <class T> T LinkedList<T>::getFirst() const
{
	return pHead->item;
}
	

template <class T> bool LinkedList<T>::insert(T item, uint32_t index)
{
	try {
		(void) this->get(index);
	} catch (const LinkedList<T>::LinkedListExcept& e) {
		std::cerr << this << " -> LinkedList<T>(): Error in insert() > " << e.what() << std::endl;
		return false;
	}

	try {
		this->insertAtCurrentIndex(item);
	} catch (const LinkedList<T>::LinkedListExcept& e) {
		std::cerr << this << " -> LinkedList<T>(): Error in insert() > " << e.what() << std::endl;
		return false;
	}

	return true;
}
	
	
template <class T> T LinkedList<T>::get(uint32_t index) const
{
	if (index >= itemCount)
	{
		throw LinkedListExcept(LinkedListExcept::errCodes::INDEX_OUT_OF_RANGE);
		return T();
	}

	pIndex = pHead;
	for (uint32_t i = 0; i < index; i++)
		(void) this->next(); 
	
	return pIndex->item;
}


template <class T> uint32_t LinkedList<T>::getCurrIndex() const
{
	struct Node<T>* savedIndex = pIndex;
	uint32_t index = 0;

	pIndex = pHead;
	while (pIndex != savedIndex)
	{
		try {
			(void) this->next();
		} catch (const LinkedList<T>::LinkedListExcept& e) {
			std::cerr << this << " -> LinkedList<T>(): Error in getCurrIndex() > " << e.what() << std::endl;
			return UINT32_MAX;
		}
		index++;
	}

	return index;
}


template <class T> uint32_t LinkedList<T>::nextIndexOf(T item) const
{
	for (uint32_t index = this->getCurrIndex(); pIndex != pTail;)
	{
		++index;
		if (this->next() == item)
			return index;
	}

	return UINT32_MAX;
}


template <class T> uint32_t LinkedList<T>::prevIndexOf(T item) const
{
 	for (uint32_t index = this->getCurrIndex(); pIndex != pHead;)
 	{
 		--index;
		if (this->prev() == item)
			return index;
	}

	return UINT32_MAX;
}


template <class T> uint32_t LinkedList<T>::indexOf(T item) const
{
	pIndex = pHead;
	return nextIndexOf(item);
}


template <class T> uint32_t LinkedList<T>::lastIndexOf(T item) const
{
	pIndex = pTail;
	return prevIndexOf(item);
}


template <class T> std::vector<uint32_t>* LinkedList<T>::allIndexsOf(T item) const
{
	std::vector<uint32_t>* vec;
	try {
		vec = new std::vector<uint32_t>();
	} catch (const std::bad_alloc& exc) {
		std::cerr << exc.what() << std::endl;
		throw exc;
		return NULL;
	}
	
	uint32_t index;

	pIndex = pHead;
	while (pIndex != pTail)
	{
		index = nextIndexOf(item);
		if (index == UINT32_MAX)
			return vec;
		vec->push_back(index);
	}

	if (vec->empty())
		return NULL;
	return vec;
}


template <class T> bool LinkedList<T>::replace(T item, uint32_t index)
{
	try {
		(void) this->get(index);
	} catch (const LinkedList<T>::LinkedListExcept& e) {
		std::cerr << this << " -> LinkedList<T>(): Error in replace() > " << e.what() << std::endl;
		return false;
	}

	pIndex->item = item;
	return true;
}


template <class T> bool LinkedList<T>::removeAt(uint32_t index)
{
	try {
		(void) this->get(index);
	} catch (const LinkedList<T>::LinkedListExcept& e) {
		std::cerr << this << " -> LinkedList<T>(): Error in removeAt() > " << e.what() << std::endl;
		return false;
	}

	this->removeAtCurrentIndex();
	return true;
}


template <class T> bool LinkedList<T>::remove(T item)
{
	if (this->contains(item))
	{
		this->removeAtCurrentIndex();
		return true;
	}

	return false;
}


template <class T> uint32_t LinkedList<T>::removeAll(T item)
{
	uint32_t instCount = 0;

	while (this->contains(item))
	{
		this->removeAtCurrentIndex();
		instCount++;
	}

	return instCount;
}


template <class T> bool LinkedList<T>::contains(T item) const
{
	pIndex = pHead;

	do {
		if (pIndex->item == item)
			return true;

		pIndex = pIndex->pNext;
	} while (pIndex->pNext != NULL);

	if (pIndex->item == item)
		return true;
	else
		return false;
}


template <class T> void LinkedList<T>::clear()
{
	while (this->count() > 0)
		(void) this->pop();
}


template <class T> std::vector<T>* LinkedList<T>::toVector() const
{
	std::vector<T>* vec;
	try {
		vec = new std::vector<T>();
	} catch (const std::bad_alloc& exc) {
		std::cerr << exc.what() << std::endl;
		throw exc;
		return NULL;
	}
	
	struct Node<T>* savedIndex = pIndex;

	for (uint32_t i = 0; i < itemCount; i++)
		vec->push_back(this->get(i));

	pIndex = savedIndex;
	if (vec->empty())
		return NULL;
	return vec;
}


template <class T> T LinkedList<T>::next() const
{
	if (pIndex->pNext != NULL)
	{
		pIndex = pIndex->pNext;
		return pIndex->item;
	}
	
	throw LinkedListExcept(LinkedListExcept::errCodes::ERR_NO_NEXT);
	return T();
}


template <class T> T LinkedList<T>::prev() const
{
	if (pIndex->pPrev != NULL)
	{
		pIndex = pIndex->pPrev;
		return pIndex->item;
	}
	
	throw LinkedListExcept(LinkedListExcept::errCodes::ERR_NO_PREV);
	return T();
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


template <class T> void LinkedList<T>::removeAtCurrentIndex()
{
	if (itemCount == 0)
	{
		throw LinkedListExcept(LinkedListExcept::errCodes::ERR_EMPTY_LIST);
		return;
	}
	else if (itemCount == 1)
	{
		itemCount--;
		return;
	}

	const Node<T>* oldNode = pIndex;

	// remove from the head of the list.
	if (pIndex->pPrev == NULL)
	{
		pHead = pHead->pNext;
		pHead->pPrev = NULL;
		pIndex = pHead;
	} 
	// remove from the tail of the list.
	else if (pIndex->pNext == NULL)
	{
		pTail = pTail->pPrev;
		pTail->pNext = NULL;
		pIndex = pTail;
	}
	else
	{
		pIndex->pPrev->pNext = pIndex->pNext;
		pIndex->pPrev->pNext->pPrev = pIndex->pPrev;
		pIndex = pIndex->pPrev;
	}

	itemCount--;
	delete oldNode;
}


template <class T> void LinkedList<T>::insertAtCurrentIndex(T item)
{
	if (itemCount == UINT32_MAX)
	{
		throw LinkedListExcept(LinkedListExcept::errCodes::ERR_LIST_FULL);
		return;
	}
	
	if (pIndex->pPrev == NULL)
	{
		if (itemCount == 0)
		{
			pHead->item = item;
		}
		else
		{
			Node<T>* newNode;
			try {
				newNode = new struct Node<T>();
			} catch (const std::bad_alloc& exc) {
				std::cerr << exc.what() << std::endl;
				throw exc;
				return;
			}
			
			newNode->pPrev = NULL;
			newNode->pNext = pHead;
			pHead->pPrev = newNode;
			pHead = newNode;

			pHead->item = item;
			pIndex = pHead;
		}
	}
	else
	{
		Node<T>* prevNode = pIndex->pPrev;
		try {
			pIndex->pPrev = new struct Node<T>();
		} catch (const std::bad_alloc& exc) {
			std::cerr << exc.what() << std::endl;
			throw exc;
			return;
		}
	
		pIndex->pPrev->pNext = pIndex;
		pIndex->pPrev->item = item;
		pIndex->pPrev->pPrev = prevNode;
		pIndex->pPrev->pPrev->pNext = pIndex->pPrev;
		pIndex = pIndex->pPrev;
	}

	itemCount++;
}
