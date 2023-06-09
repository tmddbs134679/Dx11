#pragma once
#include "Anack_Engine.h"
#include"AkGraphicDevice_Dx11.h"
#include "AkMesh.h"
#include "AkShader.h"
#include "AkConstantBuffer.h"


using namespace Ak::math;

namespace Ak::renderer
{
	struct Vertex
	{
		Vector3 Pos;
		Vector4 color;
	};


	void Initialize();

	extern Vector4 Pos;

	extern Vertex vertexes[];
	extern ID3D11InputLayout* TriangleLayout;

	extern Ak::Mesh* mesh;
	extern Ak::Shader* shader;
	extern Ak::graphics::ConstantBuffer* constantBuffer;
	
}

