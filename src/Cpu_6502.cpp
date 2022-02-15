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

// ---- �жϳ��� ----

// RESET ��λ�жϣ�RESET ��ť���º����ϵͳ���ϵ�ʱ����
void  Cpu_6502:: reset()
{
	// ���� pc ���жϳ����ַ
	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	// �����ڲ��Ĵ���
	a = 0;
	x = 0;
	y = 0;
	stkp = 0xFD;
	status = 0x00 | U;

	// ���ø�������
	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	// �ж���Ҫ8������
	cycles = 8;
}


// �ж�������һ�����ӵĲ�����ֻ�е�"disable Interrupt "��־Ϊ0ʱ�Żᷢ����irq�������κ�ʱ����
// ע��Ҫ���ݿ�������״̬, �Ա��жϽ�����ָ�
// �����ɡ�RTI��ָ��ʵ�ֵ�,Ҳ������ break ָ��� ��һ��IRQ�������������ڸ�λ�ķ�ʽ����Ӳ����λ��0xFFFE��ȡ�ɱ�̵�ַ�����������Ϊ�����������
void Cpu_6502::irq()
{

	// ����ж�û������
	if (GetFlag(I) == 0)
	{
		// ��pc��ֵ��ջ
		write(0x0100 + stkp, (pc >> 8) & 0x00FF);
		stkp--;
		write(0x0100 + stkp, pc & 0x00FF);
		stkp--;

		// ������һЩ�жϱ�־λ,Ȼ��״̬�Ĵ�����ջ
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(0x0100 + stkp, status);
		stkp--;

		// ���жϱ���жϳ����ַ
		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		// IRQs ��Ҫ 7 ������
		cycles = 7;
	}
}
//���������жϲ��ܱ����ԡ������볣��IRQ�ķ�ʽ��ͬ
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



// ģ��һ�� cpu ������
void Cpu_6502::clock()
{
	// ÿ��ָ����Ҫһ���ɱ��ʱ��������ִ�С�
	// ����ʵ��Ϊģ������˵,�������ָ����ʵ�ʻ�������Ҫ���ٸ�ִ������
	// ����ģ������˵Ҳ���� "����һ������"  �Ĳ�������
	// ���Ϊ�˼��cpu�������豸��ͬ��,������Ҫ��¼ÿ��ָ��ʣ�µ�����,�õ���ʱ�ķ�ʽ���ӳ�ָ������......
	if (cycles == 0)
	{
		//��ȡ��һ��ָ���ֽڡ����8λ��ֵ��������lookup���Ի���й����ʵ��ָ��������Ϣ
		opcode = read(pc);


		// ��δʹ��״̬��־λ����Ϊ1
		SetFlag(U, true);

		//�����������������ȡ�������ֽ�
		pc++;

		//��ȡ��ʼ��ѭ����
		cycles = lookup[opcode].cycles;

		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		//Ѱַģʽ�Ͳ�����������Ӹ�ָ����������ѭ����
		cycles += (additional_cycle1 & additional_cycle2);

		// ��δʹ��״̬��־λ����Ϊ1
		SetFlag(U, true);

	}
	//����ȫ��ʱ�Ӽ���-��ʵ������û���� , ����һ��������ֱ��������
	clock_count++;

	//��ָ���ʣ��ѭ������1
	cycles--;

}


//���� 16 λ�ĵ�ַ , �����߶�  8-bit byte 
uint8_t Cpu_6502::read(uint16_t a)
{
	return bus->cpuRead(a, false);
}

void Cpu_6502::write(uint16_t a, uint8_t d)
{
	bus->cpuWrite(a, d);
}




// ---- Flag ���ߺ��� ----

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


// ---- ָ������� ---


bool Cpu_6502::complete()
{
	return cycles == 0;
}

//���������ָ��ʹ�õ�����Դ��Ϊһ���������ֵ��������Щָ���Ҫ��ȡ���ݣ���Ϊָ������������Դ��
//���磬��INX������X�Ĵ���������Ҫ��������ݡ�
//������������Ѱַģʽ������פ����addr_abs�б����λ�ã���˴������ȡ���ݡ�
uint8_t Cpu_6502::fetch()
{
	if (!(lookup[opcode].addrmode == &Cpu_6502 ::IMP))
		fetched = read(addr_abs);
	return fetched;
}


