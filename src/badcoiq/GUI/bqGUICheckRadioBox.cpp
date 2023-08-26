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

#include "../framework/bqFrameworkImpl.h"
extern bqFrameworkImpl* g_framework;

#include "bqGUIDefaultTextDrawCallbacks.h"

bqGUICheckRadioBoxTextDrawCallback::bqGUICheckRadioBoxTextDrawCallback()
{
	m_color = bq::ColorWhite;
}

bqGUICheckRadioBoxTextDrawCallback::~bqGUICheckRadioBoxTextDrawCallback()
{
}

bqGUIFont* bqGUICheckRadioBoxTextDrawCallback::OnFont(uint32_t r, char32_t)
{
	return m_font;
}

bqColor* bqGUICheckRadioBoxTextDrawCallback::OnColor(uint32_t r, char32_t)
{
	return &m_color;
}

bqGUICheckRadioBox::bqGUICheckRadioBox(bqGUIWindow* w, const bqVec2f& position, const bqVec2f& size)
	:
	bqGUIButton(w, position, size)
{
	m_iconVerticalIndent = 2.f;
	SetDrawBG(false);
}

bqGUICheckRadioBox::~bqGUICheckRadioBox() {}

void bqGUICheckRadioBox::Rebuild()
{
	bqGUIButton::Rebuild();
}

void bqGUICheckRadioBox::Draw(bqGS* gs, float dt)
{
	char32_t text[2] = { (char32_t)bqGUIDefaultIconID::CheckboxUncheck, 0 };

	if (m_asRadioButton)
	{
		if (m_isChecked)
			text[0] = (char32_t)bqGUIDefaultIconID::RadioCheck;
		else
			text[0] = (char32_t)bqGUIDefaultIconID::RadioUncheck;
	}
	else
	{
		if (m_isChecked)
			text[0] = (char32_t)bqGUIDefaultIconID::CheckboxCheck;
	}

	auto iconTextSz = m_iconDrawCallback->GetTextSize(text);

	m_textPosition.x += iconTextSz.x;
	bqGUIButton::Draw(gs, dt);
	m_textPosition.x -= iconTextSz.x;

	bqVec2f textPosition;
	textPosition.x = m_buildRect.x;
	textPosition.y = m_buildRect.y + m_iconVerticalIndent;

	if (IsEnabled())
	{
		if (IsClickedLMB())
		{
			m_textDrawCallback->m_reason = bqGUIDrawTextCallback::Reason_pressed;
			gs->DrawGUIText(text, 1, textPosition, m_textDrawCallback);
		}
		else
		{
			if (IsCursorInRect())
			{
				m_textDrawCallback->m_reason = bqGUIDrawTextCallback::Reason_mouseAbove;
				gs->DrawGUIText(text, 1, textPosition, m_textDrawCallback);
			}
			else
			{
				m_textDrawCallback->m_reason = bqGUIDrawTextCallback::Reason_icon;
				gs->DrawGUIText(text, 1, textPosition, m_textDrawCallback);
			}
		}
	}
	else
	{
		m_textDrawCallback->m_reason = bqGUIDrawTextCallback::Reason_disabled;
		gs->DrawGUIText(text, 1, textPosition, m_textDrawCallback);
	}
}

void bqGUICheckRadioBox::OnClickLMB()
{
	if (m_asRadioButton)
	{
		if (!m_isChecked)
		{
			m_isChecked = true;

			// uncheck other
			bqGUIElement* parent = dynamic_cast<bqGUIElement*>(GetParent());
			if (parent)
			{
				auto& children = *parent->GetChildren();
				for (auto _child : children)
				{
					auto child = dynamic_cast<bqGUICheckRadioBox*>(_child);
					if (child)
					{
						if (child != this && child->m_asRadioButton)
						{
							if (child->m_radiouGroup == m_radiouGroup)
							{
								child->m_isChecked = false;
								child->OnUnCheck();
							}
						}
					}
				}
			}
		}
	}
	else
	{
		m_isChecked = m_isChecked ? false : true;
		m_isChecked ? OnCheck() : OnUnCheck();
	}
}

void bqGUICheckRadioBox::OnCheck()
{
}

void bqGUICheckRadioBox::OnUnCheck()
{
}
