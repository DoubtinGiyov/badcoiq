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

#include "../../DemoApp.h"


ExampleBasics3DModel::ExampleBasics3DModel(DemoApp* app)
	:
	DemoExample(app)
{
}

ExampleBasics3DModel::~ExampleBasics3DModel()
{
}


bool ExampleBasics3DModel::Init()
{
	m_camera = new bqCamera();
	m_camera->m_position = bqVec3(10.f, 10.f, 10.f);
	m_camera->m_aspect = (float)m_app->GetWindow()->GetCurrentSize()->x / (float)m_app->GetWindow()->GetCurrentSize()->y;
	m_camera->SetType(bqCamera::Type::PerspectiveLookAt);
	m_camera->Update(0.f);
	m_camera->m_viewProjectionMatrix = m_camera->m_projection * m_camera->m_view;

	// для 3D линии
	bqFramework::SetMatrix(bqMatrixType::ViewProjection, &m_camera->m_viewProjectionMatrix);

	m_model = new MyModel(m_gs);
	m_model->Load(bqFramework::GetPath("../data/models/4_objs.obj").c_str());

	return true;
}

void ExampleBasics3DModel::MyModel::Load(const char* f)
{
	bqFramework::SummonMesh(f, &m_cb);
}

void ExampleBasics3DModel::Shutdown()
{
	BQ_SAFEDESTROY(m_model);
	BQ_SAFEDESTROY(m_camera);
}

void ExampleBasics3DModel::OnDraw()
{
	if (bqInput::IsKeyHit(bqInput::KEY_ESCAPE))
	{
		m_app->StopExample();
		return;
	}

	m_camera->Update(0.f);
	m_camera->m_viewProjectionMatrix = m_camera->m_projection * m_camera->m_view;

	if (bqInput::IsKeyHold(bqInput::KEY_A))
		m_camera->m_position.x += 10.0 * (double)(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_D))
		m_camera->m_position.x -= 10.0 * (double)(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_W))
		m_camera->m_position.z += 10.0 * (double)(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_S))
		m_camera->m_position.z -= 10.0 * (double)(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_Q))
		m_camera->m_position.y += 10.0 * (double)(*m_app->m_dt);
	if (bqInput::IsKeyHold(bqInput::KEY_E))
		m_camera->m_position.y -= 10.0 * (double)(*m_app->m_dt);


	m_gs->BeginGUI();
	m_gs->EndGUI();

	m_gs->BeginDraw();
	m_gs->ClearAll();

	m_gs->SetShader(bqShaderType::Standart, 0);
	bqMat4 W, WVP;
	bqFramework::SetMatrix(bqMatrixType::World, &W);
	WVP = m_camera->m_projection * m_camera->m_view * W;
	bqFramework::SetMatrix(bqMatrixType::WorldViewProjection, &WVP);
	bqMaterial material;
	material.m_shaderType = bqShaderType::Standart;
	material.m_sunPosition.Set(2.f, 2.f, 1.f);
	m_gs->SetMaterial(&material);
	for (size_t i = 0; i < m_model->m_gpuModels.m_size; ++i)
	{
		material.m_maps[0].m_texture = m_app->m_texture4x4;

		m_gs->SetMesh(m_model->m_gpuModels.m_data[i]);
		m_gs->Draw();
	}

	m_gs->SetShader(bqShaderType::Line3D, 0);
	m_gs->DrawLine3D(bqVec3(-1.f, 0.f, 0.f), bqVec3(1.f, 0.f, 0.f), bq::ColorRed);
	m_gs->DrawLine3D(bqVec3(0.f, -1.f, 0.f), bqVec3(0.f, 1.f, 0.f), bq::ColorYellow);
	m_gs->DrawLine3D(bqVec3(0.f, 0.f, -1.f), bqVec3(0.f, 0.f, 1.f), bq::ColorLime);
	m_gs->EndDraw();
	m_gs->SwapBuffers();
}
