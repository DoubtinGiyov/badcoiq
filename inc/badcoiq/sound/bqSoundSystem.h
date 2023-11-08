/*
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

#pragma once
#ifndef __BQ_SS_H__
#define __BQ_SS_H__

#include "bqSound.h"
#include "bqSoundEngine.h"

// реализация должна быть во фреймворке
// класс должен управлять всем что связано со звуком


namespace std
{
	class thread;
}

class bqSoundEngineObject;
class bqSoundObject
{
public:
	bqSoundObject();
	~bqSoundObject();
	BQ_PLACEMENT_ALLOCATOR(bqSoundObject);

	//uint8_t* m_buffer = 0;
	bqSoundSourceData m_sourceData;

	bqSoundEngineObject* m_engineObject = 0;
	bqSoundEngine* m_engine = 0;
};

class bqSoundSystem
{
	friend void bqSoundSystem_thread(bqSoundSystem* ss);

	bqArray<bqSoundEngine*> m_engines;

	std::thread* m_thread = 0;
	bool m_threadRun = false;
public:
	bqSoundSystem();
	~bqSoundSystem();

	uint32_t GetNumOfEngines();

	bqSoundEngine* GetEngine(uint32_t, const char*);

	bqSoundObject* SummonSoundObject(bqSoundEngine*, bqSound*);
};

#endif


