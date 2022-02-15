#pragma once
#include <cstdint>

// �����mirrorģʽ
enum MIRROR
{
    HARDWARE,
    HORIZONTAL,
    VERTICAL,
    ONESCREEN_LO,
    ONESCREEN_HI,
};


// mapper ���Կ����� cpu , ppu ���� Cartridge �Ĵ���
// ����һ�������
class Mapper
{
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper();

public:
    // �� CPU ���ߵ�ַת��Ϊ PRG ROM ƫ����
    virtual bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr, uint8_t& data) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0) = 0;


    // �� PPU ���ߵ�ַת��Ϊ CHR ROM ƫ����
    virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

    // ����ÿ�����͵�mapper����reset���ܵ�, ����Ϊ�˷�����չ, �Ͱ�����ӿ�����ȥ
    virtual void reset() = 0;

    // Get Mirror mode if mapper is in control
    virtual MIRROR mirror();

    // ����ÿ�����͵�mapper�����жϹ��ܵ�, ����Ϊ�˷�����չ, �Ͱ�����ӿ�����ȥ
    // IRQ Interface 
    virtual bool irqState();
    virtual void irqClear();

    // ����ÿ�����͵�mapper����scanline�������ܵ�, ����Ϊ�˷�����չ, �Ͱ�����ӿ�����ȥ
    // Scanline Counting
    virtual void scanline();

protected:

    // ���ӳ��������Ҫ����������, ��Щ��Ϣ����ͨ����ȡ������ȡ
    // ��������ͨ�����캯����ʼ��
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
};
