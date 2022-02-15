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

// ī�ж��Ⱪ¶�Ľӿ�ֻ������cpu �� ����ppu �Ľӿ�
// ī���ڲ��洢����Ϣ�� Program Memory �� Pattern Memory (�ֳ� character memory)
class Cartridge
{
public:
    explicit Cartridge(const std::string& sFileName);
    ~Cartridge();
    
public:// ��������,�ж�֡�Ƿ���Ⱦ���
    bool ImageValid();
private:
    bool bImageValid = false;

private:
    
    // �洢mirror ��ʽ ,  ��� enum ������ mapper.h 
    MIRROR hw_mirror = HORIZONTAL;

    // ��¼ nes �ļ���Ϣ
    uint8_t nMapperID = 0; // Mapper ����
    uint8_t nPRGBanks = 0; // ����� bank ��
    uint8_t nCHRBanks = 0; // CHR rom bank ��

    // �ò���������洢 progrom rom �� charatic rom
    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;


public: // Mapper ���
    std::shared_ptr<Mapper> GetMapper();
private:
    std::shared_ptr<Mapper> pMapper;

public:
    // ������ͨ��
    bool cpuRead(uint16_t addr, uint8_t& data);
    bool cpuWrite(uint16_t addr, uint8_t data);

    // ��PPUͨ������
    bool ppuRead(uint16_t addr, uint8_t& data);
    bool ppuWrite(uint16_t addr, uint8_t data);

    // ���ÿ���, ��Щ mapper ��Ҫ�����Ĺ���, ���԰�װһ��
    void reset();

    MIRROR Mirror();

};

