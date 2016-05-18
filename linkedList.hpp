#ifndef __linked_list_hpp
#define __linked_list_hpp


#include <stdint.h>
#include <exception>


/**
 * <h1>LinkedList</h1>
 *
 * <p>A simple linked list library class.</p>
 */
template <class T> class LinkedList {
private:
	Node<T>* pHead;				// pointer to head of the list
	Node<T>* pTail;				// pointer to tail of the list
	mutable Node<T>* pIndex;	// list index pointer
	uint32_t itemCount;			// count of items in the list
	
	/**
	 * <h1>Node</h1>
	 * 
	 * <p>A struct representing a linked list node.</p>
	 */
	template <class T2> struct Node {
		struct Node* pPrev;		// pointer to previous node.
		struct Node* pNext;		// pointer to next node.
		T2 item;				// pointer to stored item.
	};
	
	/**
	 * Custom exception object class.
	 */
	class MException : public std::exception {	
	public:
		typedef const enum {ERR_EMPTY_LIST=0, ERR_NO_PREV=1, ERR_NO_NEXT=2, ERR_LIST_FULL=3, INDEX_OUT_OF_RANGE=4} errCodes;
		errCodes m_error;
		
		MException(const errCodes error) : m_error(error) {}
	
		const char* what() const noexcept {
			return errStrings[m_error];
		}
		
		static const char* errStrings[];
	};

public:
	/**
	 * Ctor.
	 */
	LinkedList();
	
	/**
	 * Copy Ctor.
	 * @param `obj` the LinkedList object to copy.
	 */
	LinkedList(const LinkedList& obj);
	
	/**
	 * Dtor.
	 */
	~LinkedList();
	
	/**
	 * Adds new item at the tail of the list
	 * @param `item` the item to be pushed at the tail of the list.
	 * @throws ERR_LIST_FULL if node count equals UINT32_MAX.
	 */
	void push(T item);
	
	/**
	 * Removes item from the tail of the list.
	 * @return the item at the tail of the list.
	 * @throws ERR_EMPTY_LIST if list is empty.
	 */
	T pop();
	
	/**
	 * Adds new item at the head of the list
	 * @param `item` the item to be inserted at the head of the list.
	 * @throws ERR_LIST_FULL if node count equals UINT32_MAX.
	 */
	void enqueue(T item);
	
	/**
	 * Removes item from the head of the list.
	 * @return the item at the head of the list.
	 * @throws ERR_EMPTY_LIST if list is empty.
	 */
	T dequeue();
	
	/**
	 * Inserts new item at provided index.
	 * @param `item` the item to be inserted to the list.
	 * @param `index` the index at which to insert the item.
	 * @return true if insert was successful.
	 * @throws ERR_LIST_FULL if node count equals UINT32_MAX.
	 */
	bool set(T item, uint32_t index);
	
	/**
	 * Remove item at provided index.
	 * @param `index` the index of the item to remove.
	 * @return true if remove was successful.
	 */
	bool remove(uint32_t index);
	
	/**
	 * Replaces item at provided index.
	 * @param `item` the new item.
	 * @param `index` the index of the item to replace.
	 * @return true if replace was successful.
	 */
	bool replace(T item, uint32_t index);
	
	/**
	 * Go to provided index and return item.
	 * @param `index` the index of the item to get.
	 * @return the item at provided index.
	 * @throws INDEX_OUT_OF_RANGE if index greater than node count.
	 */
	T get(uint32_t index) const;
	
	/**
	 * Go to next item.
	 * @return the next item.
	 * @throws ERR_NO_NEXT if index at tail of list.
	 */
	T next() const;
	
	/**
	 * Go to previous item.
	 * @return the previous item.
	 * @throws ERR_NO_PREV if index at head of list.
	 */
	T prev() const;
	
	/**
	 * Returns true if list has next item or false otherwise.
	 */
	bool hasNext() const;
	
	/**
	 *  Returns true if list has previous item or false otherwise.
	 */
	bool hasPrev() const;

	/**
	 * Getter for the `itemCount` variable member.
	 */
	uint32_t count() const;
};


#endif // __linked_list_hpp
