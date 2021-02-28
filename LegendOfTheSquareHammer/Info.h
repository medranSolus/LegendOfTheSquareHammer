/*
Author: Marek Machliñski
Date: 20.04.2018
*/
#pragma once
#include "Player.h"
#include "Data.h"
#include "EventHandler.h"
#include "StatsPotion.h"
#include "AttackScroll.h"
#include "EffectScroll.h"
#include "SpellScroll.h"

namespace LOTSH
{
	class Info
	{
		static void ArmorInfo(Renders::Items::Armor& item, const Stats& statistics);
		static void WeaponInfo(Renders::Items::Weapon& item, const Stats& statistics);
		static void PotionInfo(Renders::Items::SingleUse::Potion& item);
		static void SetStatsLabels(const Stats& currentStatistics, const Damage& currentDamage);

	public:
		Info() = delete;

		static void ItemInfo(Renders::Items::Item& item, const Renders::Objects::Player& player);
		static void PlayerStatistics(Renders::Objects::Player& player);
		static void PlayerEquipment(Renders::Objects::Player& player);
	};
}