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
#include "badcoiq/common/bqFileBuffer.h"

//class bqSoundBlock
//{
//	float * m_data = 0;
//	uint32_t m_channels = 1;
//public:
//	bqSoundBlock(uint32_t channels)
//	{
//		if (channels == 0)
//			channels = 1;
//		m_channels = channels;
//
//		m_data = (float*)bqMemory::calloc(sizeof(float) * channels);
//	}
//
//	~bqSoundBlock()
//	{
//		if (m_data)
//			bqMemory::free(m_data);
//	}
//
//	void MakeZero()
//	{
//		for (uint32_t i = 0; i < m_channels; ++i)
//		{
//			m_data[i] = 0;
//		}
//	}
//
//	void FromUint8(uint8_t* other, uint32_t ch)
//	{
//	}
//};

template <typename _type>
class bqSoundSample
{
public:
	_type m_data;

	using sample_type = _type;
};

#include "badcoiq/common/bqPack.h"
struct bqSoundSample24Base { int8_t m_byte[3]; };
#include "badcoiq/common/bqUnpack.h"

using bqSoundSample_8bit = bqSoundSample<uint8_t>;
using bqSoundSample_16bit = bqSoundSample<int16_t>;
using bqSoundSample_24bit = bqSoundSample<bqSoundSample24Base>;
using bqSoundSample_32bit = bqSoundSample<int32_t>;
using bqSoundSample_32bitFloat = bqSoundSample<bqFloat32>;
using bqSoundSample_64bitFloat = bqSoundSample<bqFloat64>;

class bqSoundLab
{
public:
	bqSoundLab() {}
	~bqSoundLab() {}

	// Все типы сэмплов должны уметь конвертироваться в 32bitFloat
	static bqSoundSample_32bitFloat ConvertSample_32bitFloat(bqSoundSample_8bit);
	static bqSoundSample_32bitFloat ConvertSample_32bitFloat(bqSoundSample_16bit);
//	static bqSoundSample_32bitFloat ConvertSample_32bitFloat(bqSoundSample_24bit);
//	static bqSoundSample_32bitFloat ConvertSample_32bitFloat(bqSoundSample_32bit);
//	static bqSoundSample_32bitFloat ConvertSample_32bitFloat(bqSoundSample_64bitFloat);

	// 32bitFloat должен быть сконвертирован в другие форматы
	static bqSoundSample_8bit ConvertSample_8bit(bqSoundSample_32bitFloat);
	static bqSoundSample_16bit ConvertSample_16bit(bqSoundSample_32bitFloat);
	//static bqSoundSample_24bit ConvertSample_24bit(bqSoundSample_32bitFloat);
	//static bqSoundSample_32bit ConvertSample_32bit(bqSoundSample_32bitFloat);
	//static bqSoundSample_64bitFloat ConvertSample_64bitFloat(bqSoundSample_32bitFloat);

};

bqSoundSample_32bitFloat bqSoundLab::ConvertSample_32bitFloat(bqSoundSample_8bit in_sample)
{
	bqSoundSample_32bitFloat out_sample;
	out_sample.m_data = 0.f;

	const float m = 0.00787401574;// 1:127
	out_sample.m_data = (in_sample.m_data * m) - 1.f;
	if (out_sample.m_data < -1.f)
		out_sample.m_data = -1.f;
	if (out_sample.m_data > 1.f)
		out_sample.m_data = 1.f;

	return out_sample;
}

bqSoundSample_32bitFloat bqSoundLab::ConvertSample_32bitFloat(bqSoundSample_16bit in_sample)
{
	bqSoundSample_32bitFloat out_sample;
	out_sample.m_data = 0.f;

	const double m = 0.0000305185094759971922971282;// 1:32767
	out_sample.m_data = ((double)in_sample.m_data * m);
	if (out_sample.m_data < -1.f)
		out_sample.m_data = -1.f;
	if (out_sample.m_data > 1.f)
		out_sample.m_data = 1.f;

	return out_sample;
}

bqSoundSample_8bit bqSoundLab::ConvertSample_8bit(bqSoundSample_32bitFloat in_sample)
{
	bqSoundSample_8bit out_sample;
	out_sample.m_data = 0;

	if (in_sample.m_data < -1.f)
		in_sample.m_data = -1.f;
	in_sample.m_data += 1.f;
	in_sample.m_data *= 127.f;
	in_sample.m_data = floorf(in_sample.m_data);
	if (in_sample.m_data > 255.f)
		in_sample.m_data = 255.f;
	out_sample.m_data = (bqSoundSample_8bit::sample_type)in_sample.m_data;
	return out_sample;
}

bqSoundSample_16bit bqSoundLab::ConvertSample_16bit(bqSoundSample_32bitFloat in_sample)
{
	bqSoundSample_16bit out_sample;
	out_sample.m_data = 0;

	if (in_sample.m_data < -1.f)
		in_sample.m_data = -1.f;
	in_sample.m_data *= 32767.f;
	in_sample.m_data = floorf(in_sample.m_data);
	if (in_sample.m_data > 0xffff)
		in_sample.m_data = 0xffff;
	out_sample.m_data = (bqSoundSample_16bit::sample_type)in_sample.m_data;
	return out_sample;
}


