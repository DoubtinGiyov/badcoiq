﻿/*
BSD 2-Clause License

Copyright (c) 2023, badcoiq
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "badcoiq.h"
#include "badcoiq/system/bqWindow.h"
#include "badcoiq/system/bqWindowWin32.h"

#include "bqFrameworkImpl.h"

void bqInputUpdatePre();
void bqInputUpdatePost();

bqFrameworkImpl* g_framework = 0;

class bqFrameworkDestroyer
{
public:
	bqFrameworkDestroyer() {}
	~bqFrameworkDestroyer() 
	{
		if (g_framework)
			bqFramework::Stop();
	}
};
// При закрытии программы этот объект будет уничтожен - будет вызван деструктор
//   в котором будет вызван bqFramework::Stop();
bqFrameworkDestroyer g_frameworkDestroyer;

// Выделяю память
void bqFramework::Start(bqFrameworkCallback* cb)
{
	BQ_ASSERT_ST(cb);
	BQ_ASSERT_ST(!g_framework);
	if (!g_framework)
	{
		g_framework = new bqFrameworkImpl();

#ifdef BQ_PLATFORM_WINDOWS
		RAWINPUTDEVICE device;
		device.usUsagePage = 0x01;
		device.usUsage = 0x02;
		device.dwFlags = 0;
		device.hwndTarget = 0;
		RegisterRawInputDevices(&device, 1, sizeof device);
#endif
	}
}

// Освобождаю
void bqFramework::Stop()
{
	BQ_ASSERT_ST(g_framework);
	if (g_framework)
	{
		delete g_framework;
		g_framework = 0;
	}
}

void bqFramework::Update()
{
	BQ_ASSERT_ST(g_framework);

	bqInputUpdatePre();

#ifdef BQ_PLATFORM_WINDOWS
	// без этого окно не будет реагировать
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif

	bqInputUpdatePost();

	static clock_t then = 0;
	clock_t now = clock();

	g_framework->m_deltaTime = (float)(now - then) / CLOCKS_PER_SEC;
	then = now;
}

float* bqFramework::GetDeltaTime()
{
	BQ_ASSERT_ST(g_framework);
	return &g_framework->m_deltaTime;
}

bqWindow* bqFramework::SummonWindow(bqWindowCallback* cb)
{
	BQ_ASSERT_ST(g_framework);
	BQ_ASSERT_ST(cb);
	return new bqWindow(cb);
}
