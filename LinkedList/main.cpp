#include <iostream>
#include "LinkedList.h"
int main()
{
	LinkedList<int> list;
	
	list.InsertAtEnd(20);
	list.InsertAtEnd(25);
	list.InsertAtEnd(30).InsertAtEnd(35).InsertAtEnd(40);
	list.InsertAfter(1, Node<int>(20));
	for (auto& elem : list)
	{
		std::cout << elem.Value() << '\t';
	}

	
	return 0;

}