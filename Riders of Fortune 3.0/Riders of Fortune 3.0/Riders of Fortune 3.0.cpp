#include "stdafx.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "RReaper.h"
#include "BruteTroll.h"
#include "DemonKnight.h"
#include <Windows.h>
#include <MMSystem.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <ctime>

// Weapons structure
struct WeaponsCollection
{
	string Name;
	short int AttackModifier = 0;
};

// Globals
// Player
PlayerCharacter Player;
stack<WeaponsCollection> BlackSmith;
WeaponsCollection Weapons[6];
vector<WeaponsCollection> WeaponsCollected;
const short int RC_SQRE_VALUE = 8;
const short int MapIndex = RC_SQRE_VALUE - 1;
const short MAX_HEALTH = 100;
const short MIN_EXP = 20;
short int Map[RC_SQRE_VALUE][RC_SQRE_VALUE];
short int invSize = 5;
short int *Inventory = nullptr;
short int DieRoll;
short int ActiveWeapon = 0;
short int InvCounter = 0;
bool InCombat = false;
bool PlayerMoved = true;
bool MoveX_Axis = true;
bool MovePositive = true;

// Prototypes
void MainMenu();
void MapGenerator();
void GenerateWeapons();
void DisplayMap();
void DiceRoll();
void MonsterRoll();
void GameplayUI();
void GameBoard();
void CheckWeapons();
void SaveGameFile();
void LoadSaveFile();
void PlayerMovement();
void FindCorners(short, short);
void PlayerMapMove(short &, short &);
void ExploreEnv(short, short);
void ChangeAlignment(bool &);
void RandomMonster();
short UseItems(short);
void RemoveItem(short);
void SwitchWeapons();
void EnvInteractSystem();
void AddWeapons();
void CheckInventory();
void addInv(short);
void IncreaseInv();
void TransferInventory();
void MysticBlackSmith();
void BlackSmithUpgrades();
void LeaveDespair();
void DebugTesting();
void HelpMenu();
void PressEnter();

using namespace std;

int main()
{
	system("Title Riders of Fortune 3");
	srand((unsigned int)time(NULL)); // Seed srand()

	Player.SetPlayerHealth(MAX_HEALTH);

	// Set the inventory size
	Inventory = new short[invSize];

	MapGenerator();
	GenerateWeapons();
	MainMenu();

	// End game
	return 0;
}

// Loads the main menu
void MainMenu()
{
	// Local variables
	unsigned int Option;

	// Menu
	system("cls");
	cout << "Welcome to Riders of Fortune 3 Choose to:\n\n";
	cout << "1) Start a New Game\n";
	cout << "2) Load a Saved Game\n";
	cout << "3) Help\n";
	cout << endl;
	cin >> Option;

	// Run menu configuration
	system("cls");
	switch (Option)
	{
	case 1:
		GameBoard();
		break;
	case 2:
		LoadSaveFile();
		GameBoard();
		break;
	case 3:
		HelpMenu();
		PressEnter();
		MainMenu();
	default:
		MainMenu();
	}
}

// Generates the map data
void MapGenerator()
{
	// Generates the map
	short randspace1 = 0, randspace2 = 0;
	short MapFixturePoint;

	// Misc.
	MapFixturePoint = (RC_SQRE_VALUE / 2);

	// Build map coordinate data
	for (int row = 0; row < RC_SQRE_VALUE; row++)
	{
		for (int column = 0; column < RC_SQRE_VALUE; column++)
		{
			if ((row == 0 || row == MapIndex) || (column == 0 || column == MapIndex))
				Map[column][row] = 0;
			else
				Map[column][row] = 21;
		}
	}

	// Set fixture spaces
	Map[0][0] = 2;							//Start top left
	Map[MapFixturePoint][0] = 4;			//Well top center
	Map[MapFixturePoint][MapIndex] = 4;		//Well bottom center
	Map[MapIndex][0] = 6;					//Cathedral top right
	Map[MapIndex][MapIndex] = 6;			//Cathedral bottom right
	Map[MapIndex][MapFixturePoint - 1] = 8; //Graveyard middle right
	Map[0][MapIndex] = 8;					//Graveyard Bottom left
	Map[0][MapFixturePoint - 1] = 10;		//Exit middle Left

	// Set starting player position
	Map[0][0] += 1;
}

