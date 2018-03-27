#include "Generator.h"
#include "SymbolTable.h"
#include "Types.h"
#include "Utils.h"

const std::string AsmMemory::dataSizeName[] = {
	"byte", "word", "dword", "qword",
};

const std::string AsmRegister::registerName[] = {
	"eax", "ebx", "ecx", "edx", "xmm0", "xmm1", "esp", "ebp", "al", "cl", "ah", "bl", "ax",
};

const std::string AsmCommand::commandName[] = {
	"mov", "push", "pop", "add", "sub", "imul", "idiv", "cdq", "printf", "movsd",
	"and", "or", "xor", "mulsd", "addsd", "divsd", "subsd",
	"setge", "setg", "setle", "setl", "sete", "setne", "cmp", "jmp", "",
	"comisd", "ucomisd", "setbe", "setb", "seta", "setae", "jp", "jnp", "lahf", "test",
	"loop", "jnz", "jz", "inc", "dec", "jge", "jle",
	"movsx",
};

std::string AsmMemory::toString()
{
	return dataSizeName[dataSize] + " ptr [ebp - " + std::to_string(offset) + "]";
}

std::string AsmRegister::toString()
{
	if (full) {
		return AsmMemory::dataSizeName[dataSize] + " ptr [" + registerName[registerType]
			+ " - " + std::to_string(offset) + "]";
	}
	return registerName[registerType];
}

std::string AsmCode::getLabel(std::string name)
{
	return "$" + name + std::to_string(labelCnt++) + "@";
}

void AsmCode::addSymbol(PSymbol symbol)
{
	size += symbol->type->size;
	offsets[lowerString(symbol->token->text)] = size;
}

void AsmCode::push_back(AsmCommand && command)
{
	commands.push_back(command);
}

std::string AsmCode::toString()
{
	std::string result = "include c:\\masm32\\include\\masm32rt.inc\n.xmm\n.const\n";
	result += ".code\nstart:\n";
	result += "push ebp\nmov ebp, esp\nsub esp, " + std::to_string(size) + "\n";
	for (auto &command : commands)
		result += command.toString() + '\n';
	result += "mov esp, ebp\npop ebp\n";
	return result + "exit\nend start\n";
}

AsmCommand::AsmCommand(CommandType commandType, AsmRegister::RegisterType reg1)
	: commandType(commandType)
{
	push_back(std::make_shared<AsmRegister>(reg1));
}

AsmCommand::AsmCommand(CommandType commandType, AsmRegister::RegisterType reg1, AsmRegister::RegisterType reg2)
	: commandType(commandType)
{
	push_back(std::make_shared<AsmRegister>(reg1), std::make_shared<AsmRegister>(reg2));
}

AsmCommand::AsmCommand(CommandType commandType, AsmRegister::RegisterType reg, std::string value)
	: commandType(commandType)
{
	push_back(std::make_shared<AsmRegister>(reg), std::make_shared<AsmValue>(value));
}

AsmCommand::AsmCommand(CommandType commandType, std::string value)
	: commandType(commandType)
{
	push_back(std::make_shared<AsmValue>(value));
}

AsmCommand::AsmCommand(CommandType commandType, AsmMemory::DataSize dataSize, int offset)
	: commandType(commandType)
{
	push_back(std::make_shared<AsmMemory>(dataSize, offset));
}

AsmCommand::AsmCommand(CommandType commandType, AsmMemory::DataSize dataSize, int offset, AsmRegister::RegisterType reg)
	: commandType(commandType)
{
	//push_back(std::make_shared<AsmMemory>(dataSize, offset));
	push_back(std::make_shared<AsmRegister>(reg, dataSize, offset));
}

AsmCommand::AsmCommand(CommandType commandType, AsmMemory::DataSize dataSize, AsmRegister::RegisterType reg1, AsmRegister::RegisterType reg2)
	: commandType(commandType)
{
	//push_back(std::make_shared<AsmMemory>(dataSize, ))
	push_back(std::make_shared<AsmRegister>(reg1, dataSize, 0));
	push_back(std::make_shared<AsmRegister>(reg2));
}

AsmCommand::AsmCommand(CommandType commandType, AsmRegister::RegisterType reg1, AsmMemory::DataSize dataSize2, AsmRegister::RegisterType reg2, int offset2)
	: commandType(commandType)
{
	push_back(std::make_shared<AsmRegister>(reg1));
	push_back(std::make_shared<AsmRegister>(reg2, dataSize2, offset2));
}

AsmCommand::AsmCommand(CommandType commandType, AsmMemory::DataSize dataSize1, AsmRegister::RegisterType reg1, int offset1, AsmRegister::RegisterType reg2)
	: commandType(commandType)
{
	push_back(std::make_shared<AsmRegister>(reg1, dataSize1, offset1));
	push_back(std::make_shared<AsmRegister>(reg2));
}

AsmCommand::AsmCommand(CommandType commandType)
	: commandType(commandType)
{
}

void AsmCommand::push_back(std::shared_ptr<AsmParameter> par)
{
	parameters.push_back(par);
}

void AsmCommand::push_back(std::shared_ptr<AsmParameter> par1, std::shared_ptr<AsmParameter> par2)
{
	push_back(par1);
	push_back(par2);
}

std::string AsmCommand::toString()
{
	if (commandType == label) {
		return parameters[0]->toString() + ":";
	}
	std::string res = commandName[commandType] + (commandType == CommandType::printf ? "(" : " ");
	for (auto i = 0; i < parameters.size(); ++i) {
		res += parameters[i]->toString() + (i + 1 != parameters.size() ? ", " : "");
	}
	return (commandType == CommandType::printf ? res + ")" : res);
}

std::string AsmValue::toString()
{
	return value;
}