// ---- Ѱַģʽ ----



// ����Ѱַ Implied Addressing 
// ���ݳ���: 0
// ���ֽ�ָ��, ָ���Ѿ������˲�����ַ
//$AA
//TAX - (���ۼ����е�ֵ���� X �Ĵ���, �� X = A)
uint8_t Cpu_6502::IMP()
{ 
	fetched = a;
	return 0;
}

// ����Ѱַ Immediate Addressing
// ���ݳ���: 1
// ˫�ֽ�ָ��, ָ��Ĳ��������ָ����Ĳ��� ��������ַ���ǲ���������,���ǳ�Ϊ������(00-FF֮���������)
// ��6502����У�����ģʽ�Բ�����(��** ������**)ǰ�� "#" �����.
// $A9 $0A
// LDA #$0A - (���ڴ�ֵ$0A�����ۼ���, �� A = 0x0A)
uint8_t Cpu_6502::IMM()
{
	// pc ֱ�ӵ�����˳����� addr_abs
	addr_abs = pc ++;
	return 0;
}

// ��ҳѰַ Zero Page
// ���ݳ���: 1
// ˫�ֽ�ָ��, ����ַ$00-$FF��֮Ϊ��ҳ, ����Ѱַ��������ֽ�Ϊ0, ���ɱ�Ϊ��ҳѰַ, ֱ���ܹ���Լһ���ֽ�, �ٶȽϿ�, ���Ծ���ʹ�õ����ݿ��Է�����ҳ.
// $A5 $F4
// LDA $F4 - (����ַΪ$00F4��ֵ�����ۼ���, �� A = *0x00F4)
uint8_t Cpu_6502::ZP0()
{
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF; // ���ղ���������bug
	return 0;
}



// ָ�� Zero Page ��ַ���� X  Zero Page with X Offset
// ���ݳ���: 1
// ˫�ֽ�ָ��, ����Ѱַ��ʽ�ǽ�һ��8λ��ֱ�ӵ�ַ��Ϊ����ַ, Ȼ��ͱ�ַ�Ĵ���X���������, ���������������Ч��ַ
// $DD $31
// LDA $31, X - (��ֵ$31����X�Ĵ�����Ϊ��ַ��ֵ�����ۼ���, �� A = 0x31[X])
uint8_t Cpu_6502::ZPX()
{
	addr_abs = (read(pc) + x);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}


// ԭ��ͬ ZPX
uint8_t Cpu_6502::ZPY()
{
	addr_abs = (read(pc) + y);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}


//Relative	���Ѱַ
//���ݳ���: 1
//��Ѱַ����������ת��ָ��, ָ���Ϊ2���ֽ�.
//��1�ֽ�Ϊ������, ��2�ֽ�Ϊ����ת��ָ�����ת����, �ֽ�ƫ����D.ƫ���������ɸ�, D��Ϊ���ò����ʾ.
//$F0 $A7
//BEQ $A7 - (�����־λ��'Z' - ������, �������ת - 39�ֽ�, ��ǰ��39�ֽ�)
uint8_t Cpu_6502::REL()
{
	addr_rel = read(pc);
	pc++;
	// ����Ǹ���,��λȫ��1
	if (addr_rel & 0x80) // 1000 0000
		addr_rel |= 0xFF00;
	return 0;
}

// Absolute	����Ѱַ	
// ���ݳ���: 2
//���ֽ�ָ��, ָ��Ĳ������������ǲ�����, �ڴ洢���е���Ч��ַ
//$AD $F6 $31
//LDA $31F6 - (����ַΪ$31F6��ֵ�����ۼ���, �� A = [$31F6])
uint8_t Cpu_6502::ABS()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;

	return 0;
}

