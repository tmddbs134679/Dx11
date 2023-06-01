#include "AkRenderer.h"

namespace Ak::renderer
{
	//삼각형2개 :(6)개,  사각형 6개,  원 33개
	Vertex vertexes[12] = {};

	Vector4	Pos(0.f, 0.f, 0.f, 1.f);

	//Input Layout (정점 정보)

	ID3D11InputLayout* TriangleLayout = nullptr;


	// Buffer
	ID3D11Buffer* TriangleBuffer = nullptr;
	ID3D11Buffer* TriangleIdxBuffer = nullptr;
	ID3D11Buffer* TriangleConstantBuffer = nullptr;


	//error blob
	ID3DBlob* errorBlob = nullptr;

	//Vertex Shader code -> Binary Code
	ID3DBlob* TriangleVSBlob = nullptr;

	//Vertex Shader
	ID3D11VertexShader* TriangleVSShader = nullptr;

	//Pixel Shader code -> Binary Code
	ID3DBlob* TrianglePSBlob = nullptr;

	//Vertex Shader
	ID3D11PixelShader* TrianglePSShader = nullptr;


	void SetUpState()
	{

	}

	void LoadBuffer()
	{
		//버텍스 버퍼
		D3D11_BUFFER_DESC TriangleDesc = {};
		TriangleDesc.Usage = D3D11_USAGE_DYNAMIC;
		TriangleDesc.ByteWidth = sizeof(Vertex) * 12;
		TriangleDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		TriangleDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;


		D3D11_SUBRESOURCE_DATA TriangleData = {};
		TriangleData.pSysMem = vertexes;
		Ak::graphics::GetDivce()->CreateBuffer(&TriangleBuffer, &TriangleDesc, &TriangleData);

		//인덱스 버퍼

		std::vector<UINT> indexes = {};
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		D3D11_BUFFER_DESC TriangleIdxDesc = {};

		TriangleIdxDesc.ByteWidth = sizeof(UINT) * indexes.size();
		TriangleIdxDesc.Usage = D3D11_USAGE_DEFAULT;
		TriangleIdxDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		TriangleIdxDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA TriangleIdxData = {};
		TriangleIdxData.pSysMem = indexes.data();
		Ak::graphics::GetDivce()->CreateBuffer(&TriangleIdxBuffer, &TriangleIdxDesc, &TriangleIdxData);

		//상수 버퍼(Constant Buffer)
		
		D3D11_BUFFER_DESC TriangleCSDesc = {};
		TriangleCSDesc.Usage = D3D11_USAGE_DYNAMIC;
		TriangleCSDesc.ByteWidth = sizeof(Vector4);
		TriangleCSDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		TriangleCSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		Ak::graphics::GetDivce()->CreateBuffer(&TriangleConstantBuffer, &TriangleCSDesc, nullptr);


		Ak::graphics::GetDivce()->SetConstantBuffer(TriangleConstantBuffer, &Pos, sizeof(Vector4));
		Ak::graphics::GetDivce()->BindConstantBuffer(eShaderStage::VS, eCBType::Transform, TriangleConstantBuffer);

	}

	void LoadShader()
	{
		Ak::graphics::GetDivce()->CreateShader();
	}

	
	void Initialize()
	{
		//삼격형 2개(별)
		vertexes[0].Pos = Vector3(-0.5f, 0.75, 0.f);
		vertexes[0].color = Vector4(1.f, 0.f, 0.f, 1.f);

		vertexes[1].Pos = Vector3(-0.25f, 0.25f, 0.f);
		vertexes[1].color = Vector4(1.f, 0.f, 0.f, 1.f);

		vertexes[2].Pos = Vector3(-0.75f, 0.25f, 0.f);
		vertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
	

		vertexes[3].Pos = Vector3(-0.75f, 0.66, 0.5f);
		vertexes[3].color = Vector4(1.f, 0.f, 0.f, 1.f);

		vertexes[4].Pos = Vector3(-0.25f, 0.66f, 0.5f);
		vertexes[4].color = Vector4(1.f, 0.f, 0.f, 1.f);

		vertexes[5].Pos = Vector3(-0.5f, 0.16f, 0.5f);
		vertexes[5].color = Vector4(1.f, 0.f, 0.f, 1.f);



		//사각형
		vertexes[6].Pos = Vector3(0.25f, 0.75, 0.5f);
		vertexes[6].color = Vector4(1.f, 0.f, 0.f, 1.f);

		vertexes[7].Pos = Vector3(0.75f, 0.75f, 0.5f);
		vertexes[7].color = Vector4(0.f, 1.f, 0.f, 1.f);

		vertexes[8].Pos = Vector3(0.25f, 0.25f, 0.5f);
		vertexes[8].color = Vector4(0.f, 0.f, 1.f, 1.f);


		vertexes[9].Pos = Vector3(0.75f, 0.75f, 0.f);
		vertexes[9].color = Vector4(0.f, 1.f, 0.f, 1.f);

		vertexes[10].Pos = Vector3(0.75f, 0.25f, 0.5f);
		vertexes[10].color = Vector4(1.f, 0.f, 0.f, 1.f);

		vertexes[11].Pos = Vector3(0.25f, 0.25f, 0.5f);
		vertexes[11].color = Vector4(0.f, 0.f, 1.f, 1.f);


		////원
		//const int numSegment = 32;
		//const float radius = 0.5f;

		//vertexes[12].Pos = Vector3(0.f, 0.f, 0.f);
		//vertexes[12].color = Vector4(1.f, 1.f, 1.f, 1.f);


		//float angle = 0.f;
		//float angleIn = 2.f * DirectX::XM_PI / (float)numSegment;

		//for (int i = 1; i <= numSegment; ++i)
		//{
		//	vertexes[i + 12].Pos = Vector3(radius * cos(angle), radius * sin(angle), 0.f);
		//	vertexes[i + 12].color = Vector4(1.f, 1.f, 1.f, 1.f);

		//	angle += angleIn;
		//}


		SetUpState();
		LoadBuffer();
		LoadShader();
	}


}