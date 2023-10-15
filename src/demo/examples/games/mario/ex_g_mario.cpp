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

#include "../../../DemoApp.h"


ExampleGameMario::ExampleGameMario(DemoApp* app)
	:
	DemoExample(app)
{
}

ExampleGameMario::~ExampleGameMario()
{
}


bool ExampleGameMario::Init()
{
	m_camera = new bqCamera();
	m_camera->m_position = bqVec3(3.f, 3.f, 3.f);
	m_camera->m_aspect = (float)m_app->GetWindow()->GetCurrentSize()->x / (float)m_app->GetWindow()->GetCurrentSize()->y;
	m_camera->Rotate(36.f, -45.f, 0.f);
	m_camera->SetType(bqCamera::Type::PerspectiveLookAt);
	m_camera->Update(0.f);
	m_camera->m_viewProjectionMatrix = m_camera->m_projectionMatrix * m_camera->m_viewMatrix;

	// для 3D линии
	bqFramework::SetMatrix(bqMatrixType::ViewProjection, &m_camera->m_viewProjectionMatrix);

	// генерация всей графики
	// Думаю всё можно уместить в одну текстуру
	bqImage img;
	img.Create(256, 256);
	img.Fill(bqImageFillType::Solid, bq::ColorTransparent, bq::ColorTransparent);

	bqColor palette[] =
	{
		bq::ColorWhite,
		bq::ColorBlack,

		// ground
		0xFF994E00, // brown
		0xFFFFCCC5, // like pink
		
		bq::ColorTransparent,
		
		0xFFEA9E22, // box bright
		0xFF561D00, // box dark
	};

	// ground block
	{
		uint8_t pic[] =
		{
			2, 3, 3, 3, 3, 3, 3, 3, 3, 1, 2, 3, 3, 3, 3, 2,
			3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 1,
			3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 1,
			3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 1,
			3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 1, 2, 2, 2, 1,
			3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 2,
			3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 3, 3, 3, 3, 1,
			3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 1,
			3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 1,
			3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 1,
			1, 1, 2, 2, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 2, 1,
			3, 3, 1, 1, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 2, 1,
			3, 2, 3, 3, 1, 1, 1, 1, 3, 2, 2, 2, 2, 2, 2, 1,
			3, 2, 2, 2, 3, 3, 3, 1, 3, 2, 2, 2, 2, 2, 2, 1,
			3, 2, 2, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 2, 1, 1,
			2, 1, 1, 1, 1, 1, 1, 2, 3, 1, 1, 1, 1, 1, 1, 2,
		};
		img.Fill(palette, pic, 16, 16, 0, 0);
	}

	// empty box
	{
		uint8_t pic[] =
		{
			4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1,
			1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
		};
		img.Fill(palette, pic, 16, 16, 16, 0);
	}

	// box[0]
	{
		uint8_t pic[] =
		{
			4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4,
			2, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1,
			2, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 1,
			2, 5, 5, 5, 5, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 1,
			2, 5, 5, 5, 2, 2, 1, 1, 1, 2, 2, 5, 5, 5, 5, 1,
			2, 5, 5, 5, 2, 2, 1, 5, 5, 2, 2, 1, 5, 5, 5, 1,
			2, 5, 5, 5, 2, 2, 1, 5, 5, 2, 2, 1, 5, 5, 5, 1,
			2, 5, 5, 5, 5, 1, 1, 5, 2, 2, 2, 1, 5, 5, 5, 1,
			2, 5, 5, 5, 5, 5, 5, 2, 2, 1, 1, 1, 5, 5, 5, 1,
			2, 5, 5, 5, 5, 5, 5, 2, 2, 1, 5, 5, 5, 5, 5, 1,
			2, 5, 5, 5, 5, 5, 5, 5, 1, 1, 5, 5, 5, 5, 5, 1,
			2, 5, 5, 5, 5, 5, 5, 2, 2, 5, 5, 5, 5, 5, 5, 1,
			2, 5, 5, 5, 5, 5, 5, 2, 2, 1, 5, 5, 5, 5, 5, 1,
			2, 5, 1, 5, 5, 5, 5, 5, 1, 1, 5, 5, 5, 1, 5, 1,
			2, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		};
		img.Fill(palette, pic, 16, 16, 32, 0);
	}
	// box[1]
	{
		uint8_t pic[] =
		{
			4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1,
			2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1,
			2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1,
			2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1,
			2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1,
			2, 2, 1, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 1, 2, 1,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		};
		img.Fill(palette, pic, 16, 16, 48, 0);
	}
	// box[2]
	{
		uint8_t pic[] =
		{
			4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4,
			2, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1,
			2, 6, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 6, 1,
			2, 6, 6, 6, 6, 2, 2, 2, 2, 2, 6, 6, 6, 6, 6, 1,
			2, 6, 6, 6, 2, 2, 1, 1, 1, 2, 2, 6, 6, 6, 6, 1,
			2, 6, 6, 6, 2, 2, 1, 6, 6, 2, 2, 1, 6, 6, 6, 1,
			2, 6, 6, 6, 2, 2, 1, 6, 6, 2, 2, 1, 6, 6, 6, 1,
			2, 6, 6, 6, 6, 1, 1, 6, 2, 2, 2, 1, 6, 6, 6, 1,
			2, 6, 6, 6, 6, 6, 6, 2, 2, 1, 1, 1, 6, 6, 6, 1,
			2, 6, 6, 6, 6, 6, 6, 2, 2, 1, 6, 6, 6, 6, 6, 1,
			2, 6, 6, 6, 6, 6, 6, 6, 1, 1, 6, 6, 6, 6, 6, 1,
			2, 6, 6, 6, 6, 6, 6, 2, 2, 6, 6, 6, 6, 6, 6, 1,
			2, 6, 6, 6, 6, 6, 6, 2, 2, 1, 6, 6, 6, 6, 6, 1,
			2, 6, 1, 6, 6, 6, 6, 6, 1, 1, 6, 6, 6, 1, 6, 1,
			2, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		};
		img.Fill(palette, pic, 16, 16, 64, 0);
	}

	m_texture = m_gs->SummonTexture(&img);

	return true;
}

void ExampleGameMario::Shutdown()
{
	BQ_SAFEDESTROY(m_texture);
	BQ_SAFEDESTROY(m_camera);
}

void ExampleGameMario::OnDraw()
{
	if (bqInput::IsKeyHit(bqInput::KEY_ESCAPE))
	{
		m_app->StopExample();
		return;
	}

	m_camera->Update(0.f);
	m_camera->m_viewProjectionMatrix = m_camera->m_projectionMatrix * m_camera->m_viewMatrix;
	
	m_gs->BeginGUI();
	m_gs->DrawGUIRectangle(bqVec4f(10.f, 10.f, 512.f, 512.f), bq::ColorWhite, bq::ColorWhite, m_texture, 0);
	m_gs->EndGUI();

	m_gs->BeginDraw();
	m_gs->ClearAll();

	m_app->DrawGrid(14, (float)m_camera->m_position.y);

	m_gs->EndDraw();
	m_gs->SwapBuffers();
}
