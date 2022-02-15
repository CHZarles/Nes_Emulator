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
    //һЩ���ӵ�ppu���豸, ������ڴ�ͺ�
    // ����ģ��vram���ڴ沼��
    // nes�����������洢2��nametable
    uint8_t tblName[2][1024];
    // ��ɫ��, ������ Image palette �� Sprite palette
    uint8_t tblPalette[32];
    /*Pattern Tables
    �������� 8KB ��ͼ�����ݣ�������λ�ڿ����ϣ��� Mapper �����š�
    �������������� PPU ��Ⱦͼ���ʱ����Ϊ�ο���
    �е���Ϸ������������� RAM���� CPU д��ͼ������
    */
    uint8_t tblPattern[2][4096]; // �ⲻ�Ǳ���ģ�����һ��Сfeature��δ����������չ






private:
    //  Nes ֧�ֵ���ɫ����z
    olc::Pixel  palScreen[0x40];
    // ������ʾ��
    olc::Sprite sprScreen = olc::Sprite(256, 240);



    // ���� name table �������õ��ڲ����ԣ�
    olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
    //  ���� pattern table �������õ��ڲ����ԣ�
    //  ǰ�� 4KB ��8KB
    //  ��������Ϊ 128 ����Ϊ 16 * 8bit = 128 ��16 ��ʾ��16��tile , 8 ��ʾÿ��tile 8��(8bit)��
    olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };


public:
    // ���ڴ�ӡ������Ϣ�ĺ���( �������� sprNameTable �� sprPatternTable ������ʾ )
    olc::Sprite& GetScreen();                                 // ��Ļ ����Ҫ�ģ�
    olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel); // ��ȡ��ɫ���pixel ����Ҫ�ģ�

	/*
    olc::Sprite& GetNameTable(uint8_t i);                     // ��ȡ���Ʊ� ���Ǳ�Ҫ��, ����feature��
    olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette); // ��ȡģʽ�� ���Ǳ�Ҫ��, ����feature��
	*/

    bool frame_complete = false; // �����ж�֡�Ļ����Ƿ����




// ---- ����ͼ�������� ----

private:

	union// ppu ��״̬�Ĵ���
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


	union// ppu ������Ĵ���
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

	union// ppu ���ƼĴ���
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


	// �����Ĵ���
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


	// ָ�� nametable ��ַ��ָ��v
	loopy_register vram_addr; 
	// ��Ҫ���浽ָ��vָ��ĵ�ַ����ʱ��Ϣ,��Ϣ��¼���ǹ���λ��
	loopy_register tram_addr; 

	// ��ǰ�������ص�x����
	uint8_t fine_x = 0x00;

	// ��ַ������
	uint8_t address_latch = 0x00;
	// ppu ���ݻ���
	uint8_t ppu_data_buffer = 0x00;

	// ���ء��㡱λ����Ϣ
	int16_t scanline = 0;
	int16_t cycle = 0;
	

	// ���ڱ�����Ⱦ����Ϣ
	uint8_t bg_next_tile_id = 0x00;
	uint8_t bg_next_tile_attrib = 0x00;
	uint8_t bg_next_tile_lsb = 0x00;
	uint8_t bg_next_tile_msb = 0x00;
	// ������Ⱦ���ص�Ĺ����Ĵ���
	uint16_t bg_shifter_pattern_lo = 0x0000;
	uint16_t bg_shifter_pattern_hi = 0x0000;
	uint16_t bg_shifter_attrib_lo = 0x0000;
	uint16_t bg_shifter_attrib_hi = 0x0000;



// ---- ������Ⱦ��� ----
private:
	// OAM��PPU�ڲ��Ķ����ڴ档��û���������ߡ����洢��һ֡Ҫ�����Ƶ�tile����Ϣ
	struct sObjectAttributeEntry
	{
		uint8_t y;			// Y position of sprite
		uint8_t id;			// ID of tile from pattern memory
		uint8_t attribute;	// Flags define how sprite should be rendered
		uint8_t x;			// X position of sprite
	} OAM[64];

	
	// һЩ�������� �� ����˵���Ǳ�����cpu��ppu��oamͨѶ��ַ�ļĴ����� ��ӦCPU�ڴ沼�ֵ� 0x2005 ��OAM_ADDR�Ĵ�
	uint8_t oam_addr = 0x00;

	// oam���� : �洢һ�� scanline ��Ⱦ�� (���8��) ����
	sObjectAttributeEntry spriteScanline[8];
	uint8_t sprite_count;
	// ��Ⱦ sprite ����λ�Ĵ�������
	uint8_t sprite_shifter_pattern_lo[8];
	uint8_t sprite_shifter_pattern_hi[8];

	// ����������
	bool bSpriteZeroHitPossible = false;
	bool bSpriteZeroBeingRendered = false;
	bool odd_frame = false;

	
public:
	//����DMAʹ�õ�ָ�� ( DMA �������Ϣ���� bus ���� )
	uint8_t* pOAM = (uint8_t*)OAM;



// --- PPU��ϵͳ�ӿ� ----
public:
	// ��¶�� cpu (����) �Ľӿ�
	uint8_t cpuRead(uint16_t addr, bool rdonly = false);
	void    cpuWrite(uint16_t addr, uint8_t  data);

	// ��¶�� ppu (����) �Ľӿ�
	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void    ppuWrite(uint16_t addr, uint8_t data);

private:
	// ����
	std::shared_ptr<Cartridge> cart;

public:
	// Interface
	// ��Ϊ ppu�ں�������. ��������Ƕ�ȡ������Ϣ���ڴ���
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();
	void reset();
	// nim �ж�������
	bool nmi = false;



};

