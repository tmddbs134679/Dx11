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
			//���ҽ��ʿ��� �����͸� Ž���Ѵ� �����Ͱ� �ִٸ� �ش絥���͸� ��ȯ�ϰ�
			//�����Ͱ� ���ٸ� end�� ��ȯ���ش�.
			std::map<std::wstring, Resource*>::iterator iter = mResources.find(_key);

			//ã���� �ϴ� �����Ͱ� �����Ѵٸ�
			//�ش�Ÿ������ ����ȯ�Ͽ� ��ȯ
			if (iter != mResources.end())
			{
				return dynamic_cast<T*>(iter->second);
			}

			//������ ���ٸ� ���� ��ȯ
			return nullptr;
		}

		template <typename T>
		static T* Load(const std::wstring& _key, const std::wstring& _path)
		{
			// Ű������ Ž��
			T* resource = Resources::Find<T>(_key);
			if (resource != nullptr)
			{
				return resource;
			}

			// �ش� ���ҽ��� ���ٸ�
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

