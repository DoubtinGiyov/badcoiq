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

#include "badcoiq/GUI/bqGUI.h"
#include "badcoiq/gs/bqGS.h"
#include "badcoiq/input/bqInputEx.h"

#include "../framework/bqFrameworkImpl.h"
extern bqFrameworkImpl* g_framework;
#include "bqGUIDefaultTextDrawCallbacks.h"

bqGUIScrollbar::bqGUIScrollbar(bqGUIWindow* w, const bqVec2f& position, const bqVec2f& size)
	:
	bqGUIElement::bqGUIElement(w, position, size)
{
	m_textDrawCallback = g_framework->m_defaultTextDrawCallback_staticText;
}

bqGUIScrollbar::~bqGUIScrollbar()
{
}

void bqGUIScrollbar::Rebuild()
{
	m_textDrawCallback->m_element = this;
	bqGUIElement::Rebuild();
}

void bqGUIScrollbar::Update()
{
	bqGUIElement::Update();
}

void bqGUIScrollbar::Draw(bqGS* gs, float dt)
{
	gs->SetScissorRect(m_clipRect);
	if (IsDrawBG())
		gs->DrawGUIRectangle(m_buildRect, m_style->m_staticTextBGColor, m_style->m_staticTextBGColor, 0, 0);
	m_textDrawCallback->m_element = this;
}

