#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <ctime>
#include <cmath>

using namespace std;

//Global Varialbes
string Weapons[6] = { "Knife", "Crossbow", "Flail", "Broad Sword", "Dragon Slayer", "The Spell of the Gods" };
string MonsterName[5] = { "Ogre", "Troll", "Serpent", "Gargoyle", "Dragon" };
short unsigned int PlayerPos = 0;
const int SleepTimer = 3500;
short int MonsterHealth = 0;
short int ActiveWeapon = 0;
short int AttackPower = 0;
short int sapce = 0;
short int XP;
short int randomRoll;
short int Monster;
bool InCombat = false;
bool HasMoved = false;
int Map[28];

//Prototypes
void DiceRoll();
void MapGenerator();
void DisplayMap();
void PlayerMovement();
void PlayerInteraction();
void PlayerStats();
void MonsterGen();
void HUD_UI();
void WeaponSystem();
void MonsterDescriptions();
void DragonFight();
void GodMode();

int main()
{
	unsigned seed = time(0);
	char Option, Restart;
	XP = 0;

	srand(seed);	//Seed rand()

	do
	{
		//Description of the game.
		cout << "Welcome to rider's of fortune!\n";
		cout << "You play as Yarra, a young horseman in \n";
		cout << "search of the illustrious Chalice of Knowledge,\n";
		cout << "an artifact that will bring you everlasting wisdom.\n";
		cout << "The Chalice can only be found in the dragon's dungeon after defeating the dragon.\n\n";

		//Load player info and the map
		MapGenerator();
		HUD_UI();

		//Loop until player reaches dragon!
		while (Map[27] != 15)
		{
			cout << "\nPress 1 to move\n";
			cout << "Press 2 to explore (interact with area)\n";
			cout << "Press 0 Exit\n";
			cin >> Option;

			//Checks if the options selected are appropriate
			if (Option == '1' || Option == '2' || Option == '0' || Option == '8' || Option == '9')
			{
				//Options menu
				switch (Option)
				{
				case '1':
					//Move on the map
					PlayerMovement();
					break;
				case '2':
					//Interact with environmment
					if (HasMoved == false)
					{
						system("cls");
						cout << "You have not moved!\n";
					}
					else
					{
						MonsterGen();
						PlayerInteraction();
						HasMoved = false;
					}
					break;
				case '0':
					//quit game
					exit(0);
				case '8':
					//Start with a good weapon
					ActiveWeapon = 5;
					AttackPower = 7;
					cout << "Best weapon: " << Weapons[ActiveWeapon] << " found!";
					break;
				case '9':
					//Scripted event to desimate the dragon
					GodMode();
					exit(0);
				default:
					cout << "Sorry your input was invalid. Please try again!\n";
				}
			}
			else
			{
				system("cls");
				HUD_UI();
			}
		}

		//Determine player victory
		system("cls");
		if (XP >= 5)
		{
			DragonFight();
			cout << "Due to your cunning skill and experience,\n";
			cout << "you have defeated the deadly dragon. Your quest has ended good sir.\n";
			cout << "You've obtained the Chalice of knowledge and all of earth's mysteries are revealed!";
		}
		else
		{
			cout << "Alas, the dragon's eyes stare at you and places you under his spell.\n";
			cout << "You try to move but fail to do so and find yourself torched by the dragon’s fire.\n";
			cout << "If only you had more experience, you could have seen it coming.";
		}
		cout << "\nWould you like to Play again? (Y/N)\n";
		cin >> Restart;
	} while (Restart == 'Y' || Restart == 'y');

	Sleep(SleepTimer + 6500);

	return 0;
}

void DiceRoll()
{
	//Rolls a D6
	randomRoll = rand() % 6 + 1;

	//Check if player is in combat
	if (InCombat == true)
	{
		cout << "You delt " << randomRoll << " + " << AttackPower << " = ";
		cout << randomRoll + AttackPower << " damage!\n";
	}
	else
		cout << "You rolled a " << randomRoll << endl;

	//Wait timer
	Sleep(SleepTimer - 2000);
}

void MapGenerator()
{
	//Creates monsters, weapons, dragon to the map
	int randspace = 0;

	//Create empty spaces
	for (int i = 0; i < 28; i++)
		Map[i] = 0;

	//Set space for the Dragon
	Map[27] = 14;

	//Set space for the player
	Map[0] = 1;
	PlayerPos = 0;

	//Generates random 
	randspace = rand() % 27;

	int counter = 0;

	//Generate Monsters
	while (counter < 14)
	{
		if (Map[randspace] == 0)
		{
			//set space to monster value
			Map[randspace] = 2;
			counter++;
		}
		else
		{
			//New random number
			randspace = rand() % 27;
		}
	}

	//Reset variable
	counter = 0;

	//Generate weapons
	while (counter < 5)
	{
		//Generate Weapons
		if (Map[randspace] == 0)
		{
			if (counter == 0)
			{
				//set space to Crossbow value
				Map[randspace] = 4;
				counter++;
			}
			else if (counter == 1)
			{
				//set space to Flail value
				Map[randspace] = 6;
				counter++;
			}
			else if (counter == 2)
			{
				//set space to Broadsword value
				Map[randspace] = 8;
				counter++;
			}
			else if (counter == 3)
			{
				//set space to Dragonslayer value
				Map[randspace] = 10;
				counter++;
			}
			else if (counter == 4)
			{
				//set space to Spell of the Gods value
				Map[randspace] = 12;
				counter++;
			}
		}
		else
		{
			//New random number
			randspace = rand() % 27;
		}
	}
}

