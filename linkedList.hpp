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
	/**
	 * <h1>Node</h1>
	 * 
	 * <p>A struct representing a linked list node.</p>
	 */
	template <class T2> struct Node {
		struct Node* pPrev;		// pointer to previous node.
		struct Node* pNext;		// pointer to next node.
		T2 value;				// pointer to stored value.
	};

	Node<T>* pBase;				// pointer to base of the list
	Node<T>* pTop;				// pointer to top of the list
	mutable Node<T>* pCurrent;	// pointer to current node
	uint32_t nodeCount;			// count of nodes in the list

public:
	/**
	 * Ctor.
	 */
	LinkedList();
	
	/**
	 * Copy Ctor.
	 */
	LinkedList(const LinkedList& obj);
	
	/**
	 * Dtor.
	 */
	~LinkedList();
	
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
	
	/**
	 * Adds new node at the top of the list
	 * @param the value to be pushed at the top of the list.
	 * @throws ERR_LIST_FULL if node count equals UINT32_MAX.
	 */
	void push(T value);
	
	/**
	 * Removes node at the top of the list.
	 * @return the value at the top of the list.
	 * @throws ERR_EMPTY_LIST if list is empty.
	 */
	T pop();
	
	/**
	 * Inserts new node at suplied index.
	 * @param `value` the value to be inserted to the list.
	 * @param `index` the index at which to insert the value.
	 * @return true if insert was successful.
	 * @throws ERR_LIST_FULL if node count equals UINT32_MAX.
	 */
	bool insert(T value, uint32_t index);
	
	/**
	 * Replaces value of node at suplied index.
	 * @param `value` the new value.
	 * @param `index` the index of the node to replace.
	 * @return true if replace was successful.
	 */
	bool replace(T value, uint32_t index);
	
	/**
	 * Remove node at suplied index.
	 * @param `index` the index of the node to remove.
	 * @return true if remove was successful.
	 */
	bool remove(uint32_t index);
	
	/**
	 * Go to node at suplied index.
	 * @param `index` the index of the node to go.
	 * @return the node at suplied index.
	 * @throws INDEX_OUT_OF_RANGE if index greater than node count.
	 */
	T goTo(uint32_t index) const;
	
	/**
	 * Go to the base of the list
	 * @return the value at the base of the list.
	 */
	T goToBase() const;
	
	/**
	 * Go to the top of the list
	 * @return the value at the top of the list.
	 */
	T goToTop() const;
	
	/**
	 * Go to next node.
	 * @return the next value.
	 * @throws ERR_NO_NEXT if index at top of list.
	 */
	T next() const;
	
	/**
	 * Go to previous node.
	 * @return the previous value.
	 * @throws ERR_NO_PREV if index at base of list.
	 */
	T prev() const;
	
	/**
	 * Returns true if current node has next node or false otherwise.
	 */
	bool hasNext() const;
	
	/**
	 * Returns true if current node has previous node or false otherwise.
	 */
	bool hasPrev() const;

	/**
	 * Return the current node value.
	 */
	T getCurrent() const;
	
	/**
	 * Getter for the `nodeCount` variable member.
	 */
	uint32_t getCount() const;

	/**
	 * Return the count of nodes in the list.
	 * throws ERR_EMPTY_LIST if list is empty.
	 */
	uint32_t getCurrentIndex() const;
};


#endif // __linked_list_hpp
