#pragma once
#include <string>
#include <memory>
#include <vector>

class AsmParameter {
public:
	virtual std::string toString() = 0;
};

class AsmMemory : public AsmParameter {
public:
	enum DataSize {
		byte, word, dword, qword,
	};

	DataSize dataSize;
	int offset;

	AsmMemory(DataSize dataType, int offset) :
		dataSize(dataSize), offset(offset)
	{}

	std::string toString() override;
	static const std::string dataSizeName[];
};

class AsmRegister : public AsmParameter {
public:
	enum RegisterType {
		eax, ebx, ecx, edx, xmm0, xmm1, esp, ebp,
	};

	RegisterType registerType;
	AsmMemory::DataSize dataSize;
	int offset;
	bool full;

	AsmRegister(RegisterType registerType)
		: registerType(registerType), full(false)
	{}
	AsmRegister(RegisterType registerType, AsmMemory::DataSize dataSize, int offset = 0)
		: registerType(registerType), dataSize(dataSize), offset(offset), full(true)
	{}

	std::string toString() override;
	static const std::string registerName[];
};

class AsmValue : public AsmParameter {
public:
	std::string value;
	AsmValue(std::string value)
		: value(value)
	{}

	std::string toString() override;
};

class AsmCommand {
public:
	enum CommandType {
		mov, push, pop, add, sub, mul, div, printf, movsd, and, or , xor, mulsd, addsd, divsd, subsd,
	};

	CommandType commandType;
	std::vector<std::shared_ptr<AsmParameter>> parameters;

	AsmCommand(CommandType commandType, AsmRegister::RegisterType reg1, AsmRegister::RegisterType reg2);
	AsmCommand(CommandType commandType, std::vector<std::shared_ptr<AsmParameter>> &p);
	AsmCommand(CommandType commandType, AsmRegister::RegisterType reg, std::string value);
	AsmCommand(CommandType commandType, std::string value);

	void push_back(std::shared_ptr<AsmParameter> par1, std::shared_ptr<AsmParameter> par2);
	void push_back(std::shared_ptr<AsmParameter> par);

	std::string toString();
	static const std::string commandName[];
};

class AsmCode {
public:
	AsmCode() {}

	int size = 0;
	std::vector<AsmCommand> commands;

	void push_back(AsmCommand&& command);
	std::string toString();
};
