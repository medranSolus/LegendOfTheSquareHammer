/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "Item.h"

namespace LOTSH::Renders::Items
{
	class Weapon : public Item
	{
		WeaponType type = WeaponType::Axe;
		Damage damage;
		short criticalChance = 0;
		short penetration = 0;
		MainStats minimumStatistics;
		Stats statistics;

	public:
		Weapon() = default;
		Weapon(WeaponType newType, const std::string& newName, const std::string& newDescription,
			const Damage& weaponDamage, short critical, short armorPenetration,
			size_t levelRequired, const MainStats& statisticsRequired,
			const Stats& newStatistics, const std::string& path, int x, int y)
			: Item(newName, newDescription, levelRequired, path, x, y), type(newType),
			damage(weaponDamage), criticalChance(critical), penetration(armorPenetration),
			minimumStatistics(statisticsRequired), statistics(newStatistics) {}
		virtual ~Weapon() = default;

		constexpr const Damage& Damage() const { return damage; }
		constexpr short CriticalChance() const { return criticalChance; }
		constexpr short ArmorPenetration() const { return penetration; }
		constexpr const MainStats& MinimumStatistics() const { return minimumStatistics; }
		constexpr const Stats& Statistics() const { return statistics; }
		constexpr WeaponType Type() const { return type; }

		inline LOTSH::ItemType ItemType() const override { return ItemType::Weapon; }
		inline std::ofstream& Write(std::ofstream& fout) const override { return fout << *this; }
		inline std::ifstream& Read(std::ifstream& fin) override { return fin >> *this; }

		friend std::ofstream& operator<<(std::ofstream& fout, const Weapon& weapon);
		friend std::ifstream& operator>>(std::ifstream& fin, Weapon& weapon);
	};
}