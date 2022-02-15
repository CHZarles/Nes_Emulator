#pragma once
#include <cstdint>

// 最常见的mirror模式
enum MIRROR
{
    HARDWARE,
    HORIZONTAL,
    VERTICAL,
    ONESCREEN_LO,
    ONESCREEN_HI,
};


// mapper 可以看成是 cpu , ppu 访问 Cartridge 的代理
// 这是一个虚基类
class Mapper
{
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper();

public:
    // 将 CPU 总线地址转换为 PRG ROM 偏移量
    virtual bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr, uint8_t& data) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0) = 0;


    // 将 PPU 总线地址转换为 CHR ROM 偏移量
    virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

    // 不是每种类型的mapper都有reset功能的, 这里为了方便扩展, 就把这个接口添上去
    virtual void reset() = 0;

    // Get Mirror mode if mapper is in control
    virtual MIRROR mirror();

    // 不是每种类型的mapper都有中断功能的, 这里为了方便扩展, 就把这个接口添上去
    // IRQ Interface 
    virtual bool irqState();
    virtual void irqClear();

    // 不是每种类型的mapper都有scanline计数功能的, 这里为了方便扩展, 就把这个接口添上去
    // Scanline Counting
    virtual void scanline();

protected:

    // 许多映射器都需要这两个变量, 这些信息可以通过读取卡带提取
    // 这两个量通过构造函数初始化
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
};
