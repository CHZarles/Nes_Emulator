
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

public: // ����
    // 6502 CPU
    Cpu_6502 cpu;
    // 2C02 PPUͼ�δ������������Կ�������
    PPU_2C02 ppu;
    
    std::shared_ptr<Cartridge> cart;
    // CPU��2KB���ڴ�RAM
    uint8_t cpuRam[2048];
    // �����������ݴ�λ��
    uint8_t controller[2];





public:
    // �����и���cpu�Ķ�д
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

private:
    // ppu  �������ܹ�����
    uint32_t nSystemClockCounter = 0;
    // ���ƼĴ���
    uint8_t controller_state[2];

private:
    // ��Ϸ����Ĵ���ʹ��DMA���ƣ�����һ����64����ÿ�������СΪ4���ֽ�
    // DMA���Ϳ�ʼʱ��CPUֹͣclock����ΪDMA���ͻ��߶�ȡһ�����ݣ�ÿ�����ڻ���ֻ�ܽ��ж�����дһ�β���
    // dma_page��dma_addr���һ��16λ��CPU��ַ��
    // ͨ��DMA���ƣ���CPU�е����ݣ�һ���ֽ�һ���ֽڵ��͵�PPU��
    // ���ڿ�ʼʱ��Ҫ��CPU��ż�����ڽ��е�һ�ζ���������Ҫ513����514������
    uint8_t dma_page = 0x00;
    uint8_t dma_addr = 0x00;
    uint8_t dma_data = 0x00;

    // DMA������Ҫ��ȷ���͡�ԭ������Ҫ
    // 512����������ȡ��д��256�ֽڵ�OAM�ڴ棬һ��
    // �ȶ���д�����ǣ�CPU��Ҫ���ڡ�ż����״̬
    // ʱ�����ڣ���˿�����Ҫһ������Ŀ�������
    bool dma_dummy = true;

    // DMA���ͷ�����־
    bool dma_transfer = false;

public: // ϵͳ�ӿ�
  // ����������ָ��������ӵ��ڲ�����
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    // ����ϵͳ
    void reset();
    // ϵͳclock
    void clock();
};

