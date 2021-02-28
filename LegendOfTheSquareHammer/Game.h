/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "Save.h"

namespace LOTSH
{
	class Game
	{
		bool isNewLevel = false;
		int movement = 5;
		short quit = 0;
		bool gameOn = false;
		Save gameSave;

		// Check collision of moving object with NPCs on map
		static short CheckCollision(const Renders::Objects::IObject& object, const NpcMap& monsters);
		// Check collision of moving object with buildings on map
		static CollisionEntrance CheckCollision(const Renders::Objects::IObject& object, BuildingsMap& buildings);
		static constexpr bool IsInsideScreen(const SDL_Rect& pos);

		// Deals damage to enemies in direction set by mouse. Signals new level
		void Attack(int x, int y, const std::string& place);
		// Render objects in order
		void RenderOrder(const std::string& place);
		void MoveObjects(const std::string& place, CollisionEntrance& collision, int fieldWidth, int fieldHeight, int* x = nullptr, int* y = nullptr);
		// Check if NPCs are dead and decrements time left in dead state
		void CheckDeath(NpcMap& monsters);
		// Check if player stepped on item and adding it to inventory
		void GetItems(PickUpMap& items);
		void EnterBuilding(const std::string& name);

		void PauseMenu(bool gameOver = false);
		void Castle();

	public:
		Game(std::unique_ptr<Renders::Objects::Player> newPlayer);
		Game(const std::string& path, std::string& startingPlace);
		~Game() = default;

		constexpr bool IsGame() noexcept { return gameOn; }

		// Main game screen
		void Play(const std::string& place = "main");
	};
}