float bqSoundBuffer_sin_tri(float rads)
{
	float pi = PIf;
	float halfPi = pi * 0.5f;
	float _270 = halfPi + halfPi + halfPi;
	float _180 = pi;
	float _90 = halfPi;
	
	float M = 1.f / halfPi;
	float r = 0.f;

	if (rads >= _270)
	{
		r = (1.f - (-3.f + (rads * M))) * -1.f;
	}
	else if (rads >= _180)
	{
		r = 2.f - (rads * M);
	}
	else if (rads >= _90)
	{
		r = 2.f - (rads * M);
	//	printf("%f %f %f\n", rads, rads *M, r);
	}
	else
	{
		r = rads * M;
		//printf("%f %f %f\n", rads, rads *M, r);
	}
//	printf("\n");
	return r;
}

float bqSoundBuffer_sin_saw(float rads)
{
//	float v = -1.f;
	float r = 0.f;
	float pi = PIf;
	float halfPi = pi * 0.5f;
	float _270 = halfPi + halfPi + halfPi;
	float _180 = pi;
	float _90 = halfPi;
//	float halfHalfPi = pi * 0.25f;
	float M = 1.f / pi;

	if (rads >= _270)
	{
		// from -0.5 to 0
		r = (rads * M) - 2.f;
//		printf("%f %f %f\n", rads, rads *M, r);
	}
	else if (rads >= _180)
	{
		// from -1 to -0.5
		r = (rads * M) - 2.f;
	//	printf("%f %f %f\n", rads, rads *M, r);
	}
	else if (rads >= _90)
	{
		// from 0.5 to 1
		r = (rads * M);
	//	printf("%f %f %f\n", rads, rads *M, r);
	}
	else
	{
		// from 0 to 0.5
		r = rads * M;
		
		//printf("%f\n", r);
	}
//	printf("\n");
	return r;
}

bqSoundBuffer::bqSoundBuffer()
{
}

