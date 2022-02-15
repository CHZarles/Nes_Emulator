#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include "Mapper_000.h"
#include "Mapper_002.h"
//#include "Mapper_003.h"
//#include "Mapper_066.h"

// 墨盒对外暴露的接口只有连接cpu 和 连接ppu 的接口
// 墨盒内部存储的信息有 Program Memory 和 Pattern Memory (又称 character memory)
class Cartridge
{
public:
    explicit Cartridge(const std::string& sFileName);
    ~Cartridge();
    
public:// 辅助函数,判断帧是否渲染完成
    bool ImageValid();
private:
    bool bImageValid = false;

private:
    
    // 存储mirror 方式 ,  这个 enum 定义在 mapper.h 
    MIRROR hw_mirror = HORIZONTAL;

    // 记录 nes 文件信息
    uint8_t nMapperID = 0; // Mapper 类型
    uint8_t nPRGBanks = 0; // 程序的 bank 数
    uint8_t nCHRBanks = 0; // CHR rom bank 数

    // 用不定长数组存储 progrom rom 和 charatic rom
    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;


public: // Mapper 相关
    std::shared_ptr<Mapper> GetMapper();
private:
    std::shared_ptr<Mapper> pMapper;

public:
    // 与总线通信
    bool cpuRead(uint16_t addr, uint8_t& data);
    bool cpuWrite(uint16_t addr, uint8_t data);

    // 与PPU通信总线
    bool ppuRead(uint16_t addr, uint8_t& data);
    bool ppuWrite(uint16_t addr, uint8_t data);

    // 重置卡带, 有些 mapper 需要这样的功能, 所以包装一下
    void reset();

    MIRROR Mirror();

};

