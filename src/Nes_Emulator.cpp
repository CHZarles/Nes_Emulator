
/*
	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <sstream>

#include "Bus.h"
#include "Cpu_6502.h"

#define OLC_PGE_APPLICATION //一定要加这个
#include "olcPixelGameEngine.h"



class Nes_Emulator: public olc::PixelGameEngine
{
public:
	Nes_Emulator() { sAppName = "Nes Emulator"; }

private:
	// The NES
	Bus nes;
	std::shared_ptr<Cartridge> cart;
	// 这两个变量是用来控制时间的
	bool bEmulationRun = false;
	float fResidualTime = 0.0f;
	// 控制速度
	float frequence = 60;

	

private:
	// 游戏初始化
	bool OnUserCreate()
	{
		// Load the cartridge
		cart = std::make_shared<Cartridge>("恶魔城.nes");

		if (!cart->ImageValid())
			return false;

		// Insert into NES
		nes.insertCartridge(cart);

		// Reset NES
		nes.reset();
		return true;
	}

	// 不断更新每一帧
	bool OnUserUpdate(float fElapsedTime)
	{
		Clear(olc::DARK_BLUE);

		// 设置输入
		nes.controller[0] = 0x00;
		nes.controller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;     // A Button
		nes.controller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;     // B Button
		nes.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00;     // Select
		nes.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00;     // Start
		nes.controller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;
		
		
		if (GetKey(olc:: Key :: W).bPressed){
			frequence++;
			printf("加速，当前刷新率 : 1 / %.f \n", frequence);
		}

		if (GetKey(olc::Key:: E ).bPressed) {
			frequence--;
			printf("减速 ，当前刷新率 : 1 / %.f \n", frequence);
		}
		
		

		// 暂停模拟
		if (GetKey(olc::Key::SPACE).bPressed) bEmulationRun = !bEmulationRun;
		// 重置
		if (GetKey(olc::Key::R).bPressed) nes.reset();
		

		if (bEmulationRun) // 正常情况
		{
			if (fResidualTime > 0.0f)
				fResidualTime -= fElapsedTime;
			else
			{
				// 这是控制60帧的关键
				fResidualTime += (1.0f / frequence ) - fElapsedTime;
				do { nes.clock(); } while (!nes.ppu.frame_complete);
				nes.ppu.frame_complete = false;
			}
		}
	

		DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);
		return true;
	}
};





int main()
{
	Nes_Emulator demo;
	demo.Construct(510, 480, 1, 1);
	demo.Start();
	return 0;
}
