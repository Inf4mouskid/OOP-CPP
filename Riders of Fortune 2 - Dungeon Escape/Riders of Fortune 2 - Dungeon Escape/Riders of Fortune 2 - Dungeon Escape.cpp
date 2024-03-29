#include "stdafx.h"
#include "PlayerCharacter.h"
#include <Windows.h>
#include <MMSystem.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// Weapons structure
struct WeaponsCollection
{
	string ID_Name;
	short int AttackModifier;
};

//Global variables
PlayerCharacter Player;
string FileName = "SavedGame.dat";
WeaponsCollection Weapons[6];
vector<WeaponsCollection> WeaponsCollected;
const int RC_SQRE_VALUE = 8;
const int MapIndex = RC_SQRE_VALUE - 1;
const short MAX_HEALTH = 100;
const short MIN_EXP = 20;
short int invSize = 5;
short int Map[RC_SQRE_VALUE][RC_SQRE_VALUE];
short int DieRoll;
short int ActiveWeapon;
short int *Inventory = nullptr;
short int InvCounter = 0;
bool InCombat = false;
bool PlayerMoved = true;
bool MoveX_Axis = true;
bool MovePositive = true;

//Prototypes
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
void RandomMonster(short);
short UseItems(short);
void RemoveItem(short);
void SwitchWeapons();
void EnvInteractSystem();
void AddWeapons();
void CheckInventory();
void addInv(short);
void IncreaseInv();
void TransferInventory();
void DebugTesting();
void PressEnter();

int main()
{
	unsigned int seed = time(0);

	system("Title Riders of Fortune 2: Dungeon Escape");
	srand(seed); // Seed srand()

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
	cout << "Welcome to Riders of Fortune 2: Dungeon Escape Choose to:\n\n";
	cout << "1) Start a New Game\n";
	cout << "2) Load a Saved Game\n";
	cin >> Option;

	// Run menu configuration
	if (Option == 1 || Option == 2)
	{
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
		default:
			break;
		}
	}
	else
		MainMenu();
}

// Generates the map data
void MapGenerator()
{
	//Generates the map
	short randspace1, randspace2;
	short MapFixturePoint;

	//Misc.
	MapFixturePoint = (RC_SQRE_VALUE / 2);

	//Build map coordinate data
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

	//Set fixture spaces
	Map[0][0] = 2; //Start top left
	Map[MapFixturePoint][0] = 4; //Well top center
	Map[MapFixturePoint][MapIndex] = 4; //Well bottom center
	Map[MapIndex][0] = 6; //Cathedral top right
	Map[MapIndex][MapIndex] = 6; //Cathedral bottom right
	Map[MapIndex][MapFixturePoint - 1] = 8; //Graveyard middle right
	Map[0][MapIndex] = 8; //Graveyard Bottom left
	Map[0][MapFixturePoint - 1] = 10; //Exit middle Left

									  //Set starting player position
	Map[0][0] += 1;
}

