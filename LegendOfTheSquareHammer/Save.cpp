/*
Author: Marek Machliñski
Date:	11.02.2021
*/
#include "Save.h"
#include "StatsPotion.h"
#include <filesystem>

namespace LOTSH
{
	Save::Save(std::unique_ptr<Renders::Objects::Player> newPlayer) : player(std::move(newPlayer))
	{
		buildings.emplace("main", BuildingsMap());
		buildings.at("main").emplace("castle", Renders::Objects::Building(3, 0, 0, -1095, -1534, { 0,104,426,350 }, { 195,434,35,20 }, "Textures/Castle.png", "castle"));
		buildings.emplace("castle", BuildingsMap());
		buildings.at("castle").emplace("wallUp", Renders::Objects::Building(1, 0, 40, 0, 40, { 0,0,1920,115 }, { 0,0,0,0 }, "Textures/Blank.png", "wallUp"));
		buildings.at("castle").emplace("wallLeft", Renders::Objects::Building(1, 161, 0, 161, 0, { 0,0,100,1080 }, { 0,0,0,0 }, "Textures/Blank.png", "wallLeft"));
		buildings.at("castle").emplace("wallRight", Renders::Objects::Building(1, 1659, 0, 1659, 0, { 0,0,100,1080 }, { 0,0,0,0 }, "Textures/Blank.png", "wallRight"));
		buildings.at("castle").emplace("wallDownLeft", Renders::Objects::Building(1, 0, 639, 0, 839, { 0,0,862,201 }, { 0,0,0,0 }, "Textures/Blank.png", "wallDownLeft"));
		buildings.at("castle").emplace("wallDownRight", Renders::Objects::Building(1, 1059, 839, 1059, 839, { 0,0,862,201 }, { 0,0,0,0 }, "Textures/Blank.png", "wallDownRight"));
		buildings.at("castle").emplace("wallDown", Renders::Objects::Building(1, 0, 1039, 0, 1039, { 0,0,1920,41 }, { 760,0,400,20 }, "Textures/Blank.png", "wallDown"));
		monsters.emplace("main", NpcMap());
		monsters.at("main").emplace("roger", Renders::Objects::Npc(2.5, 2000, 3000, 905, 1466, 201, 30, 500,
			{ 0,56,44,12 }, { 15,0,0,0,0 }, { 5,0,0,0,0 }, "Textures/Roger.png",
			std::make_unique<Renders::Objects::PickUp>(0, 0,
				std::make_unique<Renders::Items::Armor>(BodyPart::Hands, "Best of gloves",
					"Best what you can get for that price", Stats({ 4,5,6,3,5,2,1,6,7,9 }),
					1, MainStats({ 0,0,0 }), "Textures/BestGloves.png", 0, 0))));
		monsters.emplace("castle", NpcMap());
		items.emplace("main", PickUpMap());
		items.at("main").emplace("Health potion", Renders::Objects::PickUp(2500, 4500,
			std::make_unique<Renders::Items::SingleUse::Potion>("Minor health potion",
				"Immediatelly regenerates life but cost your mana.", -5, 10, 1, "Textures/HPotion.png", 1405, 2966)));
		items.at("main").emplace("Mana potion", Renders::Objects::PickUp(2500, 500,
			std::make_unique<Renders::Items::SingleUse::Potion>("Minor mana potion",
				"Immediatelly regenerates mana but cost your life.", 10, -5, 1, "Textures/MPotion.png", 1405, -1034)));
		items.emplace("castle", PickUpMap());
		items.at("castle").emplace("Sword of Doom", Renders::Objects::PickUp(900, 500,
			std::make_unique<Renders::Items::Weapon>(WeaponType::Sword, "Sword of Doom",
				"Sword wield by the almighty God Emperor of Manikind. All Imperial citizens dreams of seeing this sword and you just slashing it from left to right. The Emperor protects!",
				Damage({ 15,9,0,0,0 }), 20, 10, 2, MainStats({ 0,16,0 }), Stats({ 2,6,1,5,7,1,1,1,1,1 }), "Textures/DoomSword.png", 900, 500)));
	}

