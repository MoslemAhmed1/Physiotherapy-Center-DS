#pragma once
#include "priNode.h"

template <typename T>
class priQueue
{
protected:
    priNode<T>* head;
    int count;

public:
    priQueue() : head(nullptr)
    {
        count = 0;
    }

    void enqueue(const T& data, int priority)
    {
        count++;
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri()) {

            newNode->setNext(head);
            head = newNode;
            return;
        }

        priNode<T>* current = head;
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext(current->getNext());
        current->setNext(newNode);
    }

    bool dequeue(T& topEntry, int& pri)
    {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        count--;
        return true;
    }

    bool peek(T& topEntry, int& pri)
    {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        return true;
    }

    bool isEmpty() const
    {
        return head == nullptr;
    }

    int getCount() const
    {
        return count;
    }

    void PrintInfo()
    {
        //Use pointer manipulation to print the queue
        if (isEmpty())
        {
            cout << endl;
            return;
        }

        priNode<Patient*>* traverse = head;
        int dummPri;

        while (traverse->getNext())
        {
            cout << *(traverse->getItem(dummPri)) << ", ";
            traverse = traverse->getNext();
        }

        cout << *(traverse->getItem(dummPri)) << endl;

        traverse = nullptr;
    }

    ~priQueue()
    {
        T tmp;
        int p;
        while (dequeue(tmp, p));
    }
};
