#ifndef __linked_list_hpp
#define __linked_list_hpp


#include <stdint.h>
#include <exception>
#include <vector>


/**
 * <h1>LinkedList</h1>
 */
template <class T> class LinkedList {
private:
	/**
	 * <h1>Node</h1>
	 */
	template <class T2> struct Node {
		struct Node* pPrev;
		struct Node* pNext;	
		T2 item;
	};
	
	Node<T>* pHead;	
	Node<T>* pTail;	
	mutable Node<T>* pIndex;
	uint32_t itemCount;	

	/**
	 * Removes the item pointed to by list index.
	 * @throws ERR_EMPTY_LIST if list is empty.
	 */
	void removeAtCurrentIndex();

	/**
	 * Inserts item at the list index.
	 * @param `item` the item to insert.
	 * @throws ERR_LIST_FULL if list is full.
	 */
	void insertAtCurrentIndex(T item);

	/**
	 * Custom exception object class.
	 */
	class LinkedListExcept : public std::exception {	
	public:
		typedef const enum 
		{
			ERR_EMPTY_LIST=0,
			ERR_NO_PREV=1,
			ERR_NO_NEXT=2,
			ERR_LIST_FULL=3,
			INDEX_OUT_OF_RANGE=4
		} errCodes;
		errCodes m_error;
		
		LinkedListExcept(const errCodes error) : m_error(error) {}
	
		const char* what() const noexcept 
		{
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
	 * Dtor.
	 */
	~LinkedList();

	/**
	 * Copy Ctor.
	 * @param `obj` the LinkedList object to copy.
	 */
	LinkedList(const LinkedList<T>& obj);
	
	/**
	 * Add new item at the tail of the list and move the list index to the tail of the list.
	 * @param `item` the item to be pushed at the tail of the list.
	 * @throws ERR_LIST_FULL if node count equals UINT32_MAX.
	 */
	void push(T item);
	
	/**
	 * Remove item from the tail of the list and move the list index to the tail of the list.
	 * @return the item at the tail of the list.
	 * @throws ERR_EMPTY_LIST if list is empty.
	 */
	T pop();
	
	/**
	 * Add new item at the head of the list and move the list index to the head of the list.
	 * @param `item` the item to be inserted at the head of the list.
	 * @throws ERR_LIST_FULL if node count equals UINT32_MAX.
	 */
	void enqueue(T item);
	
	/**
	 * Remove item from the head of the list and move the list index to the head of the list.
	 * @return the item at the head of the list.
	 * @throws ERR_EMPTY_LIST if list is empty.
	 */
	T dequeue();
	
	/**
	 * Insert new item at provided index and move the list index to the new node.
	 * @param `item` the item to be inserted to the list.
	 * @param `index` the index at which to insert the item.
	 * @return true if insert was successful.
	 */
	bool insert(T item, uint32_t index);
	
	/**
	 * Remove item at provided index and move the list index to the provided index.
	 * @param `index` the index of the item to remove.
	 * @return true if remove was successful.
	 */
	bool removeAt(uint32_t index);

	/**
	 * Remove the first instance of item and move the index to the previouse node.
	 * @param `item` the item to be removed.
	 * @return true if item was found in the list or false otherwise.
	 */
	bool remove(T item);

	/**
	 * Remove all instances of item and move the index to the previouse node.
	 * @param `item` the item to be removed.
	 * @return the number of instances removed.
	 */
	uint32_t removeAll(T item);
	
	/**
	 * Replace item at provided index and move the list index to the provided index.
	 * @param `item` the new item.
	 * @param `index` the index of the item to replace.
	 * @return true if replace was successful.
	 */
	bool replace(T item, uint32_t index);

	/**
	 * Returns the last value in the list.
	 */
	T getLast() const;

	/**
	 * Returns the first value in the list.
	 */
	T getFirst() const;

	/**
	 * Go to provided index and return item.
	 * @param `index` the index of the item to get.
	 * @return the item at provided index.
	 * @throws INDEX_OUT_OF_RANGE if index greater than item count.
	 */
	T get(uint32_t index) const;

	/**
	 * Search the list for the provided item.
	 * @param `item` the item to search for in the list.
	 * @return true if item found or false if not found.
	 */
	bool contains(T item) const;

	/**
	 * Emptys the list from all items.
	 */
	void clear();

	/**
	 * Returns a vector containing all the items in the list or NULL if list is empty.
	 */
	std::vector<T>* toVector() const;
	
	/**
	 * Get the current list index.
	 */
	uint32_t getCurrIndex() const;

	/**
	 * Return the index of the next instance of item.
	 * @param `item` the item to search for.
	 * @return the index of the next instance of item or UINT32_MAX if not found.
	 */
	uint32_t nextIndexOf(T item) const;

	 /**
	  * Return the index of the previouse instance of item.
	  * @param `item` the item to search for.
	  * @return the index of the previouse instance of item or UINT32_MAX if not found.
	  */
	uint32_t prevIndexOf(T item) const;

	/**
	 * Get the index of the first instans of item.
	 * @param `item` the item to search for.
	 * @return the index of the first item instance or UINT32_MAX if not found.
	 */
	uint32_t indexOf(T item) const;

	/**
	 * Get the index of the last instans of item.
	 * @param `item` the item to search for.
	 * @return the index of the last item instance or UINT32_MAX if not found.
	 */
	uint32_t lastIndexOf(T item) const;

	/**
	 * Return a vector containing indexs of all item instances. 
	 * @param `item` the item to search for.
	 * @return a vector of uint32_t values or NULL if not found.
	 */
	std::vector<uint32_t>* allIndexsOf(T item) const;
	
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
	 * Returns true if list has previous item or false otherwise.
	 */
	bool hasPrev() const;

	/**
	 * Getter for the `itemCount` member variable.
	 */
	uint32_t count() const;
};


#endif // __linked_list_hpp
