#pragma once
#include "Anack_Engine.h"



namespace Ak
{
	class Entity
	{
	public:
		Entity();
		~Entity();

	public:
		void SetName(const std::wstring& _Name)
		{
			mName = _Name;
		}

		void SetID(UINT64 _id)
		{
			mID = _id;
		}

	public:

		std::wstring& GetName()
		{
			return mName;
		}

		UINT64	GetID()
		{
			return mID;
		}

	private:

		std::wstring mName;
		UINT64 mID;

	};


}
