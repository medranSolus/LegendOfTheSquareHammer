/*
Author: Marek Machliñski
Date:	9.02.2021
*/
#pragma once
#include <fstream>

namespace LOTSH
{
	struct MainStats
	{
		int Inteligence = 0;
		int Strenght = 0;
		int Dextermity = 0;

		constexpr bool operator>=(const MainStats& mainStatistics) { return Inteligence >= mainStatistics.Inteligence && Strenght >= mainStatistics.Strenght && Dextermity >= mainStatistics.Dextermity; }
		constexpr MainStats operator*(int x) const { return { Inteligence * x, Strenght * x, Dextermity * x }; }
		constexpr MainStats operator-(const MainStats& mainStatistics) const { return *this + mainStatistics * -1; }
		constexpr MainStats& operator-=(const MainStats& mainStatistics) { return *this += mainStatistics * -1; }
		constexpr MainStats& operator+=(const MainStats& mainStatistics) { return *this = *this + mainStatistics; }
		constexpr MainStats operator+(const MainStats& mainStatistics) const;

		friend std::ofstream& operator<<(std::ofstream& fout, const MainStats& mainStatistics);
		friend std::ifstream& operator>>(std::ifstream& fin, MainStats& mainStatistics);
	};

	constexpr MainStats MainStats::operator+(const MainStats& mainStatistics) const
	{
		return
		{
			Inteligence + mainStatistics.Inteligence,
			Strenght + mainStatistics.Strenght,
			Dextermity + mainStatistics.Dextermity
		};
	}
}