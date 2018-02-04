#pragma once

#include <iostream>
#include <vector>

namespace BrainfuckProgramInternal
{
	enum BrainfuckInstructionType
	{
		BFI_POINTER_LEFT,
		BFI_POINTER_RIGHT,
		BFI_INCREMENT,
		BFI_DECREMENT,
		BFI_INPUT,
		BFI_OUTPUT,
		BFI_ZERO_JUMP,
		BFI_NONZERO_JUMP
	};

	struct BrainfuckInstruction
	{
		BrainfuckInstructionType type;
		union
		{
			int jumpInstructionPos;
			int repeatAmount;
		};
	};
}

class BrainfuckProgram
{
public:
	// Parses an input stream.
	BrainfuckProgram(std::istream& inputStream);

	void Run(std::istream& inputStream, std::ostream& outputStream);

private:
	std::vector<BrainfuckProgramInternal::BrainfuckInstruction> instructionStack;

	void CollapseInstructionTop();
};
