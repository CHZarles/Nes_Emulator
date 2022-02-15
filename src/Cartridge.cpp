#include "Cartridge.h"

Cartridge::Cartridge(const std::string& sFileName) {

	// NES游戏文件的文件头 一共16字节
	// ref: https://www.jianshu.com/p/994f1663475a
	//一个临时变量
	struct sHeader {
		char name[4]; // 4E 45 53 1A，对应着 ASCII 码的 'NES␚'，可以用来检测是否为 NES 文件
		uint8_t prg_rom_chunks; //一块chunk的大小为 16KB
		uint8_t chr_rom_chunks; //一块chunk大小为 8KB

		uint8_t mapper1; // flag 位
		/* mapper1 各个信息位的含义
		76543210
		||||||||
		|||||||+- Mirroring: 0: 水平镜像（PPU 章节再介绍）
		|||||||              1: 垂直镜像（PPU 章节再介绍）
		||||||+-- 1: 卡带上有没有带电池的 SRAM
		|||||+--- 1: Trainer 标志
		||||+---- 1: 4-Screen 模式（PPU 章节再介绍）
		++++----- Mapper 号的低 4 bit
		*/


		uint8_t mapper2;
		/*
		mapper2 各个位含义
		76543210
		||||||||
		|||||||+- VS Unisystem，不需要了解
		||||||+-- PlayChoice-10，不需要了解
		||||++--- 如果为 2，代表 NES 2.0 格式，不需要了解
		++++----- Mapper 号的高 4 bit
		*/
		uint8_t prg_ram_size;
		uint8_t tv_system1;
		uint8_t tv_system2;
		char unused[5];
	}header;


	bImageValid = false;

	std::ifstream ifs;
	ifs.open(sFileName, std::ifstream::binary);

	if (ifs.is_open())
	{
		// 读iNES文件头
		ifs.read(reinterpret_cast<char*>(&header), sizeof(sHeader));

		// 如果有mapper1，则调到512字节开始读取文件内容
		if (header.mapper1 & 0x04)
			ifs.seekg(512, std::ios_base::cur);

		// 读取这个iNES文件所使用的mapperid号
		nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		// 获取 mirror 类型
		hw_mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		// 这里默认是iNES文件格式，后续再添加iNES0.7 iNES2.0 // TODO
		uint8_t nFileType = 1;
		if ((header.mapper2 & 0x0C) == 0x08)
			nFileType = 2;

		if (nFileType == 0)
		{
			// TODO
		}

		// 最常见的类型
		if (nFileType == 1)
		{
			nPRGBanks = header.prg_rom_chunks;
			vPRGMemory.resize(nPRGBanks * 16384);
			ifs.read(reinterpret_cast<char*>(vPRGMemory.data()), vPRGMemory.size());

			nCHRBanks = header.chr_rom_chunks;
			if (nCHRBanks == 0)
			{
				// 默认还是要分配内存
				vCHRMemory.resize(8192);
			}
			else
			{
				// 分配character rom内存
				vCHRMemory.resize(nCHRBanks * 8192);
			}
			ifs.read(reinterpret_cast<char*>(vCHRMemory.data()), vCHRMemory.size());
		}

		// nes 2.0 属于很现代的格式了	
		// ref : https://wiki.nesdev.org/w/index.php/NES_2.0
		if (nFileType == 2)
		{
			nPRGBanks = ((header.prg_ram_size & 0x07) << 8) | header.prg_rom_chunks;
			vPRGMemory.resize(nPRGBanks * 16384);
			ifs.read(reinterpret_cast<char*>(vPRGMemory.data()), vPRGMemory.size());

			nCHRBanks = ((header.prg_ram_size & 0x38) << 8) | header.chr_rom_chunks;
			vCHRMemory.resize(nCHRBanks * 8192);
			ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
		}

		// 选择对应的映射器
		//mapper4 没用 bug太多了
		switch (nMapperID)
		{
		case 0:
			pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
			break;
		case 2:
			pMapper = std::make_shared<Mapper_002>(nPRGBanks, nCHRBanks);
			break;
			/*
		case 1:
			pMapper = std::make_shared<Mapper_001>(nPRGBanks, nCHRBanks);
			break;
		case 2:
			pMapper = std::make_shared<Mapper_002>(nPRGBanks, nCHRBanks);
			break;
		case 3:
			pMapper = std::make_shared<Mapper_003>(nPRGBanks, nCHRBanks);
			break;
		case 4:
			pMapper = std::make_shared<Mapper_004>(nPRGBanks, nCHRBanks);
			break;
		case 66:
			pMapper = std::make_shared<Mapper_066>(nPRGBanks, nCHRBanks);
			break;
		default:
			printf("Sorry we dont't the game whih Mapper %d \n", nMapperID);
			exit(0);
			*/
		}
		printf("MapperID is %d \n", nMapperID);
		bImageValid = true;
		ifs.close();
	}
}


Cartridge::~Cartridge()
{

}


bool Cartridge::ImageValid()
{
	return bImageValid;
}


// ---- 卡带暴露给 cpu 和 ppu 的接口 ----
// 下面的写法都差不多,本质上都是调用 mapper  暴露出的接口获取信息,然后再根据需求进行操作


// 给 mapper 的cpu读接口套适配一个接口
bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
{
	// 以传引用的方式获取 mapped_addr
	uint32_t mapped_addr = 0;
	// 如果地址 addr 属于 bank的地址范围 cpuMapRead 返回 true 
	if (pMapper->cpuMapRead(addr, mapped_addr, data))
	{
		if (mapped_addr == 0xFFFFFFFF) // 这个意义不明, 但看别人都是这么写的,应该说,这个地址是合法的,但是没有信息
		{
			return true; 
		}
		else
		{
			data = vPRGMemory[mapped_addr];
		}
		return true;
	}
	else
		return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	// 以传引用的方式获取 mapped_addr
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapWrite(addr, mapped_addr, data))
	{
		if (mapped_addr == 0xFFFFFFFF) //  这个意义不明, 但看别人都是这么写的,应该说,这个地址是合法的,但是没有信息
		{
			return true;
		}
		else
		{
			vPRGMemory[mapped_addr] = data;
		}
		return true;
	}
	else
		return false;
}



bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr))
	{
		data = vCHRMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapWrite(addr, mapped_addr))
	{
		vCHRMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}


// 有些卡带的mapper有重置的功能,这里套个接口
void Cartridge::reset()
{
	// 卡带重置，会重置mapper
	if (pMapper != nullptr)
		pMapper->reset();
}


// 即时获取Mirror模式

MIRROR Cartridge::Mirror()
{

	// 有的游戏的mirror模式是动态变化的(比如 Mapper_001 ),有的游戏的mirror模式是固定的
	// 不同类型的mappper有不同的模式,但接口是一样的
	
	// 调用 mapper 的接口
	MIRROR m = pMapper->mirror();
	if (m == MIRROR::HARDWARE)
	{
		// 镜像配置已定义 , 通过焊接在硬件中 ( 写死了 )
		return hw_mirror;
	}
	else
	{
		// 镜像配置即可 , 通过映射器动态设置
		return m;
	}
}


std::shared_ptr<Mapper> Cartridge::GetMapper()
{
	return pMapper;
}