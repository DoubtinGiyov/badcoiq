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


ExampleSceneCameraFly::ExampleSceneCameraFly(DemoApp* app)
	:
	DemoExample(app)
{
}

ExampleSceneCameraFly::~ExampleSceneCameraFly()
{
}


bool ExampleSceneCameraFly::Init()
{
	m_camera = new bqCamera();
	m_camera->m_position = bqVec3(3.f, 3.f, 3.f);
	m_camera->m_aspect = (float)m_app->GetWindow()->GetCurrentSize()->x / (float)m_app->GetWindow()->GetCurrentSize()->y;
	m_camera->Rotate(36.f, -45.f, 0.f);
	m_camera->SetType(bqCamera::Type::Perspective);
	m_camera->Update(0.f);
	m_camera->m_viewProjectionMatrix = m_camera->m_projectionMatrix * m_camera->m_viewMatrix;

	// для 3D линии
	bqFramework::SetMatrix(bqMatrixType::ViewProjection, &m_camera->m_viewProjectionMatrix);

	return true;
}

void ExampleSceneCameraFly::Shutdown()
{
	BQ_SAFEDESTROY(m_camera);
}

void ExampleSceneCameraFly::OnDraw()
{
	if (bqInput::IsKeyHit(bqInput::KEY_ESCAPE))
	{
		m_app->StopExample();
		return;
	}

	m_camera->Update(0.f);
	m_camera->m_viewProjectionMatrix = m_camera->m_projectionMatrix * m_camera->m_viewMatrix;

	if (bqInput::IsKeyHold(bqInput::KEY_SPACE))
	{
		m_camera->Rotate(bqInput::GetData()->m_mouseMoveDelta, *m_app->m_dt);

		// move cursor to center of the screen
		bqPoint windowCenter;
		m_app->GetWindow()->GetCenter(windowCenter);
		bqInput::SetMousePosition(m_app->GetWindow(), windowCenter.x, windowCenter.y);
	}

	if (bqInput::IsKeyHold(bqInput::KEY_A))
		m_camera->MoveLeft(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_D))
		m_camera->MoveRight(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_W))
		m_camera->MoveForward(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_S))
		m_camera->MoveBackward(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_Q))
		m_camera->MoveDown(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_E))
		m_camera->MoveUp(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_R))
		m_camera->Rotate(0.f, 0.f, 10.f * *m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_F))
		m_camera->Rotate(0.f, 0.f, -10.f * *m_app->m_dt);

	m_gs->BeginGUI();
	m_gs->EndGUI();

	m_gs->BeginDraw();
	m_gs->ClearAll();

	m_gs->SetShader(bqShaderType::Line3D, 0);
	m_gs->DrawLine3D(bqVec3(-1.f, 0.f, 0.f), bqVec3(1.f, 0.f, 0.f), bq::ColorRed);
	m_gs->DrawLine3D(bqVec3(0.f, -1.f, 0.f), bqVec3(0.f, 1.f, 0.f), bq::ColorYellow);
	m_gs->DrawLine3D(bqVec3(0.f, 0.f, -1.f), bqVec3(0.f, 0.f, 1.f), bq::ColorLime);

	m_gs->EndDraw();
	m_gs->SwapBuffers();
}
