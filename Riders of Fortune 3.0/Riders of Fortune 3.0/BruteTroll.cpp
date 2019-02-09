#include "stdafx.h"
#include "BruteTroll.h"

using namespace std;

BruteTroll::BruteTroll()
{
}


BruteTroll::~BruteTroll()
{
}

short BruteTroll::attack(PlayerCharacter Player)
{
	//Variables
	string CurrentWeapon = Player.ShowActiveWeapon();
	short PlayerHealth = Player.GetPlayerHealth();
	short DealDamage;

	// Actack condition
	if (CurrentWeapon == "Mace" || CurrentWeapon == "Flail" || CurrentWeapon == "Broad sword")
		DealDamage = PlayerHealth;
	else
		DealDamage = rand() % 25 + 1;

	return DealDamage;
}
