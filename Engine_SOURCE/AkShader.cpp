#include "AkShader.h"

Ak::Shader::Shader()
    : mInputLayout(nullptr)
    , mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

Ak::Shader::~Shader()
{
    mInputLayout->Release();
}

HRESULT Ak::Shader::Load(const std::wstring& path)
{
	return E_NOTIMPL;
}


bool Ak::Shader::Create(const eShaderStage _stage, const std::wstring& _fileName, const std::string& _funName)
{
	std::filesystem::path shaderPath
		= std::filesystem::current_path().parent_path();
	shaderPath += L"\\Shader_SOURCE\\";

	std::filesystem::path fullPath(shaderPath.c_str());
	fullPath += _fileName;

	ID3DBlob* errorBlob = nullptr;
	if (_stage == eShaderStage::VS)
	{
		GetDevice()->CompileFromfile(fullPath, _funName, "vs_5_0", mVSBlob.GetAddressOf());
		GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer()
			, mVSBlob->GetBufferSize(), mVS.GetAddressOf());
	}
	else if (_stage == eShaderStage::PS)
	{
		GetDevice()->CompileFromfile(fullPath, _funName, "ps_5_0", mPSBlob.GetAddressOf());
		GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer()
			, mPSBlob->GetBufferSize(), mPS.GetAddressOf());
	}


	return true;;
}

void Ak::Shader::Binds()
{
	GetDevice()->BindVertexShader(mVS.Get());
	GetDevice()->BindPixelShader(mPS.Get());
}
