#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>

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

	AsmMemory(DataSize dataSize, int offset) :
		dataSize(dataSize), offset(offset)
	{}

	std::string toString() override;
	static const std::string dataSizeName[];
};

class AsmRegister : public AsmParameter {
public:
	enum RegisterType {
		eax, ebx, ecx, edx, xmm0, xmm1, esp, ebp, al, cl, ah, bl, ax,
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
		mov, push, pop, add, sub, imul, idiv, cdq, printf, movsd, and, or , xor, mulsd, addsd, divsd, subsd,
		setge, setg, setle, setl, sete, setne, cmp, jmp, label,
		comisd, ucomisd, setbe, setb, seta, setae, jp, jnp, lahf, test,
		loop, jnz, jz, inc, dec, jge, jle,
		movsx,
	};

	CommandType commandType;
	std::vector<std::shared_ptr<AsmParameter>> parameters;

	AsmCommand(CommandType commandType);
	AsmCommand(CommandType commandType, AsmRegister::RegisterType reg1);
	AsmCommand(CommandType commandType, AsmRegister::RegisterType reg1, AsmRegister::RegisterType reg2);
	AsmCommand(CommandType commandType, AsmRegister::RegisterType reg, std::string value);
	AsmCommand(CommandType commandType, std::string value);
	AsmCommand(CommandType commandType, AsmMemory::DataSize dataSize, int offset);

	AsmCommand(CommandType commandType, AsmMemory::DataSize dataSize, int offset, AsmRegister::RegisterType reg);
	AsmCommand(CommandType commandType, AsmMemory::DataSize dataSize, AsmRegister::RegisterType reg1, AsmRegister::RegisterType reg2);


	//AsmCommand::cmp, AsmMemory::DataSize::dword, AsmRegister::esp, AsmRegister::ebx

	void push_back(std::shared_ptr<AsmParameter> par1, std::shared_ptr<AsmParameter> par2);
	void push_back(std::shared_ptr<AsmParameter> par);

	std::string toString();
	static const std::string commandName[];
};

class Symbol;
typedef std::shared_ptr<Symbol> PSymbol;

class AsmCode {
public:
	AsmCode() {}

	int size = 0;
	std::vector<AsmCommand> commands;
	std::map<std::string, int> offsets;

	std::string getLabel(std::string name);
	void addSymbol(PSymbol symbol);
	void push_back(AsmCommand&& command);
	std::string toString();

private:
	int labelCnt = 0;
};
