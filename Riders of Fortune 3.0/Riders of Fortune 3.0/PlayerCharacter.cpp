#include "stdafx.h"
#include "PlayerCharacter.h"
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>

using namespace std;

PlayerCharacter::PlayerCharacter()
{
	HP = 0;
	XP = 0;
	ActiveWeapon = "";
	PlayerX = 0;
	PlayerY = 0;
	Alignment = true;
}


PlayerCharacter::~PlayerCharacter()
{
}

// Sets the player's health
void PlayerCharacter::SetPlayerHealth(short Health)
{
	HP = Health;
}

// Gets the player's health
short PlayerCharacter::GetPlayerHealth()
{
	return HP;
}

// Sets the player's Experience
void PlayerCharacter::SetPlayerXP(short Exp)
{
	XP = Exp;
}

// Gets the player's Experience
short PlayerCharacter::GetPlayerXP()
{
	return XP;
}

// Sets the X position
void PlayerCharacter::SetPlayerX(short x)
{
	PlayerX = x;
}

// Sets the Y position
void PlayerCharacter::SetPlayerY(short y)
{
	PlayerY = y;
}

// Gets the X position
int PlayerCharacter::GetPlayerX()
{
	return PlayerX;
}

// Gets the Y position
int PlayerCharacter::GetPlayerY()
{
	return PlayerY;
}

// Sets the players alignment to GOOD or EVIL
// True = GOOD, False = EVIL
void PlayerCharacter::SetPlayerAlignment(bool PlayerAlignment)
{
	Alignment = PlayerAlignment;
}

// Gets the players alignment (GOOD/EVIL)
// True = GOOD, False = EVIL
bool PlayerCharacter::GetPlayerAlignment()
{
	return Alignment;
}

//Set's the active weapon
void PlayerCharacter::SetActiveWeapon(string Weapon)
{
	ActiveWeapon = Weapon;
}

//Get's the active weapon
string PlayerCharacter::ShowActiveWeapon()
{
	return ActiveWeapon;
}

