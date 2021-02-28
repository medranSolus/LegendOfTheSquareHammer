/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "Stats.h"

namespace LOTSH
{
	enum class BodyPart : uint8_t { Weapon, Necklace, Ring, Head, Chest, Leg, Hands };
	enum class WeaponType : uint8_t { Axe, Hammer, Sword, Dagger };
	enum class ItemType : uint8_t { AttackScroll, EffectScroll, SpellScroll, Potion, StatsPotion, Armor, Weapon, Item };
	enum Direction : uint8_t { None = 0, N = 1, E = 2, W = 4, S = 8, Enter = 16 };

	namespace Renders::Objects
	{
		class Building;
	}

	struct CollisionEntrance
	{
		short Direction = Direction::None;
		Renders::Objects::Building* Building = nullptr;
	};

	struct Damage
	{
		int Normal = 0;
		int Fire = 0;
		int Ice = 0;
		int Magic = 0;
		int Darkness = 0;

		friend std::ofstream& operator<<(std::ofstream& fout, const Damage& damage);
		friend std::ifstream& operator>>(std::ifstream& fin, Damage& damage);
	};
}