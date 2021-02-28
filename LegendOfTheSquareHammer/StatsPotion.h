/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "Potion.h"

namespace LOTSH::Renders::Items::SingleUse
{
	class StatsPotion : public Potion
	{
		Stats statisticsChange;

	public:
		StatsPotion() = default;
		StatsPotion(const std::string& newName, const std::string& newDescription, int mana, int health,
			size_t levelRequired, const std::string& path, int x, int y, const Stats& statistics)
			: Potion(newName, newDescription, mana, health, levelRequired, path, x, y), statisticsChange(statistics) {}
		virtual ~StatsPotion() = default;

		constexpr const Stats& StatisticsChange() const { return statisticsChange; }
		inline LOTSH::ItemType ItemType() const override { return ItemType::StatsPotion; }
		inline std::ofstream& Write(std::ofstream& fout) const override { return fout << *this; }
		inline std::ifstream& Read(std::ifstream& fin) override { return fin >> *this; }

		friend std::ofstream& operator<<(std::ofstream& fout, const StatsPotion& potion);
		friend std::ifstream& operator>>(std::ifstream& fin, StatsPotion& potion);
	};
}