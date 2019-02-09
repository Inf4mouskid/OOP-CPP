#include "stdafx.h"
#include "RReaper.h"


RReaper::RReaper()
{
}

RReaper::~RReaper()
{
}

short RReaper::attack(PlayerCharacter Player)
{
	short PlayerHealth = Player.GetPlayerHealth();
	short DealDamage;

	if (Player.GetPlayerAlignment())
		DealDamage = PlayerHealth / 2;
	else
		DealDamage = rand() % 25 + 1;

	return DealDamage;
}