void DisplayMap()
{
	//Decode and display the map
	for (int i = 0; i < 28; i++)
	{
		if ((Map[i] % 2) == 0)
		{
			if (Map[i] == 14)
			{
				cout << "D";
			}
			else
			{
				cout << "* ";
			}
		}
		else if (Map[i] == 14)
		{
			cout << "D";
		}
		else
		{
			cout << "P ";
		}
	}
	cout << " " << endl;
}

void PlayerMovement()
{
	//Initiate how many spaces the player should move
	DiceRoll();
	if ((PlayerPos + randomRoll) < 27)
	{
		Map[PlayerPos] = Map[PlayerPos] - 1;
		PlayerPos += randomRoll;
		Map[PlayerPos] = Map[PlayerPos] + 1;
		system("cls");
		HUD_UI();
		HasMoved = true;
	}
	else if ((PlayerPos + randomRoll) >= 27)
	{
		Map[PlayerPos] = Map[PlayerPos] - 1;
		PlayerPos = 27;
		Map[27] = 15;
		system("cls");
		HUD_UI();
		HasMoved = true;
	}
}

void PlayerInteraction()
{
	//Functionality for interacting with the environment
	char Restart;
	char Choice = ' ';

	//Defines each outcome of exploration
	if (Map[PlayerPos] == 1)
	{
		//Outcome for an empty space
		system("cls");
		cout << "There is nothing for you to do, so you reflect upon your adventures thus far.\n";
		cout << "You take the time to train and enhance your reflexes.\n";
		XP++;
		HUD_UI();
		return;
	}
	else if (Map[PlayerPos] == 3)
	{
		//Outcome for attacking a monster
		while (Choice != '1')
		{
			system("cls");
			InCombat = true;
			cout << "*A " << MonsterName[Monster] << " approaches*\n";
			cout << "Your " << Weapons[ActiveWeapon] << " is at the ready.\n\n";
			cout << MonsterName[Monster] << " | Health: " << MonsterHealth << endl;

			//Fight options
			cout << "Now fight!\n";
			cout << "Press 1 to use " << Weapons[ActiveWeapon] << endl;
			cout << "Press 2 for info on " << MonsterName[Monster] << endl;
			cin >> Choice;

			switch (Choice)
			{
			case '1':
				//Attack Monster
				cout << "You attacked the " << MonsterName[Monster] << "!\n";
				DiceRoll();
				randomRoll += AttackPower;
				MonsterHealth -= randomRoll;
				break;
			case '2':
				//Monster descrption
				MonsterDescriptions();
				break;
			default:
				cout << "Sorry your input was invalid. Please try again!\n";
			}
		}

		if (MonsterHealth <= 0)
		{
			XP += 2;
			cout << "You beat the " << MonsterName[Monster] << "!\n";
			Sleep(SleepTimer);
			InCombat = false;
			system("cls");
			HUD_UI();
		}
		else
		{
			//end game and to restart
			cout << "You loose!";
			Sleep(3000);
			system("cls");
			cout << "Would you like to play again? (Y/N)\n";
			cin >> Restart;
			if (Restart == 'Y' || Restart == 'y')
				main();
			else
				exit(0);
		}
	}
	else if (Map[PlayerPos] > 4 && Map[PlayerPos] < 14)
	{
		system("cls");
		WeaponSystem();
	}
}

void PlayerStats()
{
	//Creates player info menu
	cout << "XP: " << XP;
	cout << " | Current weapon: " << Weapons[ActiveWeapon];
	cout << " | HP: " << 100 << endl;
}

void MonsterGen()
{
	//Generate health for each monster
	MonsterHealth = rand() % 4 + 3;

	//Selects random monster
	Monster = rand() % 4;
}

void HUD_UI()
{
	//Function for loading the ui
	PlayerStats();
	DisplayMap();
}

void WeaponSystem()
{
	if (Map[PlayerPos] == 5 && ActiveWeapon < 1)
	{
		//Outcome for finding crossbow
		ActiveWeapon = 1;
		AttackPower = 3;
		cout << Weapons[ActiveWeapon] << " aquired!" << endl;
	}
	else if (Map[PlayerPos] == 7 && ActiveWeapon < 2)
	{
		//Outcome for finding Flail
		ActiveWeapon = 2;
		AttackPower = 4;
		cout << Weapons[ActiveWeapon] << " aquired!" << endl;
	}
	else if (Map[PlayerPos] == 9 && ActiveWeapon < 3)
	{
		//Outcome for finding Broad sword
		ActiveWeapon = 3;
		AttackPower = 5;
		cout << Weapons[ActiveWeapon] << " aquired!" << endl;
	}
	else if (Map[PlayerPos] == 11 && ActiveWeapon < 4)
	{
		//Outcome for finding Dragon slayer
		ActiveWeapon = 4;
		AttackPower = 6;
		cout << Weapons[ActiveWeapon] << " aquired!" << endl;
	}
	else if (Map[PlayerPos] == 13)
	{
		//Outcome for finding a weapon
		ActiveWeapon = 5;
		AttackPower = 7;
		cout << Weapons[ActiveWeapon] << " aquired!" << endl;
	}
	else
	{
		//Outcome for finding a weak weapon
		cout << "You found something... \n";
		cout << "..but you realize you have something better\n";
		cout << "and decide to carry on...";
	}
}

