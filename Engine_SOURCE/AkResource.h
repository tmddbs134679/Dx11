#pragma once
#include "AkEntity.h"

namespace Ak
{
	class Resource
	{
	public:
		Resource();
		virtual ~Resource();

		virtual HRESULT Load(const std::wstring& path) = 0;

		const std::wstring& GetKey() 
		{ 
			return mKey;
		}

		const std::wstring& GetPath() 
		{ 
			return mPath; 
		}

		void SetKey(const std::wstring& _key)
		{
			mKey = _key;
		}

		void SetPath(const std::wstring& _path)
		{
			mPath = _path;
		}

	private:
		std::wstring mKey;
		std::wstring mPath;
	};
}
