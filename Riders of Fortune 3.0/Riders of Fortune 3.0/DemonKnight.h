#pragma once
#include "EnemyCharacter.h"
class DemonKnight :
	public EnemyCharacter
{
public:
	DemonKnight();
	~DemonKnight();
	virtual short attack(PlayerCharacter, int, int);
};

