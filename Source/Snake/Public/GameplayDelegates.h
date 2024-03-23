#pragma once

namespace GameplayDelegates
{
	//This is how you implement a global event in unreal
	DECLARE_MULTICAST_DELEGATE(FGameOver);
	extern FGameOver OnGameOver; //Belongs to Food

}
