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

#include "badcoiq/sound/bqSoundSystem.h"

#include "bqSoundSystemInternal.h"

#include "../framework/bqFrameworkImpl.h"
extern bqFrameworkImpl* g_framework;

extern "C"
{
	bqSoundEngine* BQ_CDECL bqSoundEngine_createWaveOut();
	bqSoundEngine* BQ_CDECL bqSoundEngine_createOpenAL();
}


bqSoundSystem::bqSoundSystem()
{
	m_engines.push_back(bqSoundEngine_createWaveOut());
	m_engines.push_back(bqSoundEngine_createOpenAL());
}

bqSoundSystem::~bqSoundSystem()
{
	for (size_t i = 0; i < m_engines.m_size; ++i)
	{
		bqDestroy(m_engines.m_data[i]);
	}
}

uint32_t bqSoundSystem::GetEngineNum()
{
	return m_engines.m_size;
}

bqSoundEngine* bqSoundSystem::GetEngine(uint32_t i)
{
	return m_engines.m_data[i];
}

