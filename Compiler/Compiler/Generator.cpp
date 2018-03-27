#include "Generator.h"

const std::string AsmMemory::dataSizeName[] = {
	"byte", "word", "dword", "qword",
};

const std::string AsmRegister::registerName[] = {
	"eax", "ebx", "ecx", "edx", "xmm0", "xmm1", "esp", "ebp",
};

const std::string AsmCommand::commandName[] = {
	"mov", "push", "pop", "add", "sub", "mul", "div", "printf", "movsd",
	"and", "or", "xor", "mulsd", "addsd", "divsd", "subsd",
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

void AsmCode::push_back(AsmCommand && command)
{
	commands.push_back(command);
}

std::string AsmCode::toString()
{
	std::string result = "include c:\\masm32\\include\\masm32rt.inc\n.xmm\n.const\n";
	//for (const auto& it : double_const_)
	//	result += str(boost::format("__real\@%1% dq %1%r\n") % it.second);
	result += ".code\nstart:\n";
	result += "push ebp\nmov ebp, esp\nsub esp, " + std::to_string(size) + "\n";
	for (auto &command : commands)
		result += command.toString() + '\n';
	result += "mov esp, ebp\npop ebp\n";
	return result + "exit\nend start\n";
}

AsmCommand::AsmCommand(CommandType commandType, AsmRegister::RegisterType reg1, AsmRegister::RegisterType reg2)
	: commandType(commandType)
{
	push_back(std::make_shared<AsmRegister>(reg1), std::make_shared<AsmRegister>(reg2));
}

AsmCommand::AsmCommand(CommandType commandType, std::vector<std::shared_ptr<AsmParameter>>& p)
	: commandType(commandType)
{
	for (auto it : p)
		push_back(it);
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
