#ifndef ARRAY_STACK_
#define ARRAY_STACK_

#include "StackADT.h"
#include "Definitions.h"

template<typename T>
class ArrayStack : public StackADT<T>
{

private:
	T items[MAX_SIZE];
	int top;

public:

	ArrayStack()
	{
		top = -1;
	}

	bool isEmpty() const
	{
		return top == -1;
	}

	bool push(const T& newEntry)
	{
		if (top == MAX_SIZE - 1)
			return false;	//Stack is FULL

		top++;
		items[top] = newEntry;
		return true;
	}

	bool pop(T& TopEntry)
	{
		if (isEmpty())
			return false;

		TopEntry = items[top];
		top--;
		return true;
	}

	bool peek(T& TopEntry) const
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		return true;
	}

	int getCount() const
	{
		return (top + 1);
	}

	void PrintInfo()
	{
		if (isEmpty())
		{
			cout << endl;
			return;
		}

		int trav = top;
		while (trav >= 0)
		{
			cout << *(items[trav]);
			if (trav > 0) {
				cout << ", ";
			}
			trav--;
		}
		cout << endl;
	}

};

#endif
