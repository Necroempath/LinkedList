#include <iostream>
#include "LinkedList.h"
int main()
{
	LinkedList<int> list;

	list.InsertAtEnd(20);
	list.InsertAtEnd(25);
	list.InsertAtEnd(30).InsertAtEnd(35).InsertAtEnd(40);

	list.InsertAfter(1, Node<int>(20));

	/*for (size_t i = 0; i < list.GetLen(); i++)
	{
		std::cout << list[i]->Value() << '\n';
	}*/
	
	return 0;

}