#include "linkedList.hpp"

#include <iostream>
#include <stdint.h>


template <class T> const char* LinkedList<T>::MException::errStrings[] = {
	"pop() failed: list is empty!",				// set when pop() is called and list is empty.
	"prev() failed: index points to base of the list!",	// set when prev() is called and current node is at the base of the list.
	"next() failed: index points to top of the list!",	// set when next() is called and current node is at the top of the list.
	"push/insert() failed: list is full!",			// set when push() or insert() is called and nodeCount == UINT32_MAX.
	"goTo() failed: index out of range!"			// set when goTo() is called with index outside the lists range.
};


template <class T> LinkedList<T>::LinkedList() : nodeCount(0) 
{
	try {
		pBase = new struct Node<T>();
	} catch (const std::bad_alloc& exc) {
		std::cerr << exc.what() << std::endl;
		throw exc;
	}
	
	pBase->pPrev = NULL;
	pBase->pNext = NULL;
	pTop = pBase;
	pCurrent = pBase;
}


template <class T> LinkedList<T>::LinkedList(const LinkedList& obj) : LinkedList()
{
	obj.goToBase();
	
	this->push(obj.getCurrent());
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

	delete pBase;
}


template <class T> void LinkedList<T>::push(T value)
{
	if (nodeCount == UINT32_MAX)
	{
		throw MException(MException::errCodes::ERR_LIST_FULL);
		return;
	}
	else if (nodeCount != 0)
	{
		try {
			pTop->pNext = new struct Node<T>();
		} catch (const std::bad_alloc& exc) {
			std::cerr << exc.what() << std::endl;
			throw exc;
			return;
		}	
		pTop->pNext->pPrev = pTop;
		pTop = pTop->pNext;
	}
	
	nodeCount++;	
	pTop->value = value;
	pCurrent = pTop;
	
	return;
}


template <class T> T LinkedList<T>::pop()
{
	const T tmpValue = pTop->value;

	if (nodeCount > 1)
	{
		pTop = pTop->pPrev;
		delete pTop->pNext;
		pTop->pNext = NULL;
	}
	else if (nodeCount == 0)
	{
		throw MException(MException::errCodes::ERR_EMPTY_LIST);
		return T();
	}
	
	nodeCount--;
	pCurrent = pTop;
	return tmpValue;
}


template <class T> bool LinkedList<T>::insert(T value, uint32_t index)
{
	if (nodeCount == UINT32_MAX)
	{
		throw MException(MException::errCodes::ERR_LIST_FULL);
		return false;
	}
	
	try {
		(void) this->goTo(index);
	} catch (const LinkedList<T>::MException& e) {
		std::cerr << "LinkedList(" << this << ") Error in insert() > " << e.what() << std::endl;
		return false;
	}

	Node<T>* tmp = pCurrent->pPrev;
	try {
		pCurrent->pPrev = new struct Node<T>();
	} catch (const std::bad_alloc& exc) {
		std::cerr << exc.what() << std::endl;
		throw exc;
		return false;
	}
	pCurrent->pPrev->pNext = pCurrent;
	pCurrent->pPrev->value = value;
	pCurrent->pPrev->pPrev = tmp;
	pCurrent->pPrev->pPrev->pNext = pCurrent->pPrev;
	pCurrent = pCurrent->pPrev;

	nodeCount++;
	return true;
}


template <class T> bool LinkedList<T>::replace(T value, uint32_t index)
{
	try {
		(void) this->goTo(index);
	} catch (const LinkedList<T>::MException& e) {
		std::cerr << "LinkedList(" << this << ") Error in replace() > " << e.what() << std::endl;
		return false;
	}

	pCurrent->value = value;
	return true;
}


template <class T> bool LinkedList<T>::remove(uint32_t index)
{
	try {
		(void) this->goTo(index);
	} catch (const LinkedList<T>::MException& e) {
		std::cerr << "LinkedList(" << this << ") Error in remove() > " << e.what() << std::endl;
		return false;
	}

	const Node<T>* tmpNode = pCurrent;
	pCurrent->pPrev->pNext = pCurrent->pNext;
	pCurrent->pPrev->pNext->pPrev = pCurrent->pPrev;
	pCurrent = pCurrent->pPrev;

	nodeCount--;
	delete tmpNode;
	return true;
}


template <class T> T LinkedList<T>::goTo(uint32_t index) const
{
	if (index >= nodeCount)
	{
		throw MException(MException::errCodes::INDEX_OUT_OF_RANGE);
		return T();
	}

	(void) this->goToBase();
	for (int i = 0; i < index; i++)
		(void) this->next();
	
	return pCurrent->value;
}


template <class T> T LinkedList<T>::goToBase() const
{
	pCurrent = pBase;
	return pCurrent->value;
}


template <class T> T LinkedList<T>::goToTop() const
{
	pCurrent = pTop;
	return pCurrent->value;
}


template <class T> T LinkedList<T>::next() const
{
	if (pCurrent->pNext != NULL)
	{
		pCurrent = pCurrent->pNext;
		return pCurrent->value;
	}
	
	throw MException(MException::errCodes::ERR_NO_NEXT);
	return T();
}


template <class T> T LinkedList<T>::prev() const
{
	if (pCurrent->pPrev != NULL)
	{
		pCurrent = pCurrent->pPrev;
		return pCurrent->value;
	}
	
	throw MException(MException::errCodes::ERR_NO_PREV);
	return NULL;
}


template <class T> bool LinkedList<T>::hasNext() const
{
	return (pCurrent->pNext != NULL);
}


template <class T> bool LinkedList<T>::hasPrev() const
{
	return (pCurrent->pPrev != NULL);
}


template <class T> T LinkedList<T>::getCurrent() const
{
	return pCurrent->value;
}


template <class T> uint32_t LinkedList<T>::getCount() const
{
	return nodeCount;
}


template <class T> uint32_t LinkedList<T>::getCurrentIndex() const
{
	if (nodeCount == 0)
		throw MException(MException::errCodes::ERR_EMPTY_LIST);

	const Node<T>* tmp = pCurrent;
	uint32_t index = 0;

	(void) this->goToBase();
	while (pCurrent != tmp) {
		try {
			(void) this->next();
		} catch (const LinkedList<T>::MException& e) {
			break;
		}
		index++;
	};

	return index;
}