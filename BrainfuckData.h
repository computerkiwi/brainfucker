#pragma once

#include <iostream>
#include <deque>

class BrainfuckData
{
public:
	typedef char DataType;

	BrainfuckData(std::istream& inputStream, std::ostream& outputStream);

	void PointerLeft(int amount = 1);
	void PointerRight(int amount = 1);

	void Increment(int amount = 1);
	void Decrement(int amount = 1);

	void Output(int amount = 1);
	void Input(int amount = 1);

	const DataType& Read();

private:
	int GetDequeIndex();
	DataType& GetDataReference();

	std::istream& inputStream;
	std::ostream& outputStream;

	std::deque<DataType> dataDeque;
	int pointer;
	int dequeStart;
};
