#pragma once
#include "AkGraphics.h"

namespace Ak::graphics
{
	class ConstantBuffer 
		: public GpuBuffer
	{
	public:
		ConstantBuffer(const eCBType _type);
		~ConstantBuffer();


		bool Create(size_t _size);
		void SetData(void* _data);
		void Bind(eShaderStage _stage);

	
	private:
		const eCBType mType;


	};

}