// Generates the weapons
void GenerateWeapons()
{
	Weapons[0].Name = "Crossbow";
	Weapons[1].Name = "Flail";
	Weapons[2].Name = "Whip";
	Weapons[3].Name = "Mace";
	Weapons[4].Name = "Staff";
	Weapons[5].Name = "BroadSword";
	for (short int i = 0; i < 6; i++)
		Weapons[i].AttackModifier = rand() % 20 + 1;
}

// Displays the map
void DisplayMap()
{
	// Map rendering code
	for (int row = 0; row < RC_SQRE_VALUE; row++)
	{
		for (int column = 0; column < RC_SQRE_VALUE; column++)
		{
			// Shows the spaces and the player
			if ((Map[column][row] % 2) == 0)
			{
				// Displays the fixture spaces
				if (Map[column][row] == 0)
					cout << "* ";
				if (Map[column][row] == 2)
					cout << "S ";
				if (Map[column][row] == 4)
					cout << "W ";
				if (Map[column][row] == 6)
					cout << "C ";
				if (Map[column][row] == 8)
					cout << "G ";
				if (Map[column][row] == 10)
					cout << "E ";
			}
			else
			{
				if (Map[column][row] != 21)
					cout << "P ";
				else
					cout << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

// Handles the die roll
void DiceRoll()
{
	// Rolls a D6
	DieRoll = rand() % 6 + 1;
	short AttackStrength;

	// Identify if the player is in combat
	if (InCombat == false)
		cout << "You rolled a " << DieRoll << "!\n";
	else
	{
		cout << "You rolled a " << DieRoll << "!\n";
		Sleep(500);
		if (!WeaponsCollected.empty())
			AttackStrength = WeaponsCollected[ActiveWeapon].AttackModifier;
		else
			AttackStrength = 0;
		cout << "Added damage: " << AttackStrength << endl;
		Sleep(500);
		cout << "Total damage: " << DieRoll + AttackStrength << endl;
	}
	BlackSmithUpgrades();
	Sleep(2000);
}

// Handles the monsters attack
void MonsterRoll()
{
	DieRoll = rand() % 6 + 1;

	cout << "The monster attacks!\n";
	cout << "Deals " << DieRoll << " damage!\n";
	Sleep(2000);
}

// Creates a UI for the Player to see
void GameplayUI()
{
	// Health, experience, items, etc.
	string AlignmentStatus, CurrentWeapon;
	unsigned short int Health, Exp;

	Health = Player.GetPlayerHealth();
	Exp = Player.GetPlayerXP();

	// Map
	DisplayMap();

	/*
	DebugTesting();
	cout << "Player X: " << Player.GetPlayerX() << endl;
	cout << "Player Y: " << Player.GetPlayerY() << endl;
	MoveX_Axis ? cout << "Player MoveX _axis: true" << endl : cout << "Player MoveX_axis: false" << endl;
	MovePositive ? cout << "Player MoveX _axis: true" << endl : cout << "Player MoveX_axis: false" << endl;
	*/
	Player.GetPlayerAlignment() ? AlignmentStatus = "Good" : AlignmentStatus = "Evil";
	WeaponsCollected.empty() ? CurrentWeapon = "fists" : CurrentWeapon = WeaponsCollected[ActiveWeapon].Name;

	// Menu options
	cout << "Health: " << Health << " | XP: " << Exp
		 << " | Alignment: " << AlignmentStatus << " | Active Weapon: " << CurrentWeapon << endl;
	cout << "1) Go forward 2) Explore 3) Show Inventory 4) Save game 5) Quit";
	cout << endl;
	Player.SetActiveWeapon(CurrentWeapon);
}

// Loads the game board
void GameBoard()
{
	// Local variables
	unsigned int Option = 0;
	short PlayerX = Player.GetPlayerX();
	short PlayerY = Player.GetPlayerY();

	if (Player.GetPlayerHealth() > 0)
	{
		// Loads the HUD
		GameplayUI();

		cin >> Option;
		// Run menu configuration
		if (Option == 1 || Option == 2 || Option == 3 || Option == 4 || Option == 5)
		{
			switch (Option)
			{
			case 1:
				// Move player
				DiceRoll();
				PlayerMoved = true;
				system("cls");
				PlayerMovement();
				GameBoard();
			case 2:
				// Explore area
				if (PlayerMoved)
				{
					PlayerMoved = false;
					ExploreEnv(PlayerX, PlayerY);
					system("cls");
					GameBoard();
				}
				else
				{
					system("cls");
					cout << "You have not moved!\n";
					PressEnter();
					GameBoard();
				}
			case 3:
				// Check inventory
				CheckWeapons();
				CheckInventory();
				// try
				//{
				//	CheckWeapons();
				//	CheckInventory();
				// }
				// catch (const std::exception&)
				//{
				//	cout << "Your inventory is empty AF!\n";
				// }
				PressEnter();
				GameBoard();
			case 4:
				// Save the damn game!
				SaveGameFile();
				system("cls");
				GameBoard();
			case 5:
				// Quit and run home to mommy
				exit(0);
			default:
				break;
			}
		}
	}
	else
	{
		// You filthy scrub
		cout << "You Died!\n";
		PressEnter();
		exit(0);
	}
}

// Checks the weapons inventory
void CheckWeapons()
{
	cout << "Weapons: \n";
	if (!WeaponsCollected.empty())
	{
		for (short i = 0; i < WeaponsCollected.size(); i++)
		{
			cout << i + 1 << ". " << WeaponsCollected[i].Name;
			cout << " | Damage: " << WeaponsCollected[i].AttackModifier << endl;
		}
	}
	else
		cout << "[Empty sack]\n";
}

// Initiate how many spaces the player should move
void PlayerMovement()
{
	// Player Position, spaces remaining, and current axis
	short int PlayerX = Player.GetPlayerX();
	short int PlayerY = Player.GetPlayerY();

	for (short Spaces = DieRoll; Spaces > 0; Spaces--)
	{
		// Move Player
		FindCorners(PlayerX, PlayerY);
		PlayerMapMove(PlayerX, PlayerY);

		// Save player position
		Player.SetPlayerX(PlayerX);
		Player.SetPlayerY(PlayerY);
	}
}

// Finds the corners of the map
void FindCorners(short PlayerX, short PlayerY)
{
	// Corner detection
	if ((PlayerX == 0) && (PlayerY == 0))
	{
		// Right
		MoveX_Axis = true;
		MovePositive = true;
	}
	if ((PlayerX == 0) && (PlayerY == MapIndex))
	{
		// Up
		MoveX_Axis = false;
		MovePositive = false;
	}
	if ((PlayerX == MapIndex) && (PlayerY == MapIndex))
	{
		// Left
		MoveX_Axis = true;
		MovePositive = false;
	}
	if ((PlayerX == MapIndex) && (PlayerY == 0))
	{
		// Down
		MoveX_Axis = false;
		MovePositive = true;
	}
}

// Moves the player
void PlayerMapMove(short &PlayerX, short &PlayerY)
{
	Map[PlayerX][PlayerY]--;
	if (MoveX_Axis)
		MovePositive ? PlayerX++ : PlayerX--;
	else
		MovePositive ? PlayerY++ : PlayerY--;
	Map[PlayerX][PlayerY]++;
	DisplayMap();
	Sleep(200);
	system("cls");
}

// Events for the environment
void ExploreEnv(short PlayerX, short PlayerY)
{
	// Explore the current environment
	unsigned short Exp = Player.GetPlayerXP();
	short RandItem = rand() % 3 + 1;
	bool PlayerAlignment = Player.GetPlayerAlignment();
	bool ItemSwitched = true;

	system("cls");
	if (Map[PlayerX][PlayerY] == 1)
	{
		short RandNum = rand() % 4 + 1;
		if (RandNum == 1)
		{
			// Player found weapon
			AddWeapons();
			GameBoard();
		}
		else if (RandNum == 2)
		{
			// Player found an item
			addInv(RandItem);
			PressEnter();
			GameBoard();
		}
		else if (RandNum == 3)
		{
			// Empty spot
			cout << "There is nothing for you to do, so you reflect upon your adventures thus far.\n";
			cout << "You take the time to train and enhance your reflexes.\n";
			Exp += 2;
			Player.SetPlayerXP(Exp);
			PressEnter();
		}
		else if (RandNum == 4)
			LeaveDespair();
	}
	else if (Map[PlayerX][PlayerY] == 5)
	{
		ChangeAlignment(PlayerAlignment);
		Exp += 2;
		Player.SetPlayerXP(Exp);
	}
	else if (Map[PlayerX][PlayerY] == 7 || Map[PlayerX][PlayerY] == 9)
		EnvInteractSystem();
	else if (Map[PlayerX][PlayerY] == 11)
	{
		// Check XP
		if (Exp > 19)
		{
			for (short i = 0; i < invSize; i++)
			{
				// Check for the Relic
				if (Inventory[i] == 3)
				{
					// Beat the game
					system("cls");
					cout << "You beat the game!\n";
					cout << "You can go home and rest easy.\n";
					PressEnter();
					exit(0);
				}
			}
		}
		else
			cout << "Not experienced enough to leave!\n";
	}

	short RandomPopUp = rand() % 3 + 1;
	if (RandomPopUp == 1)
		MysticBlackSmith();
	else if (RandomPopUp == 2)
	{
		// Player found a monster
		RandomMonster();
		GameBoard();
	}
}

// Changes the player's alignment
void ChangeAlignment(bool &PlayerAlignment)
{
	if (PlayerAlignment)
	{
		// Change to evil
		cout << "Something is coming over you...\n You feel.. sick..\n";
		cout << "You're EVIL now!\n";
		PlayerAlignment = false;
	}
	else
	{
		// Change to good
		cout << "You're coming to your senses..\n";
		cout << "You're GOOD again!\n";
		PlayerAlignment = true;
	}
	Player.SetPlayerAlignment(PlayerAlignment);
	// PlaySound(TEXT("CharacterChange.wav"), NULL, SND_SYNC);
	PressEnter();
}

// Manipulates how the player's health changes
// when interacting with special environment
void EnvInteractSystem()
{
	// Local variables
	short Health = Player.GetPlayerHealth();
	short PlayerX = Player.GetPlayerX();
	short PlayerY = Player.GetPlayerY();
	short HealthManip = MAX_HEALTH * (short)0.2;
	string HealthStatus;
	bool Alignment = Player.GetPlayerAlignment();

	if (Map[PlayerX][PlayerY] == 7)
	{
		// Cathedral interaction
		if (Alignment)
		{
			if (Health < MAX_HEALTH)
				(Health + HealthManip) > MAX_HEALTH ? Health = MAX_HEALTH : Health += HealthManip;
			HealthStatus = "Health up!";
		}
		else
		{
			if (Health > 0)
				(Health - HealthManip) < 0 ? Health = 0 : Health -= HealthManip;
			HealthStatus = "Health down!";
		}
	}
	else if (Map[PlayerX][PlayerY] == 9)
	{
		// Graveyard interaction
		if (!Alignment)
		{
			if (Health < MAX_HEALTH)
				(Health + HealthManip) > MAX_HEALTH ? Health = MAX_HEALTH : Health += HealthManip;
			HealthStatus = "Health up!";
		}
		else
		{
			if (Health > 0)
				(Health - HealthManip) < 0 ? Health = 0 : Health -= HealthManip;
			HealthStatus = "Health down!";
		}
	}

	cout << HealthStatus;
	Player.SetPlayerHealth(Health);

}

// Monster Generator
void RandomMonster()
{
	BruteTroll Troll;
	RReaper Reaper;
	DemonKnight Demon;
	string CurrentWeapon, MonsterName;
	short unsigned Option;
	short MonsterHealth = 0;
	short ExpUp = MIN_EXP * (short)0.3;
	short PlayerHealth = Player.GetPlayerHealth();
	short PlayerExp = Player.GetPlayerXP();
	short RandomMonsterGen = rand() % 3 + 1;
	short AddedDamage, MonsterDamage;

	// Player/Monster experience detection
	Troll.SetMonsterHP(PlayerExp);
	Reaper.SetMonsterHP(PlayerExp);
	Demon.SetMonsterHP(PlayerExp);

	if (RandomMonsterGen == 1)
	{
		MonsterName = "Brute troll";
		MonsterHealth = Troll.GetMonsterHP();
	}
	else if (RandomMonsterGen == 2)
	{
		MonsterName = "Raging Reaper";
		MonsterHealth = Reaper.GetMonsterHP();
	}
	else if (RandomMonsterGen == 3)
	{
		MonsterName = "Winged Demon Knight";
		MonsterHealth = Demon.GetMonsterHP();
	}

	// Monster fight
	while (MonsterHealth > 0)
	{
		// Check Weapons inventory
		if (WeaponsCollected.empty())
		{
			// No weapons available
			CurrentWeapon = "fists";
			AddedDamage = 0;
		}
		else
		{
			// Weapon(s) available
			CurrentWeapon = WeaponsCollected[ActiveWeapon].Name;
			AddedDamage = WeaponsCollected[ActiveWeapon].AttackModifier;
		}

		// Actual fight
		cout << "[" << MonsterName << "]\n";
		cout << "##########################\n";
		cout << "Health: " << MonsterHealth;
		for (short i = 0; i < 3; i++)
			cout << endl;
		cout << "Player Health: " << PlayerHealth << endl;
		cout << "##########################\n";
		cout << "Active weapon: " << CurrentWeapon << endl;
		cout << "1) Attack 2)Run away 3) Use item 4) Change weapon\n";
		cin >> Option;

		switch (Option)
		{
		case 1:
			InCombat = true;
			DiceRoll();
			MonsterHealth -= (DieRoll + AddedDamage);
			if (MonsterHealth > 0)
			{
				// Loads the respetive monster's damage stats
				if (RandomMonsterGen == 1)
					MonsterDamage = Troll.attack(Player);
				else if (RandomMonsterGen == 2)
					MonsterDamage = Reaper.attack(Player);
				else if (RandomMonsterGen == 3)
					MonsterDamage = Demon.attack(Player, MAX_HEALTH, MapIndex);
				PlayerHealth -= MonsterDamage;
			}
			system("cls");
			break;
		case 2:
			InCombat = false;
			system("cls");
			cout << "You run away to save your own skin!\n";
			cout << "but.. you lost something..\n";
			RemoveItem(rand() % 3 + 1);
			PressEnter();
			return;
			break;
		case 3:
			CheckInventory();
			PlayerHealth = UseItems(PlayerHealth);
			break;
		case 4:
			CheckWeapons();
			SwitchWeapons();
			break;
		default:
			break;
		}

		// If player health reaches 0
		if (Player.GetPlayerHealth() <= 0)
		{
			// Scrub
			cout << "You Died!\n";
			PressEnter();
			exit(0);
		}
	}
	InCombat = false;

	// Update Player experience
	PlayerExp += ExpUp;
	Player.SetPlayerHealth(PlayerHealth);
	Player.SetPlayerXP(PlayerExp);
}

// Add items to inventory inventory
void addInv(short Item)
{
	if (Item == 1)
	{
		// Minor potion
		cout << "You found a Minor-Potion!\n";
		Inventory[InvCounter] = 1;
	}
	else if (Item == 2)
	{
		cout << "You found a Major-Potion!\n";
		// Major potion
		Inventory[InvCounter] = 2;
	}
	else if (Item == 3)
	{
		// Relic
		cout << "You found a Relic!\n";
		Inventory[InvCounter] = 3;
	}
	InvCounter++;
	IncreaseInv();
}

// Increase the inventory size
void IncreaseInv()
{
	// Increase size if needed
	if (InvCounter >= invSize)
		invSize = 10;

	TransferInventory();
}

// Checks the player's inventory
void CheckInventory()
{
	string Name;

	cout << "Items: \n";
	for (short i = 0; i < invSize; i++)
	{
		if (Inventory[i] == 1)
			Name = "Minor-Potion"; // Minor
		else if (Inventory[i] == 2)
			Name = "Major-Potion"; // Major
		else if (Inventory[i] == 3)
			Name = "Relic"; // Relic
		else
			Name = "Empty slot"; // empty
		cout << i + 1 << ". " << Name << endl;
	}
}

// Temporary transfer
void TransferInventory()
{
	// Throw away vector variable
	vector<short> tempInv;

	// Add stuff
	for (short i = 0; i < invSize; i++)
		tempInv.push_back(Inventory[i]);

	// delete stuff
	Inventory = new short int[invSize];
	for (short i = 0; i < invSize; i++)
		Inventory[i] = tempInv[i];
}

// Select an item
short UseItems(short Health)
{
	short Selection;

	cout << "Select an item (0 to skip)\n";
	cin >> Selection;

	if (Inventory[Selection - 1] == 1)
		Health += 5;
	else if (Inventory[Selection - 1] == 2)
		Health += 10;
	else if (Inventory[Selection - 1] == 3)
	{
		cout << "You can't use this in battle\n";
		system("cls");
		UseItems(Health);
	}
	RemoveItem(Selection - 1);
	system("cls");
	return Health;
}

// Add weapons to inventory
void AddWeapons()
{
	// RNG weapon
	short int Weapon;

	srand((unsigned int)time(NULL)); // reseed srand

	// Randomly select a weapon
	Weapon = rand() % 6;

	// Check if the player has the same weapons or no weapons at all
	if (WeaponsCollected.size() < 5)
	{
		if (WeaponsCollected.empty())
		{
			// Display the weapon the player aquired
			cout << "You got the " << Weapons[Weapon].Name << "!\n";
			WeaponsCollected.push_back(Weapons[Weapon]);
		}
		else
		{
			for (short i = 0; i < WeaponsCollected.size(); i++)
			{
				// Randomly select another weapon if the player already has one of the same
				if (Weapons[Weapon].Name == WeaponsCollected[i].Name)
				{
					Weapon = rand() % 6;
					i = -1;
				}
			}
			// Display the weapon the player aquired
			cout << "You got the " << Weapons[Weapon].Name << "!\n";
			WeaponsCollected.push_back(Weapons[Weapon]);
		}
	}
	else
		cout << "Your Weapons inventory is full!\n";
	PressEnter();
}

// Select a weapon
void SwitchWeapons()
{
	short Selection;

	cout << "Select a weapon (0 to skip)\n";
	cin >> Selection;

	switch (Selection)
	{
	case 1:
		ActiveWeapon = 0;
		break;
	case 2:
		ActiveWeapon = 1;
		break;
	case 3:
		ActiveWeapon = 2;
		break;
	case 4:
		ActiveWeapon = 3;
		break;
	case 5:
		ActiveWeapon = 4;
		break;
	default:
		break;
	}
	system("cls");
}

// Remove an item from the player's inventory
void RemoveItem(short item)
{
	for (short i = item; i < InvCounter; i++)
		Inventory[i] = Inventory[i + 1];

	Inventory[InvCounter] = 0;
	InvCounter--;
	if (InvCounter < 0)
		InvCounter = 0;
}

// BlackSmith for upgrading weapons
void MysticBlackSmith()
{
	char Restart, Option;
	short unsigned Selection, Weapon = 0;
	cout << "You found a black smith!\n";
	cout << "What do you want to do?\n";
	cout << "(L)eave weapon - (R)etreive a weapon - (C)ontinue on\n";
	cin >> Option;

	// User choice operations
	if (Option == 'L' || Option == 'l')
	{
		if (!WeaponsCollected.empty())
		{
			do
			{
				cout << "Pick a weapon you wish to upgrade: \n";
				cout << "Press 0 to move along.\n";
				CheckWeapons();
				cin >> Selection;

				// Gets the weapon from the player
				switch (Selection)
				{
				case 1:
					Weapon = 0;
					break;
				case 2:
					Weapon = 1;
					break;
				case 3:
					Weapon = 2;
					break;
				case 4:
					Weapon = 3;
					break;
				case 5:
					Weapon = 4;
					break;
				default:
					break;
				}

				// Check if player leaves a weapon with the black smith or walks away
				if (Selection != 0)
				{
					BlackSmith.push(WeaponsCollected[Weapon]);
					WeaponsCollected.erase(WeaponsCollected.begin() + Weapon);
				}
				system("cls");
				cout << "Would you like to add another? (Y/N)\n";
				cin >> Restart;

			} while (Restart == 'Y' || Restart == 'y');
		}
		else
		{
			cout << "You have no weapons!\n";
			cout << "Why did you come?\n";
			PressEnter();
		}
	}
	else if (Option == 'R' || Option == 'r')
	{
		do
		{
			if ((WeaponsCollected.size() < 5 || WeaponsCollected.empty()) && !BlackSmith.empty())
			{
				// Give the player back a weapon
				WeaponsCollected.push_back(BlackSmith.top());
				BlackSmith.pop();
			}
			else if (WeaponsCollected.size() == 5 && !BlackSmith.empty())
			{
				// Throw away a weapon
				cout << "Your Weapons sack is full!\n ";
				cout << "Which weapon would you like to through out?";
				CheckWeapons();
				cin >> Selection;

				switch (Selection)
				{
				case 1:
					Weapon = 0;
					break;
				case 2:
					Weapon = 1;
					break;
				case 3:
					Weapon = 2;
					break;
				case 4:
					Weapon = 3;
					break;
				case 5:
					Weapon = 4;
					break;
				default:
					break;
				}

				// Give the player back a weapon
				WeaponsCollected.erase(WeaponsCollected.begin() + Weapon);
				WeaponsCollected.push_back(BlackSmith.top());
				BlackSmith.pop();
			}
			cout << "Would you like to grab another weapon from the black smith?(Y/N)\n";
			cin >> Restart;
		} while (Restart == 'Y' || Restart == 'y');
	}
}

// Black smith upgrades your weapon(s) while it's in the kilm
void BlackSmithUpgrades()
{
	// Throw away vector for upgrading the player's weapon(s)
	vector<WeaponsCollection> UpgradeStation;

	// Check if the black smith has a weapon
	if (!BlackSmith.empty())
	{
		// Move to work station
		for (short i = 0; i < BlackSmith.size(); i++)
		{
			UpgradeStation.push_back(BlackSmith.top());
			BlackSmith.pop();
		}

		// Upgrade at workstation and set to cool
		for (short i = 0; i < UpgradeStation.size(); i++)
		{
			UpgradeStation[i].AttackModifier += DieRoll;
			BlackSmith.push(UpgradeStation[i]);
			UpgradeStation.pop_back();
		}
	}
}

// Path of perpectual torment
void LeaveDespair()
{
	cout << "You have fallen into despair!\n";
	cout << "May you hope to get out!\n";
	DiceRoll();
	system("cls");
	if (DieRoll == 3 || DieRoll == 6)
		GameBoard();
	else
		LeaveDespair();
}

// Save the game
void SaveGameFile()
{
	vector<WeaponsCollection> UpgradeStation;
	string FileName = "SavedGame.dat";
	ofstream SaveGame;
	short Health, Exp, PlayerX, PlayerY;
	bool Alignment;

	// Get player values
	Health = Player.GetPlayerHealth();
	Alignment = Player.GetPlayerAlignment();
	Exp = Player.GetPlayerXP();
	PlayerX = Player.GetPlayerX();
	PlayerY = Player.GetPlayerY();

	// File to save the game
	SaveGame.open(FileName, ios::out | ios::binary);

	// Save the map/player location
	SaveGame << Health << endl;
	SaveGame << Alignment << endl;
	SaveGame << Exp << endl;
	SaveGame << PlayerX << endl;
	SaveGame << PlayerY << endl;
	SaveGame << PlayerMoved << endl;
	SaveGame << MoveX_Axis << endl;
	SaveGame << MovePositive << endl;
	SaveGame << WeaponsCollected.size() << endl;

	// Save weapons
	for (short int i = 0; i < WeaponsCollected.size(); i++)
	{
		SaveGame << WeaponsCollected[i].Name << endl;
		SaveGame << WeaponsCollected[i].AttackModifier << endl;
	}
	SaveGame << ActiveWeapon << endl;

	// Save items
	SaveGame << invSize << endl;
	for (short int i = 0; i < invSize; i++)
		SaveGame << Inventory[i] << endl;

	SaveGame << InvCounter << endl;

	// Move to work station
	for (short i = 0; i < BlackSmith.size(); i++)
	{
		UpgradeStation.push_back(BlackSmith.top());
		BlackSmith.pop();
	}

	// Save the weapons given to the blacksmith
	SaveGame << UpgradeStation.size() << endl;
	for (short i = 0; i < UpgradeStation.size(); i++)
	{
		SaveGame << UpgradeStation[i].Name << endl;
		SaveGame << UpgradeStation[i].AttackModifier << endl;
	}

	// Close the file
	SaveGame.close();

	// Confirm saved game
	cout << "Saved!\n";
	PlaySound(TEXT("SavedGame.wav"), NULL, SND_SYNC);
}

// Loads the game
void LoadSaveFile()
{
	// Local variables
	vector<WeaponsCollection> UpgradeStation;
	string FileName = "SavedGame.dat";
	ifstream LoadGame;
	short Health, Exp, PlayerX, PlayerY, WeaponInvSize, StationSize;
	bool Alignment;

	LoadGame.open(FileName, ios::in | ios::binary);
	if (LoadGame.fail())
	{
		// load fail
		cout << "File does not exist!\n";
		Sleep(1500);
		MainMenu();
	}
	else
	{
		LoadGame >> Health;
		LoadGame >> Alignment;
		LoadGame >> Exp;
		LoadGame >> PlayerX;
		LoadGame >> PlayerY;
		LoadGame >> PlayerMoved;
		LoadGame >> MoveX_Axis;
		LoadGame >> MovePositive;
		LoadGame >> WeaponInvSize;

		// Load weapons
		WeaponsCollected.resize(WeaponInvSize);
		for (short int i = 0; i < WeaponInvSize; i++)
		{
			LoadGame >> WeaponsCollected[i].Name;
			LoadGame >> WeaponsCollected[i].AttackModifier;
		}
		LoadGame >> ActiveWeapon;

		// Load items
		LoadGame >> invSize;
		for (short int i = 0; i < invSize; i++)
			LoadGame >> Inventory[i];

		// Get inventory count
		LoadGame >> InvCounter;

		// Load the weapons from the station
		LoadGame >> StationSize;
		UpgradeStation.resize(StationSize);
		for (short i = 0; i < UpgradeStation.size(); i++)
		{
			LoadGame >> UpgradeStation[i].Name;
			LoadGame >> UpgradeStation[i].AttackModifier;
		}

		// Give the weapons back to the black smith like a good boy
		for (short i = 0; i < UpgradeStation.size(); i++)
		{
			BlackSmith.push(UpgradeStation[i]);
			UpgradeStation.pop_back();
		}

		Player.SetPlayerHealth(Health);
		Player.SetPlayerAlignment(Alignment);
		Player.SetPlayerXP(Exp);
		Player.SetPlayerX(PlayerX);
		Player.SetPlayerY(PlayerY);
		Map[0][0]--;
		Map[PlayerX][PlayerY]++;
	}
	LoadGame.close();
}

// Map debugging code
void DebugTesting()
{
	// Debugging code
	cout << "Map visualizer: " << endl;
	for (int row = 0; row < RC_SQRE_VALUE; row++)
	{
		cout << "Row: " << row + 1 << "	 |";
		for (int column = 0; column < RC_SQRE_VALUE; column++)
		{
			if ((row == 0 || row == 7) || (column == 0 || column == 7))
				cout << Map[column][row] << "	";
			else
				cout << Map[column][row] << "	";
		}
		cout << endl
			 << endl
			 << endl;
	}

	for (short i = 0; i < WeaponsCollected.size(); i++)
	{
		cout << WeaponsCollected[0].Name << endl;
	}
}

// Help menu
void HelpMenu()
{
	cout << "[HELP]\n\n";
	cout << "S = Starting posistion for the player.\n";
	cout << "W = The well of reflection, changes your characters view of ZA WARUDO!!\n";
	cout << "C = The Cathedral where prayers are healing and demons die!\n";
	cout << "G = The Graveyard where Angels can't leave unscaved and demons thrive!\n";
	cout << "* Monsters appear at random.\n";
	cout << "* Weapons appear at random.\n";
	cout << "* Items appear at random.\n";
	cout << "* Everything shows up at random! (Even your key to the exit!)\n";
	cout << "* Watch out for the lake of Despair! \n";
	cout << "  You'll be depressed for some time.\n";
	cout << "* Press 'Alt + Enter' for an immersive exprience\n";
	cout << endl;
}

// Function to continue on at the player's leisure
void PressEnter()
{
	cout << "Press enter to continue...";
	cin.ignore();
	cin.get();
	system("cls");
}
