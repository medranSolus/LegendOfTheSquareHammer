/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "IScroll.h"

namespace LOTSH::Renders::Items::Magic
{
	// One time item upgrade
	class EffectScroll : public IScroll
	{
		Stats statisticsChange;

	public:
		EffectScroll() = default;
		EffectScroll(const std::string& newName, const std::string& newDescription, short mana, const Stats& newStatistics,
			size_t levelRequired, const MainStats& statisticsRequired, const std::string& path, int x, int y)
			: IScroll(newName, newDescription, levelRequired, path, x, y, mana, statisticsRequired), statisticsChange(newStatistics) {}
		virtual ~EffectScroll() = default;

		constexpr const MainStats& MinimumStatistics() const { return minimumStatistics; }
		inline LOTSH::ItemType ItemType() const override { return ItemType::EffectScroll; }
		inline std::ofstream& Write(std::ofstream& fout) const override { return fout << *this; }
		inline std::ifstream& Read(std::ifstream& fin) override { return fin >> *this; }

		friend std::ofstream& operator<<(std::ofstream& fout, const EffectScroll& scroll);
		friend std::ifstream& operator>>(std::ifstream& fin, EffectScroll& scroll);
	};
}