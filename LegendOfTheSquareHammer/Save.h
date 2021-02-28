/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "Building.h"
#include "Npc.h"

namespace LOTSH
{
	typedef std::map<std::string, Renders::Objects::Building> BuildingsMap;
	typedef std::map<std::string, Renders::Objects::Npc> NpcMap;
	typedef std::map<std::string, Renders::Objects::PickUp> PickUpMap;

	class Save
	{
		std::unique_ptr<Renders::Objects::Player> player;
		// Map of all buildings in all locations
		std::map<std::string, BuildingsMap> buildings;
		// Map of all enemies in all locations
		std::map<std::string, NpcMap> monsters;
		// Map of all items in all locations
		std::map<std::string, PickUpMap> items;

	public:
		Save(std::unique_ptr<Renders::Objects::Player> newPlayer);
		Save(const std::string& path, std::string& place);
		~Save() = default;

		constexpr const std::string& Name() const { return player->Name(); }
		inline Renders::Objects::Player& Player() { return *player; }

		BuildingsMap* Buildings(const std::string& location);
		NpcMap* Monsters(const std::string& location);
		PickUpMap* Items(const std::string& location);
		bool SaveGame(const std::string& place) const;
	};
}