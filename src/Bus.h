
#include <array>
#include <cstdint>
#include <memory>

#include "Cartridge.h"
#include "PPU_2C02.h"
#include "Cpu_6502.h"

class Bus
{
public:
    Bus();
    ~Bus();

public: // 总线
    // 6502 CPU
    Cpu_6502 cpu;
    // 2C02 PPU图形处理器。。。显卡。。。
    PPU_2C02 ppu;
    
    std::shared_ptr<Cartridge> cart;
    // CPU中2KB的内存RAM
    uint8_t cpuRam[2048];
    // 控制器数据暂存位置
    uint8_t controller[2];





public:
    // 总线中负责cpu的读写
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

private:
    // ppu  经过的总共周期
    uint32_t nSystemClockCounter = 0;
    // 控制寄存器
    uint8_t controller_state[2];

private:
    // 游戏精灵的传送使用DMA机制，精灵一共有64个，每个精灵大小为4个字节
    // DMA传送开始时，CPU停止clock，改为DMA传送或者读取一次数据，每个周期还是只能进行读或者写一次操作
    // dma_page和dma_addr组成一个16位的CPU地址，
    // 通过DMA机制，将CPU中的数据，一个字节一个字节的送到PPU中
    // 由于开始时需要在CPU的偶数周期进行第一次读，所以需要513或者514个周期
    uint8_t dma_page = 0x00;
    uint8_t dma_addr = 0x00;
    uint8_t dma_data = 0x00;

    // DMA传输需要精确传送。原则上需要
    // 512个周期来读取和写入256字节的OAM内存，一个
    // 先读后写。但是，CPU需要处于“偶数”状态
    // 时钟周期，因此可能需要一个虚拟的空闲周期
    bool dma_dummy = true;

    // DMA传送发生标志
    bool dma_transfer = false;

public: // 系统接口
  // 将卡带智能指针对象连接到内部总线
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    // 重置系统
    void reset();
    // 系统clock
    void clock();
};

