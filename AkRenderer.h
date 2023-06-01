#pragma once
#include "Anack_Engine.h"
#include"AkGraphicDevice_Dx11.h"


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

	extern ID3D11Buffer* TriangleBuffer;
	extern ID3D11Buffer* TriangleIdxBuffer;
	extern ID3D11Buffer* TriangleConstantBuffer;

	extern ID3DBlob* errorBlob;
	extern ID3DBlob* TriangleVSBlob;
	extern ID3DBlob* TrianglePSBlob;

	extern ID3D11VertexShader* TriangleVSShader;

	extern ID3D11PixelShader* TrianglePSShader;
	
}

