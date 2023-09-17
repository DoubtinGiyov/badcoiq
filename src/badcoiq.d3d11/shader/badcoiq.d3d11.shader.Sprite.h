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

#pragma once
#ifndef _BQ_D3D11_SHADER_Sprt_H__
#define _BQ_D3D11_SHADER_Sprt_H__

class bqGSD3D11;
class bqGSD3D11Texture;
class bqD3D11ShaderSprite : public bqGSD3D11ShaderBase
{
public:
	bqD3D11ShaderSprite(bqGSD3D11* gs);
	virtual ~bqD3D11ShaderSprite();

	bqGSD3D11* m_gs = 0;

	ID3D11Buffer* m_cbElement = 0;

	struct cbElement
	{
		bqMat4 W;
		bqMat4 V;
		bqMat4 P;
		bqMat4 Vi;
		bqVec4 Corners;
		bqColor Color1;
		bqVec4f UVs;
		float alphaDiscard = 0.5f;
		float padding[3];
	}m_cbDataElement;

	virtual void SetConstants(bqMaterial* material) final;

	bool Init();
	void SetOnElement(bqGSD3D11Texture* texture);
};

#endif