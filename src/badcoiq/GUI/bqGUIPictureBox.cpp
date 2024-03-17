﻿/*
BSD 2-Clause License

Copyright (c) 2024, badcoiq
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

#include "badcoiq/GUI/bqGUI.h"
#include "badcoiq/gs/bqGS.h"
#include "badcoiq/input/bqInputEx.h"

#include "../framework/bqFrameworkImpl.h"
extern bqFrameworkImpl* g_framework;

bqGUIPictureBox::bqGUIPictureBox(bqGUIWindow* w, const bqVec2f& position, const bqVec2f& size)
	:
	bqGUIElement::bqGUIElement(w, position, size)
{
}

bqGUIPictureBox::~bqGUIPictureBox()
{
}

void bqGUIPictureBox::Rebuild()
{
	bqGUIElement::Rebuild();
}

void bqGUIPictureBox::Update()
{
	bqGUIElement::Update();
}

void bqGUIPictureBox::Draw(bqGS* gs, float)
{
	gs->SetScissorRect(m_clipRect);
	gs->DrawGUIRectangle(m_buildRect, 
		m_style->m_pictureBoxBGColor,
		m_style->m_pictureBoxBGColor,
		m_texture,
		&m_uv);
}

void bqGUIPictureBox::SetTexture(bqTexture* t)
{
	m_texture = t; 
}

void bqGUIPictureBox::SetUV(const bqVec4f& uv)
{
	m_uv = uv;
}

void bqGUIPictureBox::SetTCoords(float left, float top, float right, float bottom)
{
	if (m_texture)
	{
		float h = (float)m_texture->GetInfo().m_imageInfo.m_height;
		float w = (float)m_texture->GetInfo().m_imageInfo.m_width;
		m_uv.x = bqMath::CoordToUV(left, w);
		m_uv.y = bqMath::CoordToUV(top, h);
		m_uv.z = bqMath::CoordToUV(right, w);
		m_uv.w = bqMath::CoordToUV(bottom, h);
	}
}
