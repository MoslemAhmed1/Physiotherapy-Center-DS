
#ifndef LINKED_QUEUE_
#define LINKED_QUEUE_

#include "Node.h"
#include "QueueADT.h"
#include <iostream>
using namespace std;

template <typename T>
class LinkedQueue :public QueueADT<T>
{
protected:

	int count;
	Node<T>* backPtr;
	Node<T>* frontPtr;
public:

	LinkedQueue();

	bool isEmpty() const;

	bool enqueue(const T& newEntry);

	bool dequeue(T& frntEntry);

	bool peek(T& frntEntry)  const;

	int getCount() const;

	virtual void PrintInfo();

	~LinkedQueue();

	//copy constructor
	LinkedQueue(const LinkedQueue<T>& LQ);
};
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
LinkedQueue<T>::LinkedQueue()
{
	count = 0;
	backPtr = nullptr;
	frontPtr = nullptr;
}
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool LinkedQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool LinkedQueue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	if (isEmpty())
		frontPtr = newNodePtr;
	else
		backPtr->setNext(newNodePtr);

	backPtr = newNodePtr;
	count++;
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool LinkedQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();

	if (nodeToDeletePtr == backPtr)
		backPtr = nullptr;

	delete nodeToDeletePtr;
	count--;
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool LinkedQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
LinkedQueue<T>::~LinkedQueue()
{
	T temp;
	while (dequeue(temp));
	frontPtr = backPtr = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void LinkedQueue<T>::PrintInfo()
{
	if (isEmpty())
	{
		cout << endl;
		return;
	}

	Node<T>* traverse = frontPtr;
	int itemsShown = 0;
	const int maxItems = 10;

	// Print up to 10 items
	while (traverse && itemsShown < maxItems)
	{
		// Properly dereference the Patient pointer before using operator<<
		cout << *(traverse->getItem());

		// Add comma unless it's the last item or we've reached maximum
		if (traverse->getNext() && itemsShown < maxItems - 1)
			cout << ", ";

		traverse = traverse->getNext();
		itemsShown++;
	}

	// If there are more items beyond what we've shown, print "..."
	if (traverse)
		cout << "...";

	cout << endl;
}
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& LQ)
{
	frontPtr = backPtr = nullptr;
	Node<T>* NodePtr = LQ.frontPtr;
	while (NodePtr)
	{
		enqueue(NodePtr->getItem());
		NodePtr = NodePtr->getNext();
	}
}

template <typename T>
int LinkedQueue<T>::getCount() const
{
	return count;
}

#endif