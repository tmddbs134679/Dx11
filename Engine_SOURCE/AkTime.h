#pragma once
#include "Anack_Engine.h"

namespace Ak
{
	class Time
	{
	public:
		static void Initiailize();
		static void Update();
		static void Render();

		__forceinline static double DeltaTime()
			{ 
				return mDeltaTime;
			}

	private:

		static double mDeltaTime;
		static double mSecond;
		static LARGE_INTEGER mCpuFrequency;
		static LARGE_INTEGER mPrevFrequency;
		static LARGE_INTEGER mCurFrequency;
	};

}