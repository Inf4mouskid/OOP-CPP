#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

// A class that creates a player object
class PlayerCharacter
{
private:
	short HP;
	short XP;
	string ActiveWeapon;
	short PlayerX;
	short PlayerY;
	bool Alignment;
public:
	PlayerCharacter();
	~PlayerCharacter();

	// Sets the player's health
	void SetPlayerHealth(short);

	// Gets the player's health
	short GetPlayerHealth();

	// Sets the player's Experience
	void SetPlayerXP(short);

	// Gets the player's Experience
	short GetPlayerXP();

	// Sets the X position
	void SetPlayerX(short);

	// Sets the Y position
	void SetPlayerY(short);

	// Gets the X position
	int GetPlayerX();

	// Gets the Y position
	int GetPlayerY();

	// Sets the player's alignment to GOOD or EVIL
	// True = GOOD, False = EVIL
	void SetPlayerAlignment(bool);

	// Gets the player's alignment (GOOD/EVIL)
	// True = GOOD, False = EVIL
	bool GetPlayerAlignment();

	void SetActiveWeapon(string);

	string ShowActiveWeapon();
};