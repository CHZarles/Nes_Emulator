#pragma once
#include <cstdint>
#include <memory>

#include "olcPixelGameEngine.h"

#include "Cartridge.h"

class PPU_2C02
{
public:
    PPU_2C02();
    ~PPU_2C02();

private:
    //一些连接到ppu的设备, 想象成内存就好
    // 下面模拟vram的内存布局
    // nes本身有能力存储2个nametable
    uint8_t tblName[2][1024];
    // 调色板, 包含了 Image palette 和 Sprite palette
    uint8_t tblPalette[32];
    /*Pattern Tables
    这里存放了 8KB 的图像数据，该区域位于卡带上，由 Mapper 管理着。
    它的作用是用来 PPU 渲染图像的时候作为参考。
    有的游戏里面这块区域是 RAM，由 CPU 写入图像数据
    */
    uint8_t tblPattern[2][4096]; // 这不是必须的，属于一个小feature，未来可用于扩展






private:
    //  Nes 支持的颜色保存z
    olc::Pixel  palScreen[0x40];
    // 代表显示器
    olc::Sprite sprScreen = olc::Sprite(256, 240);



    // 代表 name table （调试用的内部属性）
    olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
    //  代表 pattern table （调试用的内部属性）
    //  前后 4KB 共8KB
    //  这里设置为 128 是因为 16 * 8bit = 128 （16 表示有16列tile , 8 表示每个tile 8列(8bit)）
    olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };


public:
    // 用于打印各种信息的函数( 配合上面的 sprNameTable 和 sprPatternTable 调试显示 )
    olc::Sprite& GetScreen();                                 // 屏幕 （必要的）
    olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel); // 获取调色后的pixel （必要的）

	/*
    olc::Sprite& GetNameTable(uint8_t i);                     // 获取名称表 （非必要的, 属于feature）
    olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette); // 获取模式表 （非必要的, 属于feature）
	*/

    bool frame_complete = false; // 用来判断帧的绘制是否完成




// ---- 背景图像滚动相关 ----

private:

	union// ppu 的状态寄存器
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1;
		};

		uint8_t reg;
	} status; 


	union// ppu 的掩码寄存器
	{
		struct
		{
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enhance_red : 1;
			uint8_t enhance_green : 1;
			uint8_t enhance_blue : 1;
		};

		uint8_t reg;
	} mask;

	union// ppu 控制寄存器
	{
		struct
		{
			uint8_t nametable_x : 1;
			uint8_t nametable_y : 1;
			uint8_t increment_mode : 1;
			uint8_t pattern_sprite : 1;
			uint8_t pattern_background : 1;
			uint8_t sprite_size : 1;
			uint8_t slave_mode : 1; // unused
			uint8_t enable_nmi : 1;
		};

		uint8_t reg;
	} control;


	// 滚动寄存器
	union loopy_register
	{
		
		struct
		{

			uint16_t coarse_x : 5;
			uint16_t coarse_y : 5;
			uint16_t nametable_x : 1;
			uint16_t nametable_y : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};

		uint16_t reg = 0x0000;
	};


	// 指向 nametable 地址的指针v
	loopy_register vram_addr; 
	// 将要保存到指针v指向的地址的临时信息,信息记录的是滚动位置
	loopy_register tram_addr; 

	// 当前绘制像素的x坐标
	uint8_t fine_x = 0x00;

	// 地址锁存标记
	uint8_t address_latch = 0x00;
	// ppu 数据缓存
	uint8_t ppu_data_buffer = 0x00;

	// 像素“点”位置信息
	int16_t scanline = 0;
	int16_t cycle = 0;
	

	// 用于背景渲染的信息
	uint8_t bg_next_tile_id = 0x00;
	uint8_t bg_next_tile_attrib = 0x00;
	uint8_t bg_next_tile_lsb = 0x00;
	uint8_t bg_next_tile_msb = 0x00;
	// 用于渲染像素点的滚动寄存器
	uint16_t bg_shifter_pattern_lo = 0x0000;
	uint16_t bg_shifter_pattern_hi = 0x0000;
	uint16_t bg_shifter_attrib_lo = 0x0000;
	uint16_t bg_shifter_attrib_hi = 0x0000;



// ---- 精灵渲染相关 ----
private:
	// OAM是PPU内部的额外内存。它没有连接总线。它存储下一帧要被绘制的tile的信息
	struct sObjectAttributeEntry
	{
		uint8_t y;			// Y position of sprite
		uint8_t id;			// ID of tile from pattern memory
		uint8_t attribute;	// Flags define how sprite should be rendered
		uint8_t x;			// X position of sprite
	} OAM[64];

	
	// 一些辅助变量 ， 简单来说就是保存了cpu和ppu的oam通讯地址的寄存器， 对应CPU内存布局的 0x2005 的OAM_ADDR寄存
	uint8_t oam_addr = 0x00;

	// oam数组 : 存储一条 scanline 渲染的 (最多8个) 精灵
	sObjectAttributeEntry spriteScanline[8];
	uint8_t sprite_count;
	// 渲染 sprite 的移位寄存器数组
	uint8_t sprite_shifter_pattern_lo[8];
	uint8_t sprite_shifter_pattern_hi[8];

	// 精灵零命中
	bool bSpriteZeroHitPossible = false;
	bool bSpriteZeroBeingRendered = false;
	bool odd_frame = false;

	
public:
	//方便DMA使用的指针 ( DMA 的相关信息存在 bus 类里 )
	uint8_t* pOAM = (uint8_t*)OAM;



// --- PPU的系统接口 ----
public:
	// 暴露给 cpu (总线) 的接口
	uint8_t cpuRead(uint16_t addr, bool rdonly = false);
	void    cpuWrite(uint16_t addr, uint8_t  data);

	// 暴露给 ppu (总线) 的接口
	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void    ppuWrite(uint16_t addr, uint8_t data);

private:
	// 卡带
	std::shared_ptr<Cartridge> cart;

public:
	// Interface
	// 因为 ppu内含了总线. 这个方法是读取卡带信息进内存里
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();
	void reset();
	// nim 中断允许标记
	bool nmi = false;



};

