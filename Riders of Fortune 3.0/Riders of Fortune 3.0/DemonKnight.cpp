#include "stdafx.h"
#include "DemonKnight.h"


DemonKnight::DemonKnight()
{
}


DemonKnight::~DemonKnight()
{
}

short DemonKnight::attack(PlayerCharacter Player, int InitialHealth, int MapSize)
{
	// Variables
	short PlayerX = Player.GetPlayerX();
	short PlayerY = Player.GetPlayerY();
	short FixturePoint = MapSize / 2;
	short DealDamage;

	// Attack condition
	if (
		(PlayerX == FixturePoint && PlayerY == 4) ||
		(PlayerX == FixturePoint && PlayerY == MapSize) ||
		(PlayerX == MapSize && PlayerY == 0) ||
		(PlayerX == MapSize && PlayerY == MapSize)
		)
		DealDamage = InitialHealth * 0.6;
	else
		DealDamage = rand() % 25 + 1;

	return DealDamage;
}