bqSoundBuffer::~bqSoundBuffer()
{
	if (m_bufferData.m_data)
	{
		bqMemory::free(m_bufferData.m_data);
		m_bufferData.m_data = 0;
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
	if (m_soundBuffer)
	{
		delete m_soundBuffer;
		m_soundBuffer = 0;
	}
}

// 
void bqSound::Create(float time,
	uint32_t channels,
	uint32_t sampleRate,
	uint32_t bitsPerSample)
{
	Clear();

	bqSoundBuffer* newSound = new bqSoundBuffer;
	newSound->m_bufferInfo.m_channels = channels;
	newSound->m_bufferInfo.m_sampleRate = sampleRate;
	newSound->m_bufferInfo.m_bitsPerSample = bitsPerSample;
	newSound->m_bufferInfo.m_bytesPerSample = newSound->m_bufferInfo.m_bitsPerSample / 8;
	newSound->m_bufferInfo.m_blockSize = newSound->m_bufferInfo.m_bytesPerSample * newSound->m_bufferInfo.m_channels;
	newSound->m_bufferInfo.m_time = time;

	newSound->m_bufferInfo.m_numOfSamples = (uint32_t)ceil((float)newSound->m_bufferInfo.m_sampleRate * time);
	// альтернативный способ получить m_numOfSamples
	// для данного метода не подходит так как создание происходит на основе времени
	//uint32_t numOfBlocks = m_soundBuffer->m_dataSize / m_soundBuffer->m_blockSize;
	//m_soundBuffer->m_numOfSamples = numOfBlocks;

	newSound->m_bufferData.m_dataSize = newSound->m_bufferInfo.m_numOfSamples * newSound->m_bufferInfo.m_bytesPerSample;
	newSound->m_bufferData.m_dataSize *= newSound->m_bufferInfo.m_channels;

	newSound->m_bufferData.m_data = (uint8_t*)bqMemory::malloc(newSound->m_bufferData.m_dataSize);

//	newSound->m_format = bqSoundFormatFindFormat(newSound->m_bufferInfo);

	m_soundBuffer = newSound;
}

//bqSoundFormat bqSoundFormatFindFormat(const bqSoundBufferInfo& info)
//{
//	bqSoundFormat r = bqSoundFormat::unsupported;
//
//	if (info.m_channels == 1)
//	{
//		if (info.m_sampleRate == 44100)
//		{
//			if(info.m_bitsPerSample == 8)
//				r = bqSoundFormat::uint8_mono_44100;
//			else if (info.m_bitsPerSample == 16)
//				r = bqSoundFormat::uint16_mono_44100;
//			else if (info.m_bitsPerSample == 32)
//				r = bqSoundFormat::float32_mono_44100;
//		}
//	}
//	else if (info.m_channels == 2)
//	{
//		if (info.m_sampleRate == 44100)
//		{
//			if (info.m_bitsPerSample == 8)
//				r = bqSoundFormat::uint8_stereo_44100;
//			else if (info.m_bitsPerSample == 16)
//				r = bqSoundFormat::uint16_stereo_44100;
//			else if (info.m_bitsPerSample == 32)
//				r = bqSoundFormat::float32_stereo_44100;
//		}
//	}
//	return r;
//}

void bqSoundBuffer::MakeMono(uint32_t how)
{
	/*auto type = bqSoundFormatFindFormat(m_bufferInfo);

	switch (type)
	{
	case bqSoundFormat::uint8_stereo_44100:
	case bqSoundFormat::uint16_stereo_44100:
	case bqSoundFormat::float32_stereo_44100:
	{*/
	if (m_bufferInfo.m_channels > 1)
	{
		uint32_t new_channels = 1;
		uint32_t new_blockSize = m_bufferInfo.m_bytesPerSample * new_channels;
		uint32_t new_dataSize = m_bufferInfo.m_numOfSamples * new_blockSize;
		uint8_t* new_data = (uint8_t*)bqMemory::calloc(new_dataSize);

		// Блок содержит по одному сэмплу с каждого канала.
		// Если 16бит моно то блок равен 2 байта
		// Если 32бит стерео то равен 4байта*2канала=8байт

		uint32_t old_blockSize = m_bufferInfo.m_blockSize;
		// numBlocks должен быть у обоих буферов одинаковым
		uint32_t numBlocks = new_dataSize / new_blockSize;
		
		uint8_t* dst_block = new_data;
		uint8_t* src_block = m_bufferData.m_data;
		
		// должен быть такого же размера как src block
		uint8_t* tmp_block = (uint8_t*)bqMemory::calloc(old_blockSize);

		for (uint32_t i = 0; i < numBlocks; ++i)
		{			
			// Данный способ будет брать только первый канал.
			// Нужно сделать так чтобы можно было выбрать какой канал
			// копировать. Для этого используется парамет `how`
			// Значение 0 у how должен означать использовать все каналы.
			/*for (uint32_t o = 0; o < new_blockSize; ++o)
			{
				dst_block[o] = src_block[o];
			}
			dst_block += new_blockSize;
			src_block += old_blockSize;*/

			// Пусть пока будет реализован how==0
			
			// Надо получить сумму со всех каналов и поделить на количество каналов
			int64_t summInt = 0;
			double summReal = 0.0;
			for (uint32_t o = 0; o < m_bufferInfo.m_channels; ++o)
			{
				if (m_bufferInfo.m_format == bqSoundFormat::float32)
				{
					float* float_data = (float*)src_block;
					summReal += float_data[o];
				}
				else
				{
					int64_t* int_data = (int64_t*)src_block;
					summInt += int_data[o];
				}
			}
			if (summReal != 0.0)
				summReal /= m_bufferInfo.m_channels;
			if (summInt != 0)
				summInt /= m_bufferInfo.m_channels;

			// надо заполнить tmp_block
			for (uint32_t o = 0; o < new_blockSize; ++o)
			{
			}

			for (uint32_t o = 0; o < new_blockSize; ++o)
			{
				dst_block[o] = tmp_block[o];
			}
			dst_block += new_blockSize;
			src_block += old_blockSize;
		}

		/*uint8_t* dst = newData;
		uint8_t* src = m_bufferData.m_data;

		uint8_t* dst8 = dst;
		uint16_t* dst16 = (uint16_t*)dst;
		float* dst32 = (float*)dst;

		uint8_t* src8 = src;
		uint16_t* src16 = (uint16_t*)src;
		float* src32 = (float*)src;

		uint32_t numBlocks = _dataSize / _blockSize;*/

		/*for (uint32_t i = 0; i < numBlocks; ++i)
		{
			switch (type)
			{
			case bqSoundFormat::uint8_stereo_44100:
			{
				if (how)
				{
					*dst8 = *src8;
					++dst8;
					++src8;
					++src8;
				}
				else
				{
					++src8;
					*dst8 = *src8;
					++dst8;
					++src8;
				}
			}break;
			case bqSoundFormat::uint16_stereo_44100:
			{
				if (how)
				{
					*dst16 = *src16;
					++dst16;
					++src16;
					++src16;
				}
				else
				{
					++src16;
					*dst16 = *src16;
					++dst16;
					++src16;
				}
			}break;
			case bqSoundFormat::float32_stereo_44100:
			{
				if (how)
				{
					*dst32 = *src32;
					++dst32;
					++src32;
					++src32;
				}
				else
				{
					++src32;
					*dst32 = *src32;
					++dst32;
					++src32;
				}
			}break;
			}
		}*/

		if (m_bufferData.m_data)
			bqMemory::free(m_bufferData.m_data);

		if(tmp_block)
			bqMemory::free(tmp_block);

		m_bufferInfo.m_channels = new_channels;
		m_bufferInfo.m_blockSize = new_blockSize;
		m_bufferData.m_data = new_data;
		m_bufferData.m_dataSize = new_dataSize;
	}
	/*}break;
	default:
		break;
	}
	m_format = bqSoundFormatFindFormat(m_bufferInfo);*/
}

void bqSoundBuffer::MakeStereo()
{
	auto type = bqSoundFormatFindFormat(m_bufferInfo);
	
	switch (type)
	{
	case bqSoundFormat::uint8_mono_44100:
	case bqSoundFormat::uint16_mono_44100:
	case bqSoundFormat::float32_mono_44100:
	{
		uint32_t _channels = 2;
		uint32_t _blockSize = m_bufferInfo.m_bytesPerSample * _channels;
		uint32_t _dataSize = m_bufferInfo.m_numOfSamples * m_bufferInfo.m_bytesPerSample * _channels;
		uint8_t* newData = (uint8_t*)bqMemory::calloc(_dataSize);

		uint8_t* dst = newData;
		uint8_t* src = m_bufferData.m_data;

		uint8_t* dst8 = dst;
		uint16_t* dst16 = (uint16_t*)dst;
		float* dst32 = (float*)dst;

		uint8_t* src8 = src;
		uint16_t* src16 = (uint16_t*)src;
		float* src32 = (float*)src;

		uint32_t numBlocks = _dataSize / _blockSize;
		for (uint32_t i = 0; i < numBlocks; ++i)
		{
			switch (type)
			{
			case bqSoundFormat::uint8_mono_44100:
			{
				*dst8 = *src8;
				++dst8;
				*dst8 = *src8;
				++dst8;
				++src8;
			}break;
			case bqSoundFormat::uint16_mono_44100:
			{
				*dst16 = *src16;
				++dst16;
				*dst16 = *src16;
				++dst16;
				++src16;
			}break;
			case bqSoundFormat::float32_mono_44100:
			{
				*dst32 = *src32;
				++dst32;
				*dst32 = *src32;
				++dst32;
				++src32;
			}break;
			}
		}

		if (m_bufferData.m_data)
			bqMemory::free(m_bufferData.m_data);

		m_bufferInfo.m_channels = _channels;
		m_bufferInfo.m_blockSize = _blockSize;
		m_bufferData.m_data = newData;
		m_bufferData.m_dataSize = _dataSize;
	}break;
	default:
		break;
	}

	m_format = bqSoundFormatFindFormat(m_bufferInfo);
}

uint8_t bqSoundBuffer::_32_to_8(float v)
{
	if (v < -1.f)
			v = -1.f;
		v += 1.f;
		v *= 127.f;
		v = floorf(v);
		if (v > 255.f)
			v = 255.f;
	return (uint8_t)v;
}

int16_t bqSoundBuffer::_32_to_16(float v)
{
	if (v < -1.f)
			v = -1.f;
		v *= 32767.f;
		v = floorf(v);
		if (v > 0xffff)
			v = 0xffff;
	return (int16_t)v;
}

// 0   0
// 127 1
// 255 2
float bqSoundBuffer::_8_to_32(uint8_t v)
{
	const float m = 0.00787401574;// 1:127
	float r = (v * m)-1.f;
	if(r<-1.f)
		r = -1.f;
	if(r > 1.f)
		r = 1.f;
	return r;
}


// -32767   0
// 0        1
// 32767    2
float bqSoundBuffer::_16_to_32(int16_t v)
{
	const double m = 0.0000305185094759971922971282;// 1:32767
	float r = ((double)v * m);
	if(r<-1.f)
		r = -1.f;
	if(r > 1.f)
		r = 1.f;
	return r;
}


void bqSoundBuffer::Make8bits()
{
	auto type = bqSoundFormatFindFormat(m_bufferInfo);

	switch (type)
	{
	case bqSoundFormat::uint16_mono_44100:
	case bqSoundFormat::uint16_stereo_44100:
	case bqSoundFormat::float32_mono_44100:
	case bqSoundFormat::float32_stereo_44100:
	{
		uint32_t _channels = 2;

		if(type == bqSoundFormat::uint16_mono_44100
			|| type == bqSoundFormat::float32_mono_44100)
			_channels = 1;
		 
		uint32_t _bytesPerSample = 1;
		uint32_t _bitsPerSample = 8;
		uint32_t _blockSize = _bytesPerSample * _channels;
		uint32_t _dataSize = m_bufferInfo.m_numOfSamples * _bytesPerSample * _channels;
		uint8_t* newData = (uint8_t*)bqMemory::calloc(_dataSize);

		uint8_t* dst8 = (uint8_t*)newData;
		int16_t* src16 = (int16_t*)m_bufferData.m_data;
		float* src32 = (float*)m_bufferData.m_data;

		uint32_t numBlocks = _dataSize / _blockSize;
		for (uint32_t i = 0; i < numBlocks; ++i)
		{
			switch (type)
			{
			case bqSoundFormat::uint16_mono_44100:
			{
				*dst8 = (*src16 + 32767) >> 8;
				++src16;
				++dst8;
			}break;
			case bqSoundFormat::uint16_stereo_44100:
			{
				*dst8 = (*src16 + 32767) >> 8;
				++src16;
				++dst8;
				*dst8 = (*src16 + 32767) >> 8;
				++src16;
				++dst8;
			}break;

			case bqSoundFormat::float32_mono_44100:
			{
	//			float v = *src32;
		//		if (v < -1.f)
		//			v = -1.f;
	//			v += 1.f;
	//			v *= 127.f;
		//		v = ceilf(v);
//				if (v > 255.f)
	//				v = 255.f;
		//		*dst8 = (uint8_t)v;

				*dst8 = _32_to_8(*src32);

				++src32;
				++dst8;
			}break;
			case bqSoundFormat::float32_stereo_44100:
			{
				*dst8 = _32_to_8(*src32);
				++src32;
				++dst8;
				*dst8 = _32_to_8(*src32);
				++src32;
				++dst8;
			}break;
			}
		}

		if (m_bufferData.m_data)
			bqMemory::free(m_bufferData.m_data);

		m_bufferInfo.m_channels = _channels;
		m_bufferInfo.m_blockSize = _blockSize;
		m_bufferInfo.m_bytesPerSample = _bytesPerSample;
		m_bufferInfo.m_bitsPerSample = _bitsPerSample;
		m_bufferData.m_data = newData;
		m_bufferData.m_dataSize = _dataSize;
	}break;
	default:
		break;
	}
	
	m_format = bqSoundFormatFindFormat(m_bufferInfo);
}

void bqSoundBuffer::Make16bits()
{
	auto type = bqSoundFormatFindFormat(m_bufferInfo);

	switch (type)
	{
	case bqSoundFormat::uint8_mono_44100:
	case bqSoundFormat::uint8_stereo_44100:
	case bqSoundFormat::float32_mono_44100:
	case bqSoundFormat::float32_stereo_44100:
	{
		uint32_t _channels = 2;

		if (type == bqSoundFormat::uint8_mono_44100
				|| type == bqSoundFormat::float32_mono_44100)
			_channels = 1;

		uint32_t _bytesPerSample = 2;
		uint32_t _bitsPerSample = 16;
		uint32_t _blockSize = _bytesPerSample * _channels;
		uint32_t _dataSize = m_bufferInfo.m_numOfSamples * _bytesPerSample * _channels;
		uint8_t* newData = (uint8_t*)bqMemory::calloc(_dataSize);

		int16_t* dst16 = (int16_t*)newData;
		uint8_t* src8 = (uint8_t*)m_bufferData.m_data;
		float* src32 = (float*)m_bufferData.m_data;

		uint32_t numBlocks = _dataSize / _blockSize;
		for (uint32_t i = 0; i < numBlocks; ++i)
		{
			switch (type)
			{
			case bqSoundFormat::uint8_mono_44100:
			{
				*dst16 = ((uint16_t)*src8 << 8) - 32767;
				++src8;
				++dst16;
			}break;
			case bqSoundFormat::uint8_stereo_44100:
			{
				*dst16 = ((uint16_t)*src8 << 8) - 32767;
				++src8;
				++dst16;
				*dst16 = ((uint16_t)*src8 << 8) - 32767;
				++src8;
				++dst16;
			}break;
			case bqSoundFormat::float32_mono_44100:
			{
				*dst16 = _32_to_16(*src32);
				++src32;
				++dst16;
			}break;
			case bqSoundFormat::float32_stereo_44100:
			{
				*dst16 = _32_to_16(*src32);
				++src32;
				++dst16;
				*dst16 = _32_to_16(*src32);
				++src32;
				++dst16;
			}break;
			}
		}

		if (m_bufferData.m_data)
			bqMemory::free(m_bufferData.m_data);

		m_bufferInfo.m_channels = _channels;
		m_bufferInfo.m_blockSize = _blockSize;
		m_bufferInfo.m_bytesPerSample = _bytesPerSample;
		m_bufferInfo.m_bitsPerSample = _bitsPerSample;
		m_bufferData.m_data = newData;
		m_bufferData.m_dataSize = _dataSize;
	}break;
	default:
		break;
	}

	m_format = bqSoundFormatFindFormat(m_bufferInfo);
}

void bqSoundBuffer::Make32bits()
{
	auto type = bqSoundFormatFindFormat(m_bufferInfo);

	switch (type)
	{
	case bqSoundFormat::uint8_mono_44100:
	case bqSoundFormat::uint8_stereo_44100:
	case bqSoundFormat::uint16_mono_44100:
	case bqSoundFormat::uint16_stereo_44100:
	{
		uint32_t _channels = 2;

		if (type == bqSoundFormat::uint8_mono_44100
			|| type == bqSoundFormat::uint16_mono_44100)
			_channels = 1;

		uint32_t _bytesPerSample = 4;
		uint32_t _bitsPerSample = 32;
		uint32_t _blockSize = _bytesPerSample * _channels;
		uint32_t _dataSize = m_bufferInfo.m_numOfSamples * _bytesPerSample * _channels;
		uint8_t* newData = (uint8_t*)bqMemory::calloc(_dataSize);

		float* dst32 = (float*)newData;
		uint8_t* src8 = (uint8_t*)m_bufferData.m_data;
		int16_t* src16 = (int16_t*)m_bufferData.m_data;

		uint32_t numBlocks = _dataSize / _blockSize;
		for (uint32_t i = 0; i < numBlocks; ++i)
		{
			switch (type)
			{
			case bqSoundFormat::uint8_mono_44100:
			{
				*dst32 = _8_to_32(*src8);
				++src8;
				++dst32;
			}break;
			case bqSoundFormat::uint8_stereo_44100:
			{
				*dst32 = _8_to_32(*src8);
				++src8;
				++dst32;
				*dst32 = _8_to_32(*src8);
				++src8;
				++dst32;
			}break;
			case bqSoundFormat::uint16_mono_44100:
			{
				*dst32 = _16_to_32(*src16);
				++src16;
				++dst32;
			}break;
			case bqSoundFormat::uint16_stereo_44100:
			{
				*dst32 = _16_to_32(*src16);
				++src16;
				++dst32;
				*dst32 = _16_to_32(*src16);
				++src16;
				++dst32;
			}break;
			}
		}

		if (m_bufferData.m_data)
			bqMemory::free(m_bufferData.m_data);

		m_bufferInfo.m_channels = _channels;
		m_bufferInfo.m_blockSize = _blockSize;
		m_bufferInfo.m_bytesPerSample = _bytesPerSample;
		m_bufferInfo.m_bitsPerSample = _bitsPerSample;
		m_bufferData.m_data = newData;
		m_bufferData.m_dataSize = _dataSize;
	}break;
	default:
		break;
	}

	m_format = bqSoundFormatFindFormat(m_bufferInfo);
}

void bqSound::Generate(
	bqSoundWaveType waveType,
	float time, 
	uint32_t Hz, 
	float _loudness)
{
	Clear();
	
	if (time > 0.f)
	{
		Create(time, 1, 44100, 16);

		double samplesPerWave = (double)m_soundBuffer->m_bufferInfo.m_sampleRate / Hz;
		double angle = 0.f;
		double angleStep = 360.0 / samplesPerWave;

		float lM = 1.f / 32767.f;
		int16_t loudness = int16_t(_loudness / lM);
	//	printf("%f %f %i\n", _loudness, lM, loudness);

		// наверно лучше использовать это для сброса угла
		uint32_t samplesPerWavei = (uint32_t)floor(samplesPerWave);
		uint32_t samplesPerWaveindex = 0;


		for (uint32_t i = 0, index = 0; i < m_soundBuffer->m_bufferInfo.m_numOfSamples; ++i)
		{
			auto rad = bqMath::DegToRad(angle);
			auto sn = sin(rad);

			++samplesPerWaveindex;
			if(samplesPerWaveindex >= samplesPerWavei)
			{ 
				samplesPerWaveindex = 0;
				angle = 0.;
			}

			angle += angleStep;
			if (angle >= 360.)
			{
				angle = 0.;
			}

			union _short
			{
				int8_t _8[2];
				int16_t _16;
			};
			_short v;

			if (waveType == bqSoundWaveType::sin)
			{

				v._16 = (int16_t)(sn * loudness);
				//		printf("%f %f %f %i\n", angle, rad, sn, v._16);
			}
			else if (waveType == bqSoundWaveType::square)
			{
				if (sn >= 0.f)
				{
					v._16 = loudness;
				}
				else
				{
					v._16 = -loudness;
				}
			}
			else if (waveType == bqSoundWaveType::triangle)
			{
				auto sn2 = bqSoundBuffer_sin_tri((float)rad);
				v._16 = (int16_t)(sn2 * loudness);
			}
			else if (waveType == bqSoundWaveType::saw)
			{
				auto sn2 = bqSoundBuffer_sin_saw((float)rad);
				v._16 = (int16_t)(sn2 * loudness);
			}

			m_soundBuffer->m_bufferData.m_data[index++] = v._8[0];
			m_soundBuffer->m_bufferData.m_data[index++] = v._8[1];
		}
	}
	/*if (!m_soundBuffer)
	{
		int time = 1;

		m_soundBuffer = new bqSoundBuffer;
		m_soundBuffer->m_channels = 1;
		m_soundBuffer->m_sampleRate = 44100;
		m_soundBuffer->m_size = time * 2 * m_soundBuffer->m_sampleRate;
		m_soundBuffer->m_data = (uint8_t*)bqMemory::malloc(m_soundBuffer->m_size);

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

				m_soundBuffer->m_data[index++] = v._8[0];
				m_soundBuffer->m_data[index++] = v._8[1];
			}
		}
	}
*/
}

bool bqSound::SaveToFile(bqSoundFileType ft, const bqStringA& fn)
{
	return SaveToFile(ft, fn.c_str());
}

bool bqSound::SaveToFile(bqSoundFileType ft, const char* fn)
{
	BQ_ASSERT_ST(fn);
	BQ_ASSERT_ST(m_soundBuffer);

	switch(ft)
	{
	default:
	case bqSoundFileType::wav:
	return _saveWav(fn);
	}
}

bool bqSound::_saveWav(const char* fn)
{
	FILE * f =0;
	fopen_s(&f, fn, "wb");
	if(f)
	{
		fwrite("RIFF", 4, 1, f);
			
		int32_t chunkSz = m_soundBuffer->m_bufferData.m_dataSize + 44 - 8;

		if (m_soundBuffer->m_format == bqSoundFormat::float32_mono_44100
			|| m_soundBuffer->m_format == bqSoundFormat::float32_stereo_44100)
			chunkSz += 4 + 4 + 4;

		fwrite(&chunkSz, 4, 1, f);
			
		fwrite("WAVE", 4, 1, f);
		fwrite("fmt ", 4, 1, f);
		
		// 16 - минимальный размер
		// 18 - extraFormatInfoSz
		// если 18 и extraFormatInfoSz == 22 то
		//     subchnkSz = 40
		int32_t subchnkSz = 16;
		fwrite(&subchnkSz, 4, 1, f);
			
		// pcm = 1
		// IEEE float = 3
		int16_t TYPE = 1;

		if(m_soundBuffer->m_format == bqSoundFormat::float32_mono_44100
			|| m_soundBuffer->m_format == bqSoundFormat::float32_stereo_44100)
			TYPE = 3;

		fwrite(&TYPE, 2, 1, f);
			
		fwrite(&m_soundBuffer->m_bufferInfo.m_channels, 2, 1, f);
		fwrite(&m_soundBuffer->m_bufferInfo.m_sampleRate, 4, 1, f);
			
		int nAvgBytesPerSec = (m_soundBuffer->m_bufferInfo.m_sampleRate *
			m_soundBuffer->m_bufferInfo.m_bitsPerSample * m_soundBuffer->m_bufferInfo.m_channels)/8;
		// byterate
		fwrite(&nAvgBytesPerSec, 4, 1, f);
			
		fwrite(&m_soundBuffer->m_bufferInfo.m_blockSize, 2, 1, f);
		fwrite(&m_soundBuffer->m_bufferInfo.m_bitsPerSample, 2, 1, f);
		
		// Если 
		//uint16_t extraFormatInfoSz = 0;
		//fwrite(&extraFormatInfoSz, 2, 1, f);

		if (m_soundBuffer->m_format == bqSoundFormat::float32_mono_44100
			|| m_soundBuffer->m_format == bqSoundFormat::float32_stereo_44100)
		{
			fwrite("fact", 4, 1, f);
			int32_t factchunkSz = 4;
			fwrite(&factchunkSz, 4, 1, f);
			fwrite(&m_soundBuffer->m_bufferInfo.m_numOfSamples, 4, 1, f);

		}

		fwrite("data", 4, 1, f);
		fwrite(&m_soundBuffer->m_bufferData.m_dataSize, 4, 1, f);
		fwrite(m_soundBuffer->m_bufferData.m_data, m_soundBuffer->m_bufferData.m_dataSize, 1, f);
		fclose(f);
		return true;
	}

	return false;
}

bool bqSound::LoadFromFile(const bqStringA& fn)
{
	return LoadFromFile(fn.c_str());
}

bool bqSound::LoadFromFile(const char* fn)
{
	Clear();

	bqString path(fn);
	bool r = false;
	if (path.extension(".wav"))
	{
		r = _loadWav(fn);
	}

	return r;
}

bool bqSound::_loadWav(const char* fn)
{
	uint32_t file_size = 0;
	uint8_t* ptr = bqFramework::SummonFileBuffer(fn, &file_size, false);
	if (ptr)
	{
		bool b = _loadWav(ptr, (uint32_t)file_size);
		bqDestroy(ptr);
		return b;
	}
	return false;
}

void bqSound::Convert(bqSoundFormat type)
{
	BQ_ASSERT_ST(type != bqSoundFormat::unsupported);

	if (m_soundBuffer && (type != bqSoundFormat::unsupported))
	{
		if (m_soundBuffer->m_format != type)
		{
			switch (type)
			{
				case bqSoundFormat::uint8_mono_44100:
				{
					m_soundBuffer->Make8bits();
					m_soundBuffer->MakeMono(0);
				}break;
				case bqSoundFormat::uint8_stereo_44100:
				{
					m_soundBuffer->Make8bits();
					m_soundBuffer->MakeStereo();
				}break;
				case bqSoundFormat::uint16_mono_44100:
				{
					m_soundBuffer->Make16bits();
					m_soundBuffer->MakeMono(0);
				}break;
				case bqSoundFormat::uint16_stereo_44100:
				{
					m_soundBuffer->Make16bits();
					m_soundBuffer->MakeStereo();
				}break;
				case bqSoundFormat::float32_mono_44100:
				{
					m_soundBuffer->Make32bits();
					m_soundBuffer->MakeMono(0);
				}break;
				case bqSoundFormat::float32_stereo_44100:
				{
					m_soundBuffer->Make32bits();
					m_soundBuffer->MakeStereo();
				}break;
			}
		}
	}
}

bool bqSound::_loadWav(uint8_t* buffer, uint32_t bufferSz)
{
	bqFileBuffer file(buffer, bufferSz);

	char riff[5] = { 0,0,0,0,0 };
	file.Read(riff, 4);
	if (strcmp(riff, "RIFF") == 0)
	{
		uint32_t RIFFChunkSize = 0;
		file.Read(&RIFFChunkSize, 4);

		char wave[5] = { 0,0,0,0,0 };
		file.Read(wave, 4);
		if (strcmp(wave, "WAVE") == 0)
		{
			char fmt[5] = { 0,0,0,0,0 };
			file.Read(fmt, 4);
			if (strcmp(fmt, "fmt ") == 0)
			{
				uint32_t FMTChunkSize = 0;
				file.Read(&FMTChunkSize, 4);

				uint16_t TYPE = 0;
				file.Read(&TYPE, 2);

				uint16_t channels = 0;
				file.Read(&channels, 2);

				uint32_t sampleRate = 0;
				file.Read(&sampleRate, 4);

				uint32_t nAvgBytesPerSec = 0;
				file.Read(&nAvgBytesPerSec, 4);

				uint16_t blockSize = 0;
				file.Read(&blockSize, 2);

				uint16_t bitsPerSample = 0;
				file.Read(&bitsPerSample, 2);

				if (FMTChunkSize > 16)
				{
					uint16_t extraFormatInfoSz = 0;
					file.Read(&extraFormatInfoSz, 2);

					if (extraFormatInfoSz == 22)
					{
						uint16_t wValidBitsPerSample = 0;
						uint32_t dwChannelMask = 0;
						uint8_t SubFormat[16];
						file.Read(&wValidBitsPerSample, 2);
						file.Read(&dwChannelMask, 4);
						file.Read(SubFormat, 16);
					}
				}

				if (TYPE != 1)
				{
					char fact[5] = { 0,0,0,0,0 };
					file.Read(fact, 4);
					if (strcmp(fact, "fact") == 0)
					{
						uint32_t factcksize = 0;
						file.Read(&factcksize, 4);
						if(factcksize < 4)
							return false;

						// на каждый канал.
						// наверное Chunk size и содержит размер с учётом количества каналов
						// поэтому получаю это количество через factcksize / 4
						for (uint32_t i = 0, sz = factcksize / 4; i < sz; ++i)
						{
							uint32_t factdwSampleLength = 0;
							file.Read(&factdwSampleLength, 4);
						}

					}
					else
					{
						return false;
					}
				}

				char data[5] = { 0,0,0,0,0 };
				file.Read(data, 4);

				// возможно есть PEAK
				if (strcmp(data, "PEAK") == 0)
				{
					uint32_t peakchunkDataSize = 0;
					uint32_t peakversion = 0;
					uint32_t peaktimeStamp = 0;

					struct PositionPeak
					{
						float   value;    /* signed value of peak */
						uint32_t position; /* the sample frame for the peak */
					};

					file.Read(&peakchunkDataSize, 4);
					file.Read(&peakversion, 4);
					file.Read(&peaktimeStamp, 4);

					if (peakchunkDataSize > 8)// надо ли это делать?
					{
						PositionPeak peak;
						for (uint32_t i = 0; i < channels; ++i)
						{
							file.Read(&peak.value, 4);
							file.Read(&peak.position, 4);
						}
					}

					memset(data, 0, 5);
					file.Read(data, 4);
				}

				if (strcmp(data, "data") == 0)
				{
					uint32_t dataSize = 0;
					file.Read(&dataSize, 4);
					if (dataSize)
					{
						Create(0.1f, channels, sampleRate, bitsPerSample);
						if (m_soundBuffer->m_bufferData.m_dataSize < dataSize)
							_reallocate(dataSize);

						file.Read(m_soundBuffer->m_bufferData.m_data, m_soundBuffer->m_bufferData.m_dataSize);

						//Convert();

						CalculateTime();
						return true;
					}
				}
			}
		}
	}

	return false;
}

float bqSound::CalculateTime()
{
	float time = 0.f;
	if (m_soundBuffer)
	{
		BQ_ASSERT_ST(m_soundBuffer->m_bufferInfo.m_sampleRate > 0);

		if (m_soundBuffer->m_bufferInfo.m_sampleRate && m_soundBuffer->m_bufferData.m_dataSize)
		{
			uint32_t numOfBlocks = m_soundBuffer->m_bufferData.m_dataSize / m_soundBuffer->m_bufferInfo.m_blockSize;
			
			// правильно ли то что количество сэмплов == количеству блоков?
			m_soundBuffer->m_bufferInfo.m_numOfSamples = numOfBlocks;
			
			// может быть блок может состоять из 2х и более сэмплов?
			//m_soundBuffer->m_bufferInfo.m_numOfSamples = numOfBlocks * m_soundBuffer->m_bufferInfo.m_channels;
			
			m_soundBuffer->m_bufferInfo.m_time = 1.f / m_soundBuffer->m_bufferInfo.m_sampleRate;
			m_soundBuffer->m_bufferInfo.m_time *= m_soundBuffer->m_bufferInfo.m_numOfSamples;
		}
	}

	return time;
}

// если при чтении файла нужен будет буфер бОльшего размера
// надо его увеличить.
void bqSound::_reallocate(uint32_t newSz)
{
	uint8_t* newBuf = (uint8_t*)bqMemory::malloc(newSz);
	memcpy(newBuf, m_soundBuffer->m_bufferData.m_data, m_soundBuffer->m_bufferData.m_dataSize);
	bqMemory::free(m_soundBuffer->m_bufferData.m_data);
	m_soundBuffer->m_bufferData.m_data = newBuf;
	m_soundBuffer->m_bufferData.m_dataSize = newSz;
}

//bqSoundBuffer* bqSound::DropSource()
//{
//	bqSoundBuffer* tmp = m_soundBuffer;
//	m_soundBuffer = 0;
//	return tmp;
//}
