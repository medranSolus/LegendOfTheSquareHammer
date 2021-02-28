/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#include "Structs.h"

namespace LOTSH
{
	std::ofstream& operator<<(std::ofstream& fout, const Damage& damage)
	{
		fout << damage.Normal << ' '
			<< damage.Fire << ' '
			<< damage.Ice << ' '
			<< damage.Magic << ' '
			<< damage.Darkness;
		return fout;
	}
	std::ifstream& operator>>(std::ifstream& fin, Damage& damage)
	{
		fin >> damage.Normal >> damage.Fire >> damage.Ice >> damage.Magic >> damage.Darkness;
		return fin;
	}

	std::ofstream& operator<<(std::ofstream& fout, const MainStats& mainStatistics)
	{
		fout << mainStatistics.Inteligence << ' '
			<< mainStatistics.Strenght << ' '
			<< mainStatistics.Dextermity;
		return fout;
	}
	std::ifstream& operator>>(std::ifstream& fin, MainStats& mainStatistics)
	{
		fin >> mainStatistics.Inteligence >> mainStatistics.Strenght >> mainStatistics.Dextermity;
		return fin;
	}

	std::ofstream& operator<<(std::ofstream& fout, const Stats& statistics)
	{
		fout << statistics.MainStatistics << std::endl
			<< statistics.Endurance << ' '
			<< statistics.Vitality << ' '
			<< statistics.Armor << ' '
			<< statistics.FireResistance << ' '
			<< statistics.IceResistance << ' '
			<< statistics.MagicResistance << ' '
			<< statistics.DarknessResistance;
		return fout;
	}
	std::ifstream& operator>>(std::ifstream& fin, Stats& statistics)
	{
		fin >> statistics.MainStatistics
			>> statistics.Endurance
			>> statistics.Vitality
			>> statistics.Armor
			>> statistics.FireResistance
			>> statistics.IceResistance
			>> statistics.MagicResistance
			>> statistics.DarknessResistance;
		return fin;
	}
}