#pragma once
#include "EnemyCharacter.h"
class BruteTroll :
	public EnemyCharacter
{
public:
	BruteTroll();
	~BruteTroll();
	virtual short attack(PlayerCharacter);
};
