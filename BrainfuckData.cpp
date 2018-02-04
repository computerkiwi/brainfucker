#include "BrainfuckData.h"

BrainfuckData::BrainfuckData(std::istream & _inputStream, std::ostream & _outputStream) : inputStream(_inputStream), outputStream(_outputStream), pointer(0), dequeStart(0)
{
	dataDeque.push_back(0);
}

void BrainfuckData::PointerLeft(int amount)
{
	// This can be optimized later.
	if (amount > 1)
	{
		PointerLeft(amount - 1);
	}

	if (GetDequeIndex() == 0)
	{
		dataDeque.push_front(0);
		--dequeStart;
	}

	--pointer;
}

void BrainfuckData::PointerRight(int amount)
{
	// This can be optimized later.
	if (amount > 1)
	{
		PointerRight(amount - 1);
	}

	if (GetDequeIndex() == dataDeque.size() - 1)
	{
		dataDeque.push_back(0);
	}

	++pointer;
}

void BrainfuckData::Increment(int amount)
{
	GetDataReference() += amount;
}

void BrainfuckData::Decrement(int amount)
{
	GetDataReference() -= amount;
}

void BrainfuckData::Output(int amount)
{
	for (int i = 0; i < amount; ++i)
	{
		outputStream.put(GetDataReference());
	}
}

void BrainfuckData::Input(int amount)
{
	char c;
	for (int i = 0; i < amount; ++i)
	{
		c = inputStream.get();
	}
	GetDataReference() = c;
}

const BrainfuckData::DataType& BrainfuckData::Read()
{
	return GetDataReference();
}

int BrainfuckData::GetDequeIndex()
{
	return pointer + -dequeStart;
}

BrainfuckData::DataType& BrainfuckData::GetDataReference()
{
	return dataDeque[GetDequeIndex()];
}
