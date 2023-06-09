#pragma once
#include "AkResource.h"

namespace Ak
{
	class Resources
	{
	public:
		template <typename T>
		static T* Find(const std::wstring& _key)
		{
			//리소스맵에서 데이터를 탐색한다 데이터가 있다면 해당데이터를 반환하고
			//데이터가 없다면 end를 반환해준다.
			std::map<std::wstring, Resource*>::iterator iter = mResources.find(_key);

			//찾고자 하는 데이터가 존재한다면
			//해당타입으로 형변환하여 반환
			if (iter != mResources.end())
			{
				return dynamic_cast<T*>(iter->second);
			}

			//데이터 없다면 널을 반환
			return nullptr;
		}

		template <typename T>
		static T* Load(const std::wstring& _key, const std::wstring& _path)
		{
			// 키값으로 탐색
			T* resource = Resources::Find<T>(_key);
			if (resource != nullptr)
			{
				return resource;
			}

			// 해당 리소스가 없다면
			resource = new T();
			if (FAILED(resource->Load(_path)))
			{
				assert(false);
				return nullptr;
			}

			resource->SetKey(_key);
			resource->SetPath(_path);
			mResources.insert(std::make_pair(_key, resource));

			return dynamic_cast<T*>(resource);
		}

		template <typename T>
		static void Insert(const std::wstring& _key, T* _resource)
		{
			mResources.insert(std::make_pair(_key, _resource));
		}

		static void Release()
		{
			for (auto pair : mResources)
			{
				delete pair.second;
				pair.second = nullptr;
			}
		}

	private:
		static std::map<std::wstring, Resource*> mResources;
	};
}

