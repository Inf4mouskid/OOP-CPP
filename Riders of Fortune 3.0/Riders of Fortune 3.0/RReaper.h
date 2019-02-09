#pragma once
#include "EnemyCharacter.h"
class RReaper :
	public EnemyCharacter
{
public:
	RReaper();
	~RReaper();
	virtual short attack(PlayerCharacter Player);
};
