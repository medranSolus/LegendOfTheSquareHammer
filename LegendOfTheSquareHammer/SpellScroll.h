/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "IScroll.h"

namespace LOTSH::Renders::Items::Magic
{
	// Effect on player/item/npc that changes stats and/or deal damage over time
	class SpellScroll : public IScroll
	{
		short duration = 0;
		Damage damageOverTime;
		Stats statisticsChangeOverTime;

	public:
		SpellScroll() = default;
		SpellScroll(const std::string& newName, const std::string& newDescription, short mana,
			const Damage& damage, const Stats& statistics, size_t levelRequired,
			const MainStats& statisticsRequired, const std::string& path, int x, int y)
			: IScroll(newName, newDescription, levelRequired, path, x, y, mana, statisticsRequired),
			damageOverTime(damage), statisticsChangeOverTime(statistics) {}
		virtual ~SpellScroll() = default;

		constexpr short Duration() const { return duration; }
		constexpr const Damage& Damage() const { return damageOverTime; }
		constexpr const Stats& StatisticsChange() const { return statisticsChangeOverTime; }
		inline LOTSH::ItemType ItemType() const override { return ItemType::SpellScroll; }
		inline std::ofstream& Write(std::ofstream& fout) const override { return fout << *this; }
		inline std::ifstream& Read(std::ifstream& fin) override { return fin >> *this; }

		friend std::ofstream& operator<<(std::ofstream& fout, const SpellScroll& scroll);
		friend std::ifstream& operator>>(std::ifstream& fin, SpellScroll& scroll);
	};
}