void MonsterDescriptions()
{
	//Function for loading monster descriptions
	system("cls");
	if (MonsterName[Monster] == "Ogre")
	{
		//Description of the Ogre
		cout << MonsterName[Monster] << endl;
		cout << "Fat ugly being that loves the taste of eye juice.\n";
		cout << "Especially on toast...";
	}
	else if (MonsterName[Monster] == "Troll")
	{
		//Description of the Troll
		cout << MonsterName[Monster] << endl;
		cout << "Giant lumbering beast with a snot problem.\n";
		cout << "Can someone be a dear and get him a rag?";
	}
	else if (MonsterName[Monster] == "Serpent")
	{
		//Description of the Serpent
		cout << MonsterName[Monster] << endl;
		cout << "A slithering snake that has a huge appetite.\n";
		cout << "Don't suppose you can talk it over..";
	}
	else if (MonsterName[Monster] == "Gargoyle")
	{
		//Description of the Gargoyle
		cout << MonsterName[Monster] << endl;
		cout << "A stone creature with great might and wings.\n";
		cout << "Watch out he might be stoned!";
	}
	Sleep(SleepTimer);
}

void DragonFight()
{
	//Function for interactionting with the dragon
	char Choice = ' ';
	int DragonHealth = 10;

	//Start dragon fight
	Monster = 4;
	system("cls");
	cout << "*You eneter the dragons caves.*\n";
	cout << "Your " << Weapons[ActiveWeapon] << " is at the ready.\n\n";

	while (Choice != '1')
	{
		InCombat = true;
		cout << MonsterName[Monster] << " | Health: " << DragonHealth << endl;

		//Fight options
		cout << "Now fight!\n";
		cout << "Press 1 to use " << Weapons[ActiveWeapon] << endl;
		cout << "Press 2 for info on the Dragon\n";
		cin >> Choice;

		//Fight menu
		switch (Choice)
		{
		case '1':
			//Attack Dragon
			cout << "You attacked the " << MonsterName[Monster] << "!\n";
			DiceRoll();
			randomRoll += AttackPower;
			DragonHealth -= randomRoll;
			break;
		case '2':
			//Dragon description
			system("cls");
			cout << "Mythical creature that will\n";
			cout << "kill you if you don't kill it!\n";
			Sleep(SleepTimer);
			break;
		default:
			cout << "Sorry your input was invalid. Please try again!\n";
		}
	}

	//Check to end while loop
	if (DragonHealth <= 0)
	{
		cout << "You beat the dragon!\n";
		Sleep(SleepTimer);
		system("cls");
		InCombat = false;
		return;
	}
	else
	{
		cout << "Alas, the dragon's eyes stare at you and places you under his spell.\n";
		cout << "You try to move but fail to do so and find yourself torched by the dragon’s fire.\n";
		cout << "If only you had more experience, you could have seen it coming.\n";
		cout << "You loose!";
		Sleep(3000);
	}
	system("cls");
}

void GodMode()
{
	int totalDamange = 0;

	//Ultimate power
	XP = 10000;
	Map[PlayerPos] = Map[PlayerPos] - 1;
	PlayerPos = 27;
	Map[27] = 15;
	ActiveWeapon = 5;
	system("CLS");
	HUD_UI();
	cout << "*You eneter the dragons caves.*\n";
	cout << "*An ora of energy surrounds you.*\n";
	cout << "*You see the scaly creature and a devastating beam of energy shoots from your eyes.*\n";

	//God mode energy damage sequence
	for (int i = 0; i < 5; i++)
	{
		MonsterHealth = rand() % 7 + 3;
		randomRoll = rand() % 6 + 1;
		MonsterHealth *= randomRoll;
		for (int j = 0; j < 2; j++)
		{
			MonsterHealth *= MonsterHealth;
			if (MonsterHealth < 0)
				MonsterHealth *= -1;
		}
		totalDamange += MonsterHealth;
		cout << "You deal " << MonsterHealth << " damage!\n";
		Sleep(SleepTimer);
	}
	cout << "You delt a total of " << totalDamange << " damage!\n";
	Sleep(SleepTimer);
	system("cls");
	cout << "The chalice of knowledge is your's!\n";
	cout << "You have acheived everlasting wisdom!\n";
	cout << "Now stop being a wimp and play the game legit you bum.\n";
	Sleep(SleepTimer);
}
