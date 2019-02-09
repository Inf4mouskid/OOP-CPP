#include "PlayerCharacter.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;

#pragma once
class EnemyCharacter
{
private:
	short int Health;
public:
	// Default constructor
	EnemyCharacter();

	// Destructor
	~EnemyCharacter();

	// Set's the monsters health based on player experience
	void SetMonsterHP(short);

	// Get's the monsters health
	short GetMonsterHP();
};
