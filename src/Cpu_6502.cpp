#include "Cpu_6502.h"
#include "Bus.h"



Cpu_6502::Cpu_6502()
{
	using cpu = Cpu_6502;
	lookup =
	{
		{ "BRK", &cpu::BRK, &cpu::IMM, 7 },{ "ORA", &cpu::ORA, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 3 },{ "ORA", &cpu::ORA, &cpu::ZP0, 3 },{ "ASL", &cpu::ASL, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PHP", &cpu::PHP, &cpu::IMP, 3 },{ "ORA", &cpu::ORA, &cpu::IMM, 2 },{ "ASL", &cpu::ASL, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ORA", &cpu::ORA, &cpu::ABS, 4 },{ "ASL", &cpu::ASL, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BPL", &cpu::BPL, &cpu::REL, 2 },{ "ORA", &cpu::ORA, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ORA", &cpu::ORA, &cpu::ZPX, 4 },{ "ASL", &cpu::ASL, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "CLC", &cpu::CLC, &cpu::IMP, 2 },{ "ORA", &cpu::ORA, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ORA", &cpu::ORA, &cpu::ABX, 4 },{ "ASL", &cpu::ASL, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "JSR", &cpu::JSR, &cpu::ABS, 6 },{ "AND", &cpu::AND, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "BIT", &cpu::BIT, &cpu::ZP0, 3 },{ "AND", &cpu::AND, &cpu::ZP0, 3 },{ "ROL", &cpu::ROL, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PLP", &cpu::PLP, &cpu::IMP, 4 },{ "AND", &cpu::AND, &cpu::IMM, 2 },{ "ROL", &cpu::ROL, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "BIT", &cpu::BIT, &cpu::ABS, 4 },{ "AND", &cpu::AND, &cpu::ABS, 4 },{ "ROL", &cpu::ROL, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BMI", &cpu::BMI, &cpu::REL, 2 },{ "AND", &cpu::AND, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "AND", &cpu::AND, &cpu::ZPX, 4 },{ "ROL", &cpu::ROL, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "SEC", &cpu::SEC, &cpu::IMP, 2 },{ "AND", &cpu::AND, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "AND", &cpu::AND, &cpu::ABX, 4 },{ "ROL", &cpu::ROL, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "RTI", &cpu::RTI, &cpu::IMP, 6 },{ "EOR", &cpu::EOR, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 3 },{ "EOR", &cpu::EOR, &cpu::ZP0, 3 },{ "LSR", &cpu::LSR, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PHA", &cpu::PHA, &cpu::IMP, 3 },{ "EOR", &cpu::EOR, &cpu::IMM, 2 },{ "LSR", &cpu::LSR, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "JMP", &cpu::JMP, &cpu::ABS, 3 },{ "EOR", &cpu::EOR, &cpu::ABS, 4 },{ "LSR", &cpu::LSR, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BVC", &cpu::BVC, &cpu::REL, 2 },{ "EOR", &cpu::EOR, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "EOR", &cpu::EOR, &cpu::ZPX, 4 },{ "LSR", &cpu::LSR, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "CLI", &cpu::CLI, &cpu::IMP, 2 },{ "EOR", &cpu::EOR, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "EOR", &cpu::EOR, &cpu::ABX, 4 },{ "LSR", &cpu::LSR, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "RTS", &cpu::RTS, &cpu::IMP, 6 },{ "ADC", &cpu::ADC, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 3 },{ "ADC", &cpu::ADC, &cpu::ZP0, 3 },{ "ROR", &cpu::ROR, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PLA", &cpu::PLA, &cpu::IMP, 4 },{ "ADC", &cpu::ADC, &cpu::IMM, 2 },{ "ROR", &cpu::ROR, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "JMP", &cpu::JMP, &cpu::IND, 5 },{ "ADC", &cpu::ADC, &cpu::ABS, 4 },{ "ROR", &cpu::ROR, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BVS", &cpu::BVS, &cpu::REL, 2 },{ "ADC", &cpu::ADC, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ADC", &cpu::ADC, &cpu::ZPX, 4 },{ "ROR", &cpu::ROR, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "SEI", &cpu::SEI, &cpu::IMP, 2 },{ "ADC", &cpu::ADC, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ADC", &cpu::ADC, &cpu::ABX, 4 },{ "ROR", &cpu::ROR, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "STA", &cpu::STA, &cpu::IZX, 6 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "STY", &cpu::STY, &cpu::ZP0, 3 },{ "STA", &cpu::STA, &cpu::ZP0, 3 },{ "STX", &cpu::STX, &cpu::ZP0, 3 },{ "???", &cpu::XXX, &cpu::IMP, 3 },{ "DEY", &cpu::DEY, &cpu::IMP, 2 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "TXA", &cpu::TXA, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "STY", &cpu::STY, &cpu::ABS, 4 },{ "STA", &cpu::STA, &cpu::ABS, 4 },{ "STX", &cpu::STX, &cpu::ABS, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },
		{ "BCC", &cpu::BCC, &cpu::REL, 2 },{ "STA", &cpu::STA, &cpu::IZY, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "STY", &cpu::STY, &cpu::ZPX, 4 },{ "STA", &cpu::STA, &cpu::ZPX, 4 },{ "STX", &cpu::STX, &cpu::ZPY, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },{ "TYA", &cpu::TYA, &cpu::IMP, 2 },{ "STA", &cpu::STA, &cpu::ABY, 5 },{ "TXS", &cpu::TXS, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "???", &cpu::NOP, &cpu::IMP, 5 },{ "STA", &cpu::STA, &cpu::ABX, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },
		{ "LDY", &cpu::LDY, &cpu::IMM, 2 },{ "LDA", &cpu::LDA, &cpu::IZX, 6 },{ "LDX", &cpu::LDX, &cpu::IMM, 2 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "LDY", &cpu::LDY, &cpu::ZP0, 3 },{ "LDA", &cpu::LDA, &cpu::ZP0, 3 },{ "LDX", &cpu::LDX, &cpu::ZP0, 3 },{ "???", &cpu::XXX, &cpu::IMP, 3 },{ "TAY", &cpu::TAY, &cpu::IMP, 2 },{ "LDA", &cpu::LDA, &cpu::IMM, 2 },{ "TAX", &cpu::TAX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "LDY", &cpu::LDY, &cpu::ABS, 4 },{ "LDA", &cpu::LDA, &cpu::ABS, 4 },{ "LDX", &cpu::LDX, &cpu::ABS, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },
		{ "BCS", &cpu::BCS, &cpu::REL, 2 },{ "LDA", &cpu::LDA, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "LDY", &cpu::LDY, &cpu::ZPX, 4 },{ "LDA", &cpu::LDA, &cpu::ZPX, 4 },{ "LDX", &cpu::LDX, &cpu::ZPY, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },{ "CLV", &cpu::CLV, &cpu::IMP, 2 },{ "LDA", &cpu::LDA, &cpu::ABY, 4 },{ "TSX", &cpu::TSX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 4 },{ "LDY", &cpu::LDY, &cpu::ABX, 4 },{ "LDA", &cpu::LDA, &cpu::ABX, 4 },{ "LDX", &cpu::LDX, &cpu::ABY, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },
		{ "CPY", &cpu::CPY, &cpu::IMM, 2 },{ "CMP", &cpu::CMP, &cpu::IZX, 6 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "CPY", &cpu::CPY, &cpu::ZP0, 3 },{ "CMP", &cpu::CMP, &cpu::ZP0, 3 },{ "DEC", &cpu::DEC, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "INY", &cpu::INY, &cpu::IMP, 2 },{ "CMP", &cpu::CMP, &cpu::IMM, 2 },{ "DEX", &cpu::DEX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "CPY", &cpu::CPY, &cpu::ABS, 4 },{ "CMP", &cpu::CMP, &cpu::ABS, 4 },{ "DEC", &cpu::DEC, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BNE", &cpu::BNE, &cpu::REL, 2 },{ "CMP", &cpu::CMP, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "CMP", &cpu::CMP, &cpu::ZPX, 4 },{ "DEC", &cpu::DEC, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "CLD", &cpu::CLD, &cpu::IMP, 2 },{ "CMP", &cpu::CMP, &cpu::ABY, 4 },{ "NOP", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "CMP", &cpu::CMP, &cpu::ABX, 4 },{ "DEC", &cpu::DEC, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "CPX", &cpu::CPX, &cpu::IMM, 2 },{ "SBC", &cpu::SBC, &cpu::IZX, 6 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "CPX", &cpu::CPX, &cpu::ZP0, 3 },{ "SBC", &cpu::SBC, &cpu::ZP0, 3 },{ "INC", &cpu::INC, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "INX", &cpu::INX, &cpu::IMP, 2 },{ "SBC", &cpu::SBC, &cpu::IMM, 2 },{ "NOP", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::SBC, &cpu::IMP, 2 },{ "CPX", &cpu::CPX, &cpu::ABS, 4 },{ "SBC", &cpu::SBC, &cpu::ABS, 4 },{ "INC", &cpu::INC, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BEQ", &cpu::BEQ, &cpu::REL, 2 },{ "SBC", &cpu::SBC, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "SBC", &cpu::SBC, &cpu::ZPX, 4 },{ "INC", &cpu::INC, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "SED", &cpu::SED, &cpu::IMP, 2 },{ "SBC", &cpu::SBC, &cpu::ABY, 4 },{ "NOP", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "SBC", &cpu::SBC, &cpu::ABX, 4 },{ "INC", &cpu::INC, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
	};
}

Cpu_6502::~Cpu_6502()
{
}

// ---- 中断程序 ----

// RESET 复位中断，RESET 按钮按下后或者系统刚上电时产生
void  Cpu_6502:: reset()
{
	// 设置 pc 到中断程序地址
	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	// 重置内部寄存器
	a = 0;
	x = 0;
	y = 0;
	stkp = 0xFD;
	status = 0x00 | U;

	// 重置辅助变量
	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	// 中断需要8个周期
	cycles = 8;
}


// 中断请求是一个复杂的操作，只有当"disable Interrupt "标志为0时才会发生。irq可以在任何时候发生
// 注意要备份控制流的状态, 以便中断结束后恢复
// 这是由“RTI”指令实现的,也可以由 break 指令触发 。一旦IRQ发生，以类似于复位的方式，从硬编码位置0xFFFE读取可编程地址，随后将其设置为程序计数器。
void Cpu_6502::irq()
{

	// 如果中断没有屏蔽
	if (GetFlag(I) == 0)
	{
		// 把pc的值入栈
		write(0x0100 + stkp, (pc >> 8) & 0x00FF);
		stkp--;
		write(0x0100 + stkp, pc & 0x00FF);
		stkp--;

		// 先设置一些中断标志位,然后将状态寄存器入栈
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(0x0100 + stkp, status);
		stkp--;

		// 在中断表读中断程序地址
		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		// IRQs 需要 7 个周期
		cycles = 7;
	}
}
//不可屏蔽中断不能被忽略。它在与常规IRQ的方式相同
void Cpu_6502::nmi()
{
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	write(0x0100 + stkp, status);
	stkp--;

	addr_abs = 0xFFFA;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	cycles = 8;

}



// 模拟一个 cpu 的周期
void Cpu_6502::clock()
{
	// 每条指令需要一个可变的时钟周期来执行。
	// 但其实作为模拟器来说,无论这个指令在实际机器上需要多少个执行周期
	// 对于模拟器来说也就是 "调用一个函数"  的操作而已
	// 因此为了兼顾cpu和其它设备的同步,我们需要记录每个指令剩下的周期,用倒计时的方式来延迟指令的完成......
	if (cycles == 0)
	{
		//读取下一个指令字节。这个8位的值用于索引lookup表，以获得有关如何实现指令的相关信息
		opcode = read(pc);


		// 将未使用状态标志位设置为1
		SetFlag(U, true);

		//递增程序计数器，读取操作码字节
		pc++;

		//获取开始的循环数
		cycles = lookup[opcode].cycles;

		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		//寻址模式和操作码可能增加该指令完成所需的循环数
		cycles += (additional_cycle1 & additional_cycle2);

		// 将未使用状态标志位设置为1
		SetFlag(U, true);

	}
	//增加全局时钟计数-这实际上是没有用 , 它是一个方便的手表变量调试
	clock_count++;

	//此指令的剩余循环数减1
	cycles--;

}


//根据 16 位的地址 , 从总线读  8-bit byte 
uint8_t Cpu_6502::read(uint16_t a)
{
	return bus->cpuRead(a, false);
}

void Cpu_6502::write(uint16_t a, uint8_t d)
{
	bus->cpuWrite(a, d);
}




// ---- Flag 工具函数 ----

uint8_t Cpu_6502::GetFlag(FLAGS6502 f)
{
	return ((status & f) > 0) ? 1 : 0;
}


void Cpu_6502::SetFlag(FLAGS6502 f, bool v)
{
	if (v)
		status |= f;
	else
		status &= ~f;
}


// ---- 指令辅助函数 ---


bool Cpu_6502::complete()
{
	return cycles == 0;
}

//这个函数将指令使用的数据源化为一个方便的数值变量。有些指令不需要获取数据，因为指令隐含了数据源。
//例如，“INX”增加X寄存器。不需要额外的数据。
//对于所有其他寻址模式，数据驻留在addr_abs中保存的位置，因此从那里读取数据。
uint8_t Cpu_6502::fetch()
{
	if (!(lookup[opcode].addrmode == &Cpu_6502 ::IMP))
		fetched = read(addr_abs);
	return fetched;
}


// ---- 寻址模式 ----



// 隐含寻址 Implied Addressing 
// 数据长度: 0
// 单字节指令, 指令已经隐含了操作地址
//$AA
//TAX - (将累加器中的值传给 X 寄存器, 即 X = A)
uint8_t Cpu_6502::IMP()
{ 
	fetched = a;
	return 0;
}

// 立即寻址 Immediate Addressing
// 数据长度: 1
// 双字节指令, 指令的操作数部分给出的不是 操作数地址而是操作数本身,我们称为立即数(00-FF之间的任意数)
// 在6502汇编中，这种模式以操作数(即** 立即数**)前加 "#" 来标记.
// $A9 $0A
// LDA #$0A - (将内存值$0A载入累加器, 即 A = 0x0A)
uint8_t Cpu_6502::IMM()
{
	// pc 直接递增，顺便更新 addr_abs
	addr_abs = pc ++;
	return 0;
}

// 零页寻址 Zero Page
// 数据长度: 1
// 双字节指令, 将地址$00-$FF称之为零页, 绝对寻址中如果高字节为0, 即可变为零页寻址, 直接能够节约一个字节, 速度较快, 所以经常使用的数据可以放在零页.
// $A5 $F4
// LDA $F4 - (将地址为$00F4的值载入累加器, 即 A = *0x00F4)
uint8_t Cpu_6502::ZP0()
{
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF; // 保险操作，避免bug
	return 0;
}



// 指定 Zero Page 地址加上 X  Zero Page with X Offset
// 数据长度: 1
// 双字节指令, 这种寻址方式是将一个8位的直接地址作为基地址, 然后和变址寄存器X的内容相加, 结果就是真正的有效地址
// $DD $31
// LDA $31, X - (将值$31加上X寄存器作为地址的值载入累加器, 即 A = 0x31[X])
uint8_t Cpu_6502::ZPX()
{
	addr_abs = (read(pc) + x);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}


// 原理同 ZPX
uint8_t Cpu_6502::ZPY()
{
	addr_abs = (read(pc) + y);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}


//Relative	相对寻址
//数据长度: 1
//该寻址仅用于条件转移指令, 指令长度为2个字节.
//第1字节为操作码, 第2字节为条件转移指令的跳转步长, 又叫偏移量D.偏移量可正可负, D若为负用补码表示.
//$F0 $A7
//BEQ $A7 - (如果标志位中'Z' - 被设置, 则向后跳转 - 39字节, 即前跳39字节)
uint8_t Cpu_6502::REL()
{
	addr_rel = read(pc);
	pc++;
	// 如果是负数,高位全置1
	if (addr_rel & 0x80) // 1000 0000
		addr_rel |= 0xFF00;
	return 0;
}

// Absolute	绝对寻址	
// 数据长度: 2
//三字节指令, 指令的操作数给出的是操作数, 在存储器中的有效地址
//$AD $F6 $31
//LDA $31F6 - (将地址为$31F6的值载入累加器, 即 A = [$31F6])
uint8_t Cpu_6502::ABS()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;

	return 0;
}

//Absolute,X	绝对寻址加上 X
//数据长度: 2
//三字节指令, 这种寻址方式是将一个16位的直接地址作为基地址, 然后和变址寄存器X的内容相加, 结果就是真正的有效地址
//$DD $F6 $31
//LDA $31F6, X - (将值$31F6加上X作为地址的值载入累加器, 即 A = 0x31F6[X])
uint8_t Cpu_6502::ABX()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	addr_abs += x;

	// 跨页访问,需要一个额外周期,返回1
	if ( (addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}


// 原理同 ABX
uint8_t Cpu_6502::ABY()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

//Indirect	间接寻址
//数据长度: 2
//三字节指令, 在 6502中,仅仅用于无条件跳转指令JMP这条指令该寻址方式中, 操作数给出的是间接地址, 间接地址是指存放操作数有效地址的地址
//$6C $5F $21
//JMP($215F) - 跳转至$215F地址开始两字节指向的地址
uint8_t Cpu_6502::IND()
{
	uint16_t ptr_lo = read(pc);
	pc++;
	uint16_t ptr_hi = read(pc);
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF) // 模拟一个bug
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else // 正常行为
	{
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}

//间接X变址(先变址X后间接寻址): Pre-indexed Indirect Addressing
//双字节指令
//$A1 $3E
//LDA($3E, X)
/*
这种寻址方式是先以X作为变址寄存器和零页基地址IND(这里就是$3E)相加IND+X, 不过这个变址计算得到的只是一个间接地址,还必须经过两次间接寻址才得到有效地址:

第一次对 IND + X 间址得到有效地址低 8 位
第二次对 IND + X + 1 间址得到有效地址高 8 位
然后把两次的结果合起来,就得到有效地址.
*/
uint8_t Cpu_6502::IZX()
{
	uint16_t t = read(pc); // 临时变量
	pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}

//间接Y变址(后变址Y间接寻址): Post-indexed Indirect Addressing
//双字节指令, 比较麻烦的寻址方式

/*
这种寻址方式是对IND(这里就是$4C)部分所指出的零页地址先做一次间接寻址, 得到一个低8位地址
再对IND + 1 作一次间接寻址,得到一个高8位地址
最后把这高,低两部分地址合起来作为16的基地址,和寄存器Y进行变址计算, 得到操作数的有效地址,注意的是这里IND是零页地址
*/

uint8_t Cpu_6502::IZY()
{
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}




// ---- 指令实现 ----

// Instruction: Add with Carry In
// Function:    A = A + M + C
// Flags Out:   C, V, N, Z
uint8_t Cpu_6502::ADC()
{
	// A = A + M + C
	// 获取第一个数据到 fetched 
	fetch();

	//正常情况下这是个8位运算, 但我们用16位的数据类型模拟8位加法, 这样可以方便地检查进位, 溢出
	temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);


	// 根据运算结果设置标志位
	SetFlag(C, temp > 255);

	SetFlag(Z, (temp & 0x00FF) == 0);

	//溢出只有两种情况
	//正数+正数=负结果->溢出
	//负数+负数=正结果->溢出
	//所以要同时判断 a 与 fetch是不是同号,以及 tmp与a/fetch 是否异号
	SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080); // 溢出
	
	SetFlag(N, temp & 0x80); 

	// 将结果存入累加寄存器 ( 取余 0x00FF , 保证结果的表示范围是 8 bit )
	a = temp & 0x00FF;

	// 有可能需要一个额外周期,返回 1 (增加容错率罢了. 这个并不是精确模拟)
	return 1;
}


// Instruction: Subtraction with Borrow In
// Function:    A = A - M - (1 - C)
// Flags Out:   C, V, N, Z
// 运算转换:  把减法变成补码加法
// A = A - M - (1 - C)  = A + ( ~M + 1 ) - (1 - C ) = A + ~M + C
uint8_t Cpu_6502::SBC()
{
	fetch();

	//我们可以用逐位xor对下面的8位进行反转
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

	// 下面的东西和 ADC 的操作完全一样
	temp = (uint16_t)a + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, ((temp & 0x00FF) == 0));
	SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	SetFlag(N, temp & 0x0080);
	a = temp & 0x00FF;
	return 1;
}


// Instruction: Bitwise Logic AND
// Function:    A = A & M
// Flags Out:   N, Z
uint8_t Cpu_6502::AND()
{
	fetch();
	a = a & fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}


// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t Cpu_6502::ASL()
{
	fetch();
	temp = (uint16_t)fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x80);
	if (lookup[opcode].addrmode == &Cpu_6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}


// Instruction: Branch if Carry Clear
// Function:    if(C == 0) pc = address 
uint8_t Cpu_6502::BCC()
{
	if (GetFlag(C) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Carry Set
// Function:    if(C == 1) pc = address
uint8_t Cpu_6502::BCS()
{
	if (GetFlag(C) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Equal
// Function:    if(Z == 1) pc = address
uint8_t Cpu_6502::BEQ()
{
	if (GetFlag(Z) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

uint8_t Cpu_6502::BIT()
{
	fetch();
	temp = a & fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(V, fetched & (1 << 6));
	return 0;
}


// Instruction: Branch if Negative
// Function:    if(N == 1) pc = address
uint8_t Cpu_6502::BMI()
{
	if (GetFlag(N) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Not Equal
// Function:    if(Z == 0) pc = address
uint8_t Cpu_6502::BNE()
{
	if (GetFlag(Z) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Positive
// Function:    if(N == 0) pc = address
uint8_t Cpu_6502::BPL()
{
	if (GetFlag(N) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t Cpu_6502::BRK()
{
	pc++;

	SetFlag(I, 1);
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	SetFlag(B, 1);
	write(0x0100 + stkp, status);
	stkp--;
	SetFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}


// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) pc = address
uint8_t Cpu_6502::BVC()
{
	if (GetFlag(V) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Overflow Set
// Function:    if(V == 1) pc = address
uint8_t Cpu_6502::BVS()
{
	if (GetFlag(V) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


// Instruction: Clear Carry Flag
// Function:    C = 0
uint8_t Cpu_6502::CLC()
{
	SetFlag(C, false);
	return 0;
}


// Instruction: Clear Decimal Flag
// Function:    D = 0
uint8_t Cpu_6502::CLD()
{
	SetFlag(D, false);
	return 0;
}


// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t Cpu_6502::CLI()
{
	SetFlag(I, false);
	return 0;
}


// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t Cpu_6502::CLV()
{
	SetFlag(V, false);
	return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t Cpu_6502::CMP()
{
	fetch();
	temp = (uint16_t)a - (uint16_t)fetched;
	SetFlag(C, a >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 1;
}


// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t Cpu_6502::CPX()
{
	fetch();
	temp = (uint16_t)x - (uint16_t)fetched;
	SetFlag(C, x >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t Cpu_6502::CPY()
{
	fetch();
	temp = (uint16_t)y - (uint16_t)fetched;
	SetFlag(C, y >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t Cpu_6502::DEC()
{
	fetch();
	temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t Cpu_6502::DEX()
{
	x--;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}


// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t Cpu_6502::DEY()
{
	y--;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}


// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t Cpu_6502::EOR()
{
	fetch();
	a = a ^ fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}


// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t Cpu_6502::INC()
{
	fetch();
	temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t Cpu_6502::INX()
{
	x++;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}


// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t Cpu_6502::INY()
{
	y++;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}


// Instruction: Jump To Location
// Function:    pc = address
uint8_t Cpu_6502::JMP()
{
	pc = addr_abs;
	return 0;
}


// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t Cpu_6502::JSR()
{
	pc--;

	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	pc = addr_abs;
	return 0;
}


// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t Cpu_6502::LDA()
{
	fetch();
	a = fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}


// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t Cpu_6502::LDX()
{
	fetch();
	x = fetched;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 1;
}


// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t Cpu_6502::LDY()
{
	fetch();
	y = fetched;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 1;
}

uint8_t Cpu_6502::LSR()
{
	fetch();
	SetFlag(C, fetched & 0x0001);
	temp = fetched >> 1;
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &Cpu_6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t Cpu_6502::NOP()
{
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t Cpu_6502::ORA()
{
	fetch();
	a = a | fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}


// Instruction: Push Accumulator to Stack
// Function:    A -> stack
uint8_t Cpu_6502::PHA()
{
	write(0x0100 + stkp, a);
	stkp--;
	return 0;
}


// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t Cpu_6502::PHP()
{
	write(0x0100 + stkp, status | B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	stkp--;
	return 0;
}


// Instruction: Pop Accumulator off Stack
// Function:    A <- stack
// Flags Out:   N, Z
uint8_t Cpu_6502::PLA()
{
	stkp++;
	a = read(0x0100 + stkp);
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}


// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t Cpu_6502::PLP()
{
	stkp++;
	status = read(0x0100 + stkp);
	SetFlag(U, 1);
	return 0;
}

// ROL - Rotate Left
uint8_t Cpu_6502::ROL()
{
	fetch();
	temp = (uint16_t)(fetched << 1) | GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &Cpu_6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

//ROR - Rotate Right
uint8_t Cpu_6502::ROR()
{
	fetch();
	temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &Cpu_6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t Cpu_6502::RTI()
{
	stkp++;
	status = read(0x0100 + stkp);
	status &= ~B;
	status &= ~U;

	stkp++;
	pc = (uint16_t)read(0x0100 + stkp);
	stkp++;
	pc |= (uint16_t)read(0x0100 + stkp) << 8;
	return 0;
}

uint8_t Cpu_6502::RTS()
{
	stkp++;
	pc = (uint16_t)read(0x0100 + stkp);
	stkp++;
	pc |= (uint16_t)read(0x0100 + stkp) << 8;

	pc++;
	return 0;
}




// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t Cpu_6502::SEC()
{
	SetFlag(C, true);
	return 0;
}


// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t Cpu_6502::SED()
{
	SetFlag(D, true);
	return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t Cpu_6502::SEI()
{
	SetFlag(I, true);
	return 0;
}


// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t Cpu_6502::STA()
{
	write(addr_abs, a);
	return 0;
}


// Instruction: Store X Register at Address
// Function:    M = X
uint8_t Cpu_6502::STX()
{
	write(addr_abs, x);
	return 0;
}


// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t Cpu_6502::STY()
{
	write(addr_abs, y);
	return 0;
}


// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t Cpu_6502::TAX()
{
	x = a;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t Cpu_6502::TAY()
{
	y = a;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}


// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t Cpu_6502::TSX()
{
	x = stkp;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t Cpu_6502::TXA()
{
	a = x;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t Cpu_6502::TXS()
{
	stkp = x;
	return 0;
}


// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t Cpu_6502::TYA()
{
	a = y;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}


// This function captures illegal opcodes
uint8_t Cpu_6502::XXX()
{
	return 0;
}
