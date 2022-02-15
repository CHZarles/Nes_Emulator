#pragma once
#include <stdio.h>
#include <cstdint>
#include <vector>
#include <string>


//避免循环引用
class Bus;

class Cpu_6502
{
public:
	Cpu_6502();
	~Cpu_6502();
	// 声明拷贝构造函数为 deleted 函数
	Cpu_6502(const Cpu_6502&) = delete;          
	// 声明拷贝赋值操作符为 deleted 函数
	Cpu_6502& operator = (const Cpu_6502&) = delete; 


public://寄存器 (为了方便调试和实现存档功能，设置为public)

	// 方便 状态寄存器(status register) 取/修改状态的枚举类型
	enum FLAGS6502 { 

		C = (1 << 0), //进位
		Z = (1 << 1), //零标记位
		I = (1 << 2), //中断使能
		D = (1 << 3), // 十进制位 (没啥用处)
		B = (1 << 4), // break指令标记
		U = (1 << 5), // unused
		V = (1 << 6), // 溢出
		N = (1 << 7), // 负数
	};

	// 各种寄存器
	uint8_t a = 0x00; // 累加寄存器
	uint8_t x = 0x00; // x 寄存器
	uint8_t y = 0x00; // y 寄存器
	uint8_t stkp = 0x00; // 栈顶指针寄存器
	uint16_t pc = 0x0000; // 程序计数器
	uint8_t status = 0x00; // 状态寄存器


private:// 读取修改状态寄存器的接口
	uint8_t GetFlag(FLAGS6502 f);
	void    SetFlag(FLAGS6502 f, bool v);


public:	// cpu中断引脚 

	void reset();	// 复位中断
	void irq();	    // 可屏蔽中断
	void nmi();		// 不可屏蔽中断 (实际上还是可以被ppu屏蔽的)



private: //时钟相关

	//记录"剩下"的时钟,表示 CPU 还需要多少个 clock 才能执行下一条指令
	uint8_t  cycles = 0;	   
	//统计总时钟数
	uint32_t clock_count = 0;	  

public:
	// 执行时钟动作
	void clock();

public: // 总线相关

	//连接总线
	void ConnectBus(Bus* n) { bus = n; }

private:

	//指向总线的指针
	Bus *bus = nullptr;
	// 对总线接口二次包装的cpu读写接口
	uint8_t read(uint16_t a);
	void write(uint16_t a, uint8_t d);


private: //实现指令功能相关的辅助变量

	// 表示 ALU 的输入
	uint8_t fetched = 0x00;
	// 临时全局变量
	uint16_t temp = 0x0000; 
	// 上一次访问的地址
	uint16_t addr_abs = 0x0000;
	// 遇到分支指令时,表示分支的绝对地址
	uint16_t addr_rel = 0x00;
	// 指令码
	uint8_t opcode;

public:
	//如果当前指令已完成，返回true。
	bool complete();


private: // 执行指令动作的函数

	// 取指令函数
	// 不传参，根据全局变量的信息来执行动作
	// 这个函数根据opcode对应的地址模式来决定读取数据的来源( 内存地址 / 立即字 )
	// 然后给fetched赋值
	uint8_t fetch();


	//寻址模式
	uint8_t IMP();	uint8_t IMM();
	uint8_t ZP0();	uint8_t ZPX();
	uint8_t ZPY();	uint8_t REL();
	uint8_t ABS();	uint8_t ABX();
	uint8_t ABY();	uint8_t IND();
	uint8_t IZX();	uint8_t IZY();

	//指令助记符
	uint8_t XXX();
	uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();


private: // 指令表

	// 这个结构和下面的vector用于 编译 和 存储操作码转换表。
	// 6502可以有效地拥有256个不同的指令。
	// 每一个都以（它们在表中的）数字顺序存储在一个表中，因此它们可以很容易地查找，而不需要解码。
	// 每个表条目包含:
	//		名字: 文本表示的指令(用于调试)
	//		操作码功能:一个函数指针,指向对操作码实现的函数
	//		操作码地址模式的实现:一个函数指针, 指向实现这个取址模式的函数
	//		循环计数 :一个整数，表示CPU执行指令所需要的基本时钟周期数

	struct INSTRUCTION
	{
		std::string name;
		uint8_t(Cpu_6502::* operate)(void) = nullptr;
		uint8_t(Cpu_6502::* addrmode)(void) = nullptr;
		uint8_t     cycles = 0;
	};

	std::vector<INSTRUCTION> lookup;

};