// Generates the weapons
void GenerateWeapons()
{
	Weapons[0].ID_Name = "Crossbow";
	Weapons[1].ID_Name = "Flail";
	Weapons[2].ID_Name = "Whip";
	Weapons[3].ID_Name = "Mace";
	Weapons[4].ID_Name = "Staff";
	Weapons[5].ID_Name = "Broad Sword";
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
			//Shows the spaces and the player
			if ((Map[column][row] % 2) == 0)
			{
				//Displays the fixture spaces
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

	//Identify if the player is in combat
	if (InCombat == false)
		cout << "You rolled a " << DieRoll << "!\n";
	else
	{
		cout << "You rolled a " << DieRoll << "!\n";
		Sleep(1000);
		if (!WeaponsCollected.empty())
			AttackStrength = WeaponsCollected[ActiveWeapon].AttackModifier;
		else
			AttackStrength = 0;
		cout << "Added damage: " << AttackStrength << endl;
	}
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

	/*cout << "Player X: " << Player.GetPlayerX() << endl;
	cout << "Player Y: " << Player.GetPlayerY() << endl;
	MoveX_Axis ? cout << "Player MoveX _axis: true" << endl : cout << "Player MoveX_axis: false" << endl;
	MovePositive ? cout << "Player MoveX _axis: true" << endl : cout << "Player MoveX_axis: false" << endl;*/

	Player.GetPlayerAlignment() ? AlignmentStatus = "Good" : AlignmentStatus = "Evil";
	WeaponsCollected.empty() ? CurrentWeapon = "fists" : CurrentWeapon = WeaponsCollected[ActiveWeapon].ID_Name;

	// Menu options
	cout << "Health: " << Health << " | XP: " << Exp
		<< " | Alignment: " << AlignmentStatus << " | Active Weapon: " << CurrentWeapon << endl;
	cout << "1) Go forward 2) Explore 3) Show Inventory 4) Save game 5) Quit";
	cout << endl;
}

// Loads the game board
void GameBoard()
{
	// Local variables
	unsigned int Option = 0;
	short PlayerX = Player.GetPlayerX();
	short PlayerY = Player.GetPlayerY();

	// Loads the HUD
	GameplayUI();

	cin >> Option;
	// Run menu configuration
	if (Option == 1 || Option == 2 || Option == 3 || Option == 4 || Option == 5)
	{
		switch (Option)
		{
		case 1:
			//Move player
			DiceRoll();
			PlayerMoved = true;
			system("cls");
			PlayerMovement();
			GameBoard();
		case 2:
			//Explore area
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
			//try
			//{
			//	CheckWeapons();
			//	CheckInventory();
			//}
			//catch (const std::exception&)
			//{
			//	cout << "Your inventory is empty AF!\n";
			//}
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

// Checks the weapons inventory
void CheckWeapons()
{
	cout << "Weapons: \n";
	if (!WeaponsCollected.empty())
	{
		for (short i = 0; i < WeaponsCollected.size(); i++)
		{
			cout << i + 1 << ". " << WeaponsCollected[i].ID_Name;
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
	if ((PlayerX == 0) && (PlayerY == 0)) {
		// Right
		MoveX_Axis = true;
		MovePositive = true;
	}
	if ((PlayerX == 0) && (PlayerY == MapIndex)) {
		// Up
		MoveX_Axis = false;
		MovePositive = false;
	}
	if ((PlayerX == MapIndex) && (PlayerY == MapIndex)) {
		// Left 
		MoveX_Axis = true;
		MovePositive = false;
	}
	if ((PlayerX == MapIndex) && (PlayerY == 0)) {
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
	short RandNum = rand() % 3 + 1;
	short RandItem = rand() % 3 + 1;
	short HealthLvl = 1;
	bool PlayerAlignment = Player.GetPlayerAlignment();
	bool ItemSwitched = true;

	if (Exp >= 10)
		HealthLvl = 2;
	else if (Exp >= 15)
		HealthLvl = 3;

	system("cls");
	if (Map[PlayerX][PlayerY] == 1)
	{
		if (RandNum == 1)
		{
			// Player found weapon
			AddWeapons();
			GameBoard();
		}
		else if (RandNum == 2)
		{
			// Player found a monster
			RandomMonster(HealthLvl);
			GameBoard();
		}
		else if (RandNum == 3)
		{
			// Player found an item
			cout << "You found an item!\n";
			addInv(RandItem);
			PressEnter();
			GameBoard();
		}
	}
	else if (Map[PlayerX][PlayerY] == 5)
	{
		ChangeAlignment(PlayerAlignment);
		Exp += 2;
		Player.SetPlayerXP(Exp);
		PlaySound(TEXT("CharacterChange.wav"), NULL, SND_SYNC);
	}
	else if (Map[PlayerX][PlayerY] == 7 || Map[PlayerX][PlayerY] == 9)
		EnvInteractSystem();
	else if (Map[PlayerX][PlayerY] == 11)
	{
		if (Exp > 19)
		{
			// Beat the game
			system("cls");
			cout << "You beat the game!\n";
			cout << "You can go home and rest easy.\n";
			PressEnter();
		}
		else
		{
			system("cls");
			cout << "You are not skilled enough to leave\n";
			GameBoard();
		}
	}
}

// Changes the player's alignment
void ChangeAlignment(bool &PlayerAlignment)
{
	PlayerAlignment ? PlayerAlignment = false : PlayerAlignment = true;
	Player.SetPlayerAlignment(PlayerAlignment);
}

// Manipulates how the player's health changes
// when interacting with the environment
void EnvInteractSystem()
{
	// Local variables
	short Health = Player.GetPlayerHealth();
	short PlayerX = Player.GetPlayerX();
	short PlayerY = Player.GetPlayerY();
	short HealthManip = MAX_HEALTH * 0.2;

	if (Map[PlayerX][PlayerY] == 7)
	{
		// Cathedral interaction
		if (Player.GetPlayerAlignment())
		{
			if (Health < MAX_HEALTH)
				(Health + HealthManip) > MAX_HEALTH ? Health = MAX_HEALTH : Health += HealthManip;
		}
		else
		{
			if (Health > 0)
				(Health - HealthManip) < 0 ? Health = 0 : Health -= HealthManip;
		}
	}
	else if (Map[PlayerX][PlayerY] == 9)
	{
		// Graveyard interaction
		if (Player.GetPlayerAlignment())
		{
			if (Health > 0)
				(Health - HealthManip) < 0 ? Health = 0 : Health -= HealthManip;
		}
		else
		{
			if (Health < MAX_HEALTH)
				(Health + HealthManip) > MAX_HEALTH ? Health = MAX_HEALTH : Health += HealthManip;
		}
	}

	Player.SetPlayerHealth(Health);
}

// Monster Generator
void RandomMonster(short HealthLvl)
{
	string CurrentWeapon;
	short MonsterHealth = 0;
	short ExpUp = MIN_EXP * 0.3;
	short PlayerHealth = Player.GetPlayerHealth();
	short PlayerExp = Player.GetPlayerXP();
	short Option;
	short AddedDamage;

	// Player/Monster experience detection
	if (HealthLvl == 1)
		MonsterHealth = rand() % 10 + 5;
	else if (HealthLvl == 2)
		MonsterHealth = rand() % 15 + 5;
	else if (HealthLvl == 3)
		MonsterHealth = rand() % 20 + 10;

	// Monster fight
	while (MonsterHealth > 0)
	{
		//Check Weapons inventory
		if (WeaponsCollected.empty())
		{
			CurrentWeapon = "fists";
			AddedDamage = 0;
		}
		else
		{
			CurrentWeapon = WeaponsCollected[ActiveWeapon].ID_Name;
			AddedDamage = WeaponsCollected[ActiveWeapon].AttackModifier;
		}

		//Actual fight
		cout << "A monster approaches!\n";
		cout << "Your " << CurrentWeapon << " is ready!\n";
		cout << "---------------------\n";
		cout << "Monster Health: " << MonsterHealth << endl;
		cout << endl;
		cout << "Player Health: " << PlayerHealth << endl;
		cout << "*----------------------------------*\n";
		cout << "1) Attack 2)Run away 3) Use item 4) Change weapon\n";
		cin >> Option;

		if (Option >= 1 || Option <= 4)
		{
			switch (Option)
			{
			case 1:
				InCombat = true;
				DiceRoll();
				MonsterHealth -= (DieRoll + AddedDamage);
				if (MonsterHealth > 0)
				{
					MonsterRoll();
					PlayerHealth -= DieRoll;
				}
				system("cls");
				break;
			case 2:
				InCombat = false;
				system("cls");
				cout << "You run away to save your own skin!\n";
				Sleep(1500);
				return;
				break;
			case 3:
				CheckInventory();
				PlayerHealth = UseItems(PlayerHealth);
				break;
			case 4:
				CheckWeapons();
				SwitchWeapons();
			default:
				break;
			}
		}
	}
	InCombat = false;

	//Update Player experience
	PlayerExp += ExpUp;
	Player.SetPlayerHealth(PlayerHealth);
	Player.SetPlayerXP(PlayerExp);
}

//Add weapons to the player inventory
void AddWeapons()
{
	//RNG weapon
	short int RandomSelection;
	short int Weapon;

	// Randomly select a weapon
	RandomSelection = rand() % 6 + 1;
	switch (RandomSelection)
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
	case 6:
		Weapon = 5;
		break;
	default:
		// My name is Jeff
		break;
	}

	// Check if the player has the same weapons
	for (short i = 0; i < WeaponsCollected.size(); i++)
	{
		if (WeaponsCollected[i].ID_Name == Weapons[Weapon].ID_Name)
			AddWeapons();
		else
		{
			// Display the weapon the player aquired
			cout << "You got the " << Weapons[Weapon].ID_Name << endl;
			WeaponsCollected.push_back(Weapons[Weapon]);
			PressEnter();
			return;
		}
	}
}

// Increase size of inventory
void addInv(short Item)
{
	if (Item == 1)
	{
		//Minor potion
		Inventory[InvCounter] = 1;
	}
	else if (Item == 2)
	{
		// Major potion
		Inventory[InvCounter] = 2;
	}
	else if (Item == 3)
	{
		// Relic
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
			Name = "Empty slot"; //empty
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

//Select an item
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

//Select a weapon
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

// Save the game
void SaveGameFile()
{
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
	SaveGame << MoveX_Axis << endl;
	SaveGame << MovePositive << endl;

	// Save Weapons
	SaveGame << WeaponsCollected.size() << endl;
	for (short int i = 0; i < WeaponsCollected.size(); i++)
	{
		SaveGame << WeaponsCollected[i].ID_Name << endl;
		SaveGame << WeaponsCollected[i].AttackModifier << endl;
	}
	SaveGame << ActiveWeapon << endl;

	// Save items
	SaveGame << invSize << endl;
	for (short int i = 0; i < invSize; i++)
		SaveGame << Inventory[i] << endl;

	SaveGame << InvCounter << endl;

	// Close the file
	SaveGame.close();

	// Confirm saved game
	cout << "Saved!\n";
}

// Loads the game
void LoadSaveFile()
{
	// Local variables
	ifstream LoadGame;
	short Health, Exp, PlayerX, PlayerY, WeaponInvSize;
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
		LoadGame >> MoveX_Axis;
		LoadGame >> MovePositive;
		LoadGame >> WeaponInvSize;

		//Load weapons
		WeaponsCollected.resize(WeaponInvSize);
		for (short int i = 0; i < WeaponInvSize; i++)
		{
			LoadGame >> WeaponsCollected[i].ID_Name;
			LoadGame >> WeaponsCollected[i].AttackModifier;
		}
		LoadGame >> ActiveWeapon;

		//Load items
		LoadGame >> invSize;
		for (short int i = 0; i < invSize; i++)
			LoadGame >> Inventory[i];

		//Get inventory count
		LoadGame >> InvCounter;

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
	//Debugging code
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
		cout << endl << endl << endl;
	}
}

// Function to continue on
void PressEnter()
{
	cout << "Press enter to continue...";
	cin.ignore();
	cin.get();
	system("cls");
}
