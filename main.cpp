#include <iostream>
#include <fstream>

#include "BrainfuckProgram.h"

int main(int argc, char* argv[])
{
	// Read a file if we're given one.
	if (argc > 1)
	{
		const char *fileName = argv[1];
		std::ifstream fileStream;
		fileStream.open(fileName);

		BrainfuckProgram program(fileStream);
		program.Run(std::cin, std::cout);
	}
	else
	{
		BrainfuckProgram program(std::cin);
		program.Run(std::cin, std::cout);
	}

	int x = 5;
}