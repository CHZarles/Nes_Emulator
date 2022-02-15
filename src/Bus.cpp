
#include "Bus.h"

Bus::Bus()
{
    // ��nes�������ʱ������cpu��bus
    cpu.ConnectBus(this);
}

Bus::~Bus()
{
}


void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
    if (cart->cpuWrite(addr, data))
    {
        // cartridge�����״��ж�
        // cartridge���Ը�����Ҫ������ӳ�䵽�����κε�ַ
        // ͨ�����ַ�ʽ��������չӲ�����裬����ǹ��
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        // ϵͳ�ڴ��ַ
        // ֻ��һ��8kb������ֻ��2kb������
        // ������ַ��2kb�ľ���ʹ��&0x07ff���㣬ֻʹ��2kb
        cpuRam[addr & 0x07FF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        // PPU��ַ
        // ppuֻ��8���Ĵ�������0x2000 -
        // 0x3fff�����Χ�ڣ�������8���Ĵ������ظ���������
        // ����ֱ��ʹ��&0x0007��ȡ��ȷ��ַ����
        ppu.cpuWrite(addr & 0x0007, data);
    }
    else if ((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017)
    {
        // APU��ַ
        // apu�ļ����Ĵ�����ַ 0x4000 - 0x4013 0x4015 0x4017
       
    }
    else if (addr == 0x4014)
    {
        // 0x4014��ַ��DMA���Ϳ�ʼ��־
        // ���˵�ַ��д������ʱ��˵������DMA����
        // CPU���ڽ�ֹͣ����OAM�Ĵ����е����ݴ��͵�PPU�У�64�����飬ֱ�����ͽ���
        dma_page = data;
        dma_addr = 0x00;
        dma_transfer = true;
    }
    else if (addr >= 0x4016 && addr <= 0x4017)
    {
        // ���ƼĴ�����ַ
        // 0x4016 - 0x4017�ǿ��ƼĴ�����ַ��cpuͨ����ȡ�˵�ַ�е�����
        // ��ȡ��������
        controller_state[addr & 0x0001] = controller[addr & 0x0001];
    }
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;
    if (cart->cpuRead(addr, data))
    {
        // Cartridge ��ַ
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        // ϵͳ�ڴ��ַRAM, ÿ2kb���Ǿ���  2kb = 0x07FF
        data = cpuRam[addr & 0x07FF];
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        // PPU��ַ ÿ8���ֽڶ��Ǿ�������
        data = ppu.cpuRead(addr & 0x0007, bReadOnly);
    }
    else if (addr == 0x4015)
    {
        // APU���Ĵ���
       
    }
    else if (addr >= 0x4016 && addr <= 0x4017)
    {
        // ��ȡ������������
        data = (controller_state[addr & 0x0001] & 0x80) > 0;
        controller_state[addr & 0x0001] <<= 1;
    }

    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    // ���뿨����
    // ���������ӵ�bus��ppu
    this->cart = cartridge;
    ppu.ConnectCartridge(cartridge);
}

void Bus::reset()
{
    cart->reset();
    cpu.reset();
    ppu.reset();
    nSystemClockCounter = 0;
    dma_page = 0x00;
    dma_addr = 0x00;
    dma_data = 0x00;
    dma_dummy = true;
    dma_transfer = false;
}
void Bus::clock()
{
    // ʱ�ӣ� ģ�����ĺ���
    // ����ģ����������"����"
    // ���У�ppuʱ��ͨ��������������ģ�����ٶ�
    ppu.clock();



    // CPU��ʱ�����ڱ�PPU��APU����3����Ҳ����˵��PPU��APUÿִ��3�Σ�CPUִ��һ��
    // ����NESģ�����й涨��
    // ʹ��nSystemClockCounter����¼ppu��ʱ������. ��3ȡ�༴�ɵõ�cpuʱ��
    if (nSystemClockCounter % 3 == 0)
    {
        // �ж��Ƿ���DMA����(��OAM���ݴ��͵�PPU��)
        if (dma_transfer)
        {
            // TODO(tiansongyu): ���ܴ���cpu������bug
            // ������Ȼ��cpu�������У���ʵ��cpu������clock��
            // ��ʱDMAռ��cpu����
            // ͨ��ռ��513��514������
            // ���ȴ�д�����ʱΪ 1 ���ȴ�״̬���ڣ���������� CPU ������Ϊ + 1����Ϊ
            // 256 ������� / д���ڡ��� dma_dummy��Ĭ����true
            if (dma_dummy)
            {
                // ִ�����ڱ�����ż������Ҫ�ȴ�һ��cpu����
                if (nSystemClockCounter % 2 == 1)
                {
                    // DMA��ʼ����
                    dma_dummy = false;
                }
            }
            else
            {
                if (nSystemClockCounter % 2 == 0)
                {
                    // ż�����ڴ�cpu�ж�ȡ���ݣ�
                    // dma_page�洢�� 0x(dma_page)xx����
                    // xxͨ����0��ʼ ��0xFF������
                    // 64�����飬ÿ������4���ֽڣ�������256���ֽڣ�Ҳ����0xFF��С
                    // dma_page�ǵ�ַ�ĸ�λ��dma_addr�ǵ�ַ�ĵ�λ
                    // dma_data��Ϊ��cpu�ж�ȡ������
                    dma_data = cpuRead(dma_page << 8 | dma_addr);
                }
                else
                {
                    // �������ڣ�������д��PPU��
                    ppu.pOAM[dma_addr] = dma_data;
                    // ��Ҫ����dma�ĵ�ַƫ��
                    dma_addr++;
                    // dma_addr��һ���ֽڵ�����
                    // ��dma_addr����0xFFʱ����������
                    // dma_addr = 0 ˵����ʱDMA����ֹͣ
                    // ����־�Ĵ�������
                    if (dma_addr == 0x00)
                    {
                        dma_transfer = false;
                        dma_dummy = true;
                    }
                }
            }
        }
        else
        {
            // û�з���DMA����ʱ��CPUʱ���������С�
            cpu.clock();
        }
    }

   
    if (ppu.nmi)
    {
        // PPU���ж�ͨ��������vertical blanking period����ֱ�����ڼ�
        // Ҳ����scanline����251?? ��ʱ����������Ļ���·���
        // ��ʱcpu��Ҫ������һ֡�����Ʊ��׼��
        // �������ڻ��Ƶ�ʱ�����׼����ֻ�ܵȵ����ƽ��������������ܶ�����
        // Ψһ�Ļ�������ڴ�ֱ�����ڼ���У�Ҳ��nes�������֮��
        ppu.nmi = false;
        cpu.nmi();
    }

    // Check if cartridge is requesting IRQ
    if (cart->GetMapper()->irqState())
    {
        cart->GetMapper()->irqClear();
        cpu.irq();
    }

    nSystemClockCounter++;
    
}