	Save::Save(const std::string& path, std::string& place)
	{
		std::ifstream fin(path);
		if (!fin.good())
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "Cannot open save file: " << path << std::endl;
			fout.close();
			exit(0b10000000);
		}
		getline(fin, place);
		player = std::make_unique<Renders::Objects::Player>();
		fin >> *player;
		size_t count;
		fin >> count;
		for (size_t i = 0, J; i < count; ++i)
		{
			std::string placeName;
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, placeName);
			items.emplace(placeName, PickUpMap());
			fin >> J;
			for (size_t j = 0; j < J; ++j)
			{
				std::string objectName;
				if (fin.peek() == 10)
					fin.ignore();
				getline(fin, objectName);
				fin >> items.at(placeName).emplace(objectName, Renders::Objects::PickUp()).first->second;
			}
		}
		fin >> count;
		for (size_t i = 0, J; i < count; ++i)
		{
			std::string placeName;
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, placeName);
			buildings.emplace(placeName, BuildingsMap());
			fin >> J;
			for (size_t j = 0; j < J; ++j)
			{
				std::string objectName;
				if (fin.peek() == 10)
					fin.ignore();
				getline(fin, objectName);
				fin >> buildings.at(placeName).emplace(objectName, Renders::Objects::Building()).first->second;
			}
		}
		fin >> count;
		for (size_t i = 0, J; i < count; ++i)
		{
			std::string placeName;
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, placeName);
			monsters.emplace(placeName, NpcMap());
			fin >> J;
			for (size_t j = 0; j < J; ++j)
			{
				std::string objectName;
				if (fin.peek() == 10)
					fin.ignore();
				getline(fin, objectName);
				fin >> monsters.at(placeName).emplace(objectName, Renders::Objects::Npc()).first->second;
			}
		}
		fin.close();
	}

	BuildingsMap* Save::Buildings(const std::string& location)
	{
		std::map<std::string, BuildingsMap>::iterator iterator = buildings.find(location);
		if (iterator != buildings.end())
			return &iterator->second;
		return nullptr;
	}

	NpcMap* Save::Monsters(const std::string& location)
	{
		std::map<std::string, NpcMap>::iterator iterator = monsters.find(location);
		if (iterator != monsters.end())
			return &iterator->second;
		return nullptr;
	}

	PickUpMap* Save::Items(const std::string& location)
	{
		std::map<std::string, PickUpMap>::iterator iterator = items.find(location);
		if (iterator != items.end())
			return &iterator->second;
		return nullptr;
	}

	bool Save::SaveGame(const std::string& place) const
	{
		if (!std::filesystem::directory_entry("Save").exists() && !std::filesystem::create_directory("Save"))
		{
			std::ofstream efout("error.txt", std::ios_base::app);
			efout << "Cannot create directory \"Save\". Create it manually.";
			efout.close();
			return false;
		}
		std::ofstream fout("Save/" + player->Name() + ".sav", std::ios_base::trunc);
		if (!fout.good())
		{
			std::ofstream efout("error.txt", std::ios_base::app);
			efout << "Cannot save: " << player->Name() << std::endl;
			efout.close();
			return false;
		}
		fout << place << std::endl;
		fout << *player;
		fout << items.size() << std::endl;
		for (const auto& x : items)
		{
			fout << x.first << std::endl << x.second.size() << std::endl;
			for (const auto& y : x.second)
			{
				fout << y.first << std::endl;
				fout << y.second;
			}
		}
		fout << buildings.size() << std::endl;
		for (const auto& x : buildings)
		{
			fout << x.first << std::endl << x.second.size() << std::endl;
			for (const auto& y : x.second)
			{
				fout << y.first << std::endl;
				fout << y.second;
			}
		}
		fout << monsters.size() << std::endl;
		for (const auto& x : monsters)
		{
			fout << x.first << std::endl << x.second.size() << std::endl;
			for (const auto& y : x.second)
			{
				fout << y.first << std::endl;
				fout << y.second;
			}
		}
		fout.close();
		return true;
	}
}