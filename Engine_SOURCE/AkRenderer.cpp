#include "AkRenderer.h"

namespace Ak::renderer
{
	
	Vertex vertexes[4] = {};
	Ak::Mesh* mesh = nullptr;
	Ak::Shader* shader = nullptr;
	Ak::graphics::ConstantBuffer* constantBuffer = nullptr;



	void SetUpState()
	{
		//Input Layout정점 구조 정보를 넘겨줘야한다.
		D3D11_INPUT_ELEMENT_DESC ArrLayOut[2] = {};

		//Pos
		ArrLayOut[0].AlignedByteOffset = 0;
		ArrLayOut[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ArrLayOut[0].InputSlot = 0;
		ArrLayOut[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ArrLayOut[0].SemanticName = "POSITION";
		ArrLayOut[0].SemanticIndex = 0;

		//Color
		ArrLayOut[1].AlignedByteOffset = 12;
		ArrLayOut[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		ArrLayOut[1].InputSlot = 0;
		ArrLayOut[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ArrLayOut[1].SemanticName = "COLOR";
		ArrLayOut[1].SemanticIndex = 0;

		Ak::graphics::GetDevice()->CreateInputLayout(
			ArrLayOut,
			2,
			shader->GetVSCode(),
			shader->GetInputLayoutAddressOf());

	}

	void LoadBuffer()
	{

		mesh = new Ak::Mesh();
		mesh->CreateVertexBuffer(vertexes, 4);

		std::vector<UINT> indexes = {};
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);
		mesh->CreateIndexBuffer(indexes.data(), indexes.size());

		constantBuffer = new Ak::graphics::ConstantBuffer(eCBType::Transform);

		constantBuffer->Create(sizeof(Vector4));

		Vector4 pos(0.2f, 0.0f, 0.0f, 1.0f);
		constantBuffer->SetData(&pos);
		constantBuffer->Bind(eShaderStage::VS);

	}

	void LoadShader()
	{
		shader = new Ak::Shader();
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");
	}

	
	void Initialize()
	{
		vertexes[0].Pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[1].Pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].Pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[3].Pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);


		LoadBuffer();
		LoadShader();
		SetUpState();
	}

	void Release()
	{
		delete mesh;
		delete shader;
		delete constantBuffer;
	}

}