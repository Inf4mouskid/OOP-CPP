#include "stdafx.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

EnemyCharacter::EnemyCharacter()
{
	Health = 0;
}

EnemyCharacter::~EnemyCharacter()
{
}

// Determines the monsters health by the players XP
void EnemyCharacter::SetMonsterHP(short XP)
{
	Health = rand() % 1 + 5;
	if (XP >= 10)
		Health = rand() % 10 + 5;
	else if (XP >= 25)
		Health = rand() % 20 + 10;
	else if (XP >= 45)
		Health = rand() % 30 + 15;
	else if (XP >= 65)
		Health = rand() % 30 + 20;
	else if (XP >= 85)
		Health = rand() % 35 + 20;
}

// Returns the monsters health
short EnemyCharacter::GetMonsterHP()
{
	return Health;
}

