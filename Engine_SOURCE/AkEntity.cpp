#include "AkEntity.h"



namespace Ak
{
	Ak::Entity::Entity()
		: mName(L"")
		,mID((UINT64)this)
	{
	}

	Ak::Entity::~Entity()
	{
	}

}