//Absolute,X	����Ѱַ���� X
//���ݳ���: 2
//���ֽ�ָ��, ����Ѱַ��ʽ�ǽ�һ��16λ��ֱ�ӵ�ַ��Ϊ����ַ, Ȼ��ͱ�ַ�Ĵ���X���������, ���������������Ч��ַ
//$DD $F6 $31
//LDA $31F6, X - (��ֵ$31F6����X��Ϊ��ַ��ֵ�����ۼ���, �� A = 0x31F6[X])
uint8_t Cpu_6502::ABX()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	addr_abs += x;

	// ��ҳ����,��Ҫһ����������,����1
	if ( (addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}


// ԭ��ͬ ABX
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

//Indirect	���Ѱַ
//���ݳ���: 2
//���ֽ�ָ��, �� 6502��,����������������תָ��JMP����ָ���Ѱַ��ʽ��, �������������Ǽ�ӵ�ַ, ��ӵ�ַ��ָ��Ų�������Ч��ַ�ĵ�ַ
//$6C $5F $21
//JMP($215F) - ��ת��$215F��ַ��ʼ���ֽ�ָ��ĵ�ַ
uint8_t Cpu_6502::IND()
{
	uint16_t ptr_lo = read(pc);
	pc++;
	uint16_t ptr_hi = read(pc);
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF) // ģ��һ��bug
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else // ������Ϊ
	{
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}

//���X��ַ(�ȱ�ַX����Ѱַ): Pre-indexed Indirect Addressing
//˫�ֽ�ָ��
//$A1 $3E
//LDA($3E, X)
/*
����Ѱַ��ʽ������X��Ϊ��ַ�Ĵ�������ҳ����ַIND(�������$3E)���IND+X, ���������ַ����õ���ֻ��һ����ӵ�ַ,�����뾭�����μ��Ѱַ�ŵõ���Ч��ַ:

��һ�ζ� IND + X ��ַ�õ���Ч��ַ�� 8 λ
�ڶ��ζ� IND + X + 1 ��ַ�õ���Ч��ַ�� 8 λ
Ȼ������εĽ��������,�͵õ���Ч��ַ.
*/
uint8_t Cpu_6502::IZX()
{
	uint16_t t = read(pc); // ��ʱ����
	pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}

//���Y��ַ(���ַY���Ѱַ): Post-indexed Indirect Addressing
//˫�ֽ�ָ��, �Ƚ��鷳��Ѱַ��ʽ

/*
����Ѱַ��ʽ�Ƕ�IND(�������$4C)������ָ������ҳ��ַ����һ�μ��Ѱַ, �õ�һ����8λ��ַ
�ٶ�IND + 1 ��һ�μ��Ѱַ,�õ�һ����8λ��ַ
�������,�������ֵ�ַ��������Ϊ16�Ļ���ַ,�ͼĴ���Y���б�ַ����, �õ�����������Ч��ַ,ע���������IND����ҳ��ַ
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




// ---- ָ��ʵ�� ----

// Instruction: Add with Carry In
// Function:    A = A + M + C
// Flags Out:   C, V, N, Z
uint8_t Cpu_6502::ADC()
{
	// A = A + M + C
	// ��ȡ��һ�����ݵ� fetched 
	fetch();

	//������������Ǹ�8λ����, ��������16λ����������ģ��8λ�ӷ�, �������Է���ؼ���λ, ���
	temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);


	// �������������ñ�־λ
	SetFlag(C, temp > 255);

	SetFlag(Z, (temp & 0x00FF) == 0);

	//���ֻ���������
	//����+����=�����->���
	//����+����=�����->���
	//����Ҫͬʱ�ж� a �� fetch�ǲ���ͬ��,�Լ� tmp��a/fetch �Ƿ����
	SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080); // ���
	
	SetFlag(N, temp & 0x80); 

	// ����������ۼӼĴ��� ( ȡ�� 0x00FF , ��֤����ı�ʾ��Χ�� 8 bit )
	a = temp & 0x00FF;

	// �п�����Ҫһ����������,���� 1 (�����ݴ��ʰ���. ��������Ǿ�ȷģ��)
	return 1;
}


// Instruction: Subtraction with Borrow In
// Function:    A = A - M - (1 - C)
// Flags Out:   C, V, N, Z
// ����ת��:  �Ѽ�����ɲ���ӷ�
// A = A - M - (1 - C)  = A + ( ~M + 1 ) - (1 - C ) = A + ~M + C
uint8_t Cpu_6502::SBC()
{
	fetch();

	//���ǿ�������λxor�������8λ���з�ת
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

	// ����Ķ����� ADC �Ĳ�����ȫһ��
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
