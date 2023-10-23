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
#include "badcoiq/math/bqMath.h"

void bqSound_Generate_sin(bqSoundSource* , float t, uint32_t f, float l);
void bqSound_Generate_square(bqSoundSource*, float t, uint32_t f, float l);
void bqSound_Generate_triangle(bqSoundSource*, float t, uint32_t f, float l);
void bqSound_Generate_saw(bqSoundSource*, float t, uint32_t f, float l);

bqSoundSource::bqSoundSource()
{
}

bqSoundSource::~bqSoundSource()
{
	if (m_data)
	{
		bqMemory::free(m_data);
		m_data = 0;
	}
}

bqSound::bqSound()
{
}

bqSound::~bqSound()
{
	Clear();
}

void bqSound::Clear()
{
	if (m_soundSource)
	{
		delete m_soundSource;
	m_soundSource = 0;
	}
}

// 
bqSoundSource* bqSound_createNew(float t)
{
	bqSoundSource* newSound = new bqSoundSource;
	newSound->m_channels = 1;
	newSound->m_sampleRate = 44100;

	//	надо вычислить размер в соответствии с временем 
	newSound->m_sampleRate;

	newSound->m_size = time * 2 * newSound->m_sampleRate;


	newSound->m_data = (uint8_t*)bqMemory::malloc(newSound->m_size);

	return newSound;
}

void bqSound::Generate(
	bqSoundWaveType waveType,
	float time, 
	uint32_t frequency, 
	float loudness)
{
	Clear();
	
	if (time > 0.f)
	{
		m_soundSource = bqSound_createNew(time);

		switch (waveType)
		{
		default:
		case bqSoundWaveType::sin:
			bqSound_Generate_sin(m_soundSource, time, frequency, loudness);
			break;
		case bqSoundWaveType::square:
			bqSound_Generate_square(m_soundSource, time, frequency, loudness);
			break;
		case bqSoundWaveType::triangle:
			bqSound_Generate_triangle(m_soundSource, time, frequency, loudness);
			break;
		case bqSoundWaveType::saw:
			bqSound_Generate_saw(m_soundSource, time, frequency, loudness);
			break;
		}
	}

	/*if (!m_soundSource)
	{
		int time = 1;

		m_soundSource = new bqSoundSource;
		m_soundSource->m_channels = 1;
		m_soundSource->m_sampleRate = 44100;
		m_soundSource->m_size = time * 2 * m_soundSource->m_sampleRate;
		m_soundSource->m_data = (uint8_t*)bqMemory::malloc(m_soundSource->m_size);

		union _short
		{
			int8_t _8[2];
			int16_t _16;
		};

		// Объяснение:
		//   Значение sample rate 44100 это сколько герц проигрывается за 1 секунду.
		//                                         сэмплов
		// 
		//   Чтобы проиграть звук определённой частоты, делим 44100 на эту частоту.
		//   Получается значение, типа, одна волна проходит на столько-то единиц в 44100 значении
		//                                          использует столько-то сэмплов
		//   То есть, 
		//      Hz = 440
		//      hz = 44100 / Hz = 100.2272727
		//      Синусоида должна сделать круг типа за 100 шагов.
		//   Но шаги циклом не делаем, а вычисляем angleStep, и просто 
		//   делаем приращение угла на это значение.
		//   
		//   Нам известно что из 44100 на создание одной волны нужно 100 сэмплов (при Hz = 441)
		//   Получаем значение которым будет прибавлять к углу
		//      angleStep = 360 градусов / hz
		//   
		//   Данный код надо будет задокументировать и оставить чтобы не потерялся.
		for (int index = 0, second = 0; second < time; second++)
		{
			float Hz = 365.f;
			float hz = 44100.f / Hz;
			printf("hz %f\n", hz);

			float angle = 0.f;
			float angleStep = 360.f / hz;
			printf("angleStep %f\n", angleStep);


			for (int cycle = 0; cycle < 44100; cycle++)
			{
				auto rad = bqMath::DegToRad(angle);
				auto sn = sin(rad);

				angle += angleStep;
				if (angle > 360.f)
				{
					angle = 0.f;
				}

				_short v;

				// амплитуда\громкость.
				// дефолтное значение 32767.f. можно считать максимальным значением.
				//  выше уже типа перегруз.
				v._16 = (int16_t)(sn * 32767.f);
			//		printf("%f %f %f %i\n", angle, rad, sn, v._16);

				m_soundSource->m_data[index++] = v._8[0];
				m_soundSource->m_data[index++] = v._8[1];
			}
		}
	}
*/
}

void bqSound_Generate_sin(bqSoundSource* ss, float t, uint32_t f, float l)
{
}

void bqSound_Generate_square(bqSoundSource* ss, float t, uint32_t f, float l)
{
}

void bqSound_Generate_triangle(bqSoundSource* ss, float t, uint32_t f, float l)
{
}

void bqSound_Generate_saw(bqSoundSource* ss, float t, uint32_t f, float l)
{
}
