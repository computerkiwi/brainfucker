#include "BrainfuckProgram.h"
#include "BrainfuckData.h"

using namespace BrainfuckProgramInternal;

static bool collapseCommands = true;

BrainfuckProgram::BrainfuckProgram(std::istream & inputStream)
{
	std::vector<int> loopStartStack;

	while (!inputStream.eof())
	{
		// Get a char and add the corresponding instruction.
		char c = inputStream.get();
		switch (c)
		{
			case ('>'):
			{
				instructionStack.push_back({ BFI_POINTER_RIGHT, 1 });
			}
			break;

			case ('<'):
			{
				instructionStack.push_back({ BFI_POINTER_LEFT, 1 });
			}
			break;

			case ('+'):
			{
				instructionStack.push_back({ BFI_INCREMENT, 1 });
			}
			break;

			case ('-'):
			{
				instructionStack.push_back({ BFI_DECREMENT, 1 });
			}
			break;

			case ('.'):
			{
				instructionStack.push_back({ BFI_OUTPUT, 1 });
			}
			break;

			case (','):
			{
				instructionStack.push_back({ BFI_INPUT, 1 });
			}
			break;

			case ('['):
			{
				// Add a zero jump instruction (we'll fill in where to jump to on the closing bracket) and save where the closing bracket will jump to.
				instructionStack.push_back({ BFI_ZERO_JUMP });
				loopStartStack.push_back(instructionStack.size());
			}
			break;

			case (']'):
			{
				// If there's no loops to close there's a mismatch.
				if (loopStartStack.size() == 0)
				{
					throw "Mismatched closing bracket.";
				}

				// Add an instruction to jump to the to opening bracket on the stack and update the original instruction's target to here.
				instructionStack.push_back({ BFI_NONZERO_JUMP, loopStartStack.back() });
				instructionStack[loopStartStack.back() - 1].jumpInstructionPos = instructionStack.size();
				loopStartStack.pop_back();
			}
			break;

			default:
			{
				// Counted as comment character. Do nothing.
			}
			break;
		}

		// Try to compress the top instruction.
		if (collapseCommands)
		{
			CollapseInstructionTop();
		}
	}

	// If we have loops we haven't closed we have a mismatch.
	if (loopStartStack.size() != 0)
	{
		throw "Mismatched opening bracket.";
	}
}

void BrainfuckProgram::Run(std::istream & inputStream, std::ostream & outputStream)
{
	BrainfuckData data(inputStream, outputStream);
	size_t instructionPtr = 0;

	while (instructionPtr < instructionStack.size())
	{
		// Get the instruction at the instruction ptr.
		const BrainfuckInstruction& instruction = instructionStack[instructionPtr];

		// Keep track of wether we jump to avoid incrementing instruction ptr and skipping the jump point.
		bool jumped = false;

		// Do stuff based on the instruction type.
		switch (instruction.type)
		{
		case (BFI_POINTER_LEFT):
		{
			data.PointerLeft(instruction.repeatAmount);
		}
		break;

		case (BFI_POINTER_RIGHT):
		{
			data.PointerRight(instruction.repeatAmount);
		}
		break;

		case (BFI_INCREMENT):
		{
			data.Increment(instruction.repeatAmount);
		}
		break;

		case (BFI_DECREMENT):
		{
			data.Decrement(instruction.repeatAmount);
		}
		break;

		case (BFI_INPUT):
		{
			data.Input(instruction.repeatAmount);
		}
		break;

		case (BFI_OUTPUT):
		{
			data.Output(instruction.repeatAmount);
		}
		break;

		case (BFI_NONZERO_JUMP):
		{
			BrainfuckData::DataType value = data.Read();
			if (value)
			{
				instructionPtr = instruction.jumpInstructionPos;
				jumped = true;
			}
		}
		break;

		case (BFI_ZERO_JUMP):
		{
			BrainfuckData::DataType value = data.Read();
			if (!value)
			{
				instructionPtr = instruction.jumpInstructionPos;
				jumped = true;
			}
		}
		break;

		}

		if (!jumped)
		{
			++instructionPtr;
		}
	}
}

void BrainfuckProgram::CollapseInstructionTop()
{
	// Compress while the top 2 instructions are the same non-jump type.
	while (instructionStack.size() > 1 && instructionStack.back().type != BFI_NONZERO_JUMP && instructionStack.back().type != BFI_ZERO_JUMP 
		     && instructionStack[instructionStack.size() - 1].type == instructionStack[instructionStack.size() - 2].type)
	{
		instructionStack[instructionStack.size() - 2].repeatAmount += instructionStack.back().repeatAmount;
		instructionStack.pop_back();
	}
}
