/*
Author: Marek Machliñski
Date:	9.02.2021
*/
#pragma once
#include "MainStats.h"

namespace LOTSH
{
	struct Stats
	{
		MainStats MainStatistics;
		int Endurance = 0;
		int Vitality = 0;
		int Armor = 0;
		short FireResistance = 0;
		short IceResistance = 0;
		short MagicResistance = 0;
		short DarknessResistance = 0;

		constexpr void LevelUp(const Stats& st);

		constexpr Stats operator-(const Stats& statistics) const { return *this + statistics * -1; }
		constexpr Stats& operator-=(const Stats& statistics) { return *this += statistics * -1; }
		constexpr Stats& operator+=(const Stats& statistics) { return *this = *this + statistics; }
		constexpr Stats operator*(short x) const;
		constexpr Stats operator+(const Stats& statistics) const;

		friend std::ofstream& operator<<(std::ofstream& fout, const Stats& statistics);
		friend std::ifstream& operator>>(std::ifstream& fin, Stats& statistics);
	};

	constexpr void Stats::LevelUp(const Stats& statistics)
	{
		MainStatistics += statistics.MainStatistics;
		Endurance += statistics.Endurance;
		Vitality += statistics.Vitality;
		FireResistance = short(0.1 * MainStatistics.Inteligence + 0.03 * MainStatistics.Strenght + 0.02 * MainStatistics.Dextermity + 0.06 * Endurance + 0.04 * Vitality);
		IceResistance = short(0.11 * MainStatistics.Inteligence + 0.02 * MainStatistics.Strenght + 0.01 * MainStatistics.Dextermity + 0.06 * Endurance + 0.05 * Vitality);
		MagicResistance = short(0.17 * MainStatistics.Inteligence + 0.05 * Endurance + 0.03 * Vitality);
		DarknessResistance = short(0.12 * MainStatistics.Inteligence + 0.02 * MainStatistics.Strenght + 0.01 * MainStatistics.Dextermity + 0.04 * Endurance + 0.06 * Vitality);
	}

	constexpr Stats Stats::operator*(short x) const
	{
		return
		{
			MainStatistics * x,
			Endurance * x,
			Vitality * x,
			Armor * x,
			FireResistance * x,
			IceResistance * x,
			MagicResistance * x,
			DarknessResistance * x
		};
	}

	constexpr Stats Stats::operator+(const Stats& statistics) const
	{
		return
		{
			MainStatistics + statistics.MainStatistics,
			Endurance + statistics.Endurance,
			Vitality + statistics.Vitality,
			Armor + statistics.Armor,
			FireResistance + statistics.FireResistance,
			IceResistance + statistics.IceResistance,
			MagicResistance + statistics.MagicResistance,
			DarknessResistance + statistics.DarknessResistance
		};
	}
}