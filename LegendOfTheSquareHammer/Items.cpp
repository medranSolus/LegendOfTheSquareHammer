/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#include "stdafx.h"
#include "Weapon.h"
#include "Armor.h"
#include "StatsPotion.h"
#include "SpellScroll.h"
#include "EffectScroll.h"
#include "AttackScroll.h"
#include "Data.h"

namespace LOTSH::Renders::Items
{
	Item::Item(const std::string& newName, const std::string& newDescription,
		size_t levelRequired, const std::string& path, int x, int y)
		: name(newName), description(newDescription), minimumLevel(levelRequired)
	{
		if (!texture.LoadFile(path))
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << ": Item " << name << " texture load error\n";
			fout.close();
			exit(0b100000000);
		}
		position = { x, y, static_cast<int>(50 * ProgramData::Data::SCREEN_SCALE), static_cast<int>(50 * ProgramData::Data::SCREEN_SCALE) };
	}

	std::ofstream& operator<<(std::ofstream& fout, const Weapon& weapon)
	{
		fout << std::to_string(static_cast<uint8_t>(weapon.type)) << std::endl
			<< weapon.name << std::endl
			<< weapon.description << std::endl;
		fout << weapon.damage << std::endl
			<< weapon.criticalChance << ' '
			<< weapon.penetration << ' '
			<< weapon.minimumLevel << std::endl;
		fout << weapon.minimumStatistics << std::endl;
		fout << weapon.statistics << std::endl;
		fout << weapon.position << std::endl;
		fout << weapon.texture;
		return fout;
	}
	std::ifstream& operator>>(std::ifstream& fin, Weapon& weapon)
	{
		int tmp;
		fin >> tmp;
		weapon.type = (WeaponType)tmp;
		if (fin.peek() == 10)
			fin.ignore();
		getline(fin, weapon.name);
		if (fin.peek() == 10)
			fin.ignore();
		getline(fin, weapon.description);
		fin >> weapon.damage
			>> weapon.criticalChance
			>> weapon.penetration
			>> weapon.minimumLevel;
		fin >> weapon.minimumStatistics
			>> weapon.statistics
			>> weapon.position
			>> weapon.texture;
		return fin;
	}

	std::ofstream& operator<<(std::ofstream& fout, const Armor& armor)
	{
		fout << std::to_string(static_cast<uint8_t>(armor.part)) << std::endl
			<< armor.name << std::endl
			<< armor.description << std::endl;
		fout << armor.statistics << std::endl
			<< armor.minimumLevel << std::endl;
		fout << armor.minimumStatistics << std::endl;
		fout << armor.position << std::endl;
		fout << armor.texture;
		return fout;
	}
	std::ifstream& operator>>(std::ifstream& fin, Armor& armor)
	{
		int tmp;
		fin >> tmp;
		armor.part = (BodyPart)tmp;
		if (fin.peek() == 10)
			fin.ignore();
		getline(fin, armor.name);
		if (fin.peek() == 10)
			fin.ignore();
		getline(fin, armor.description);
		fin >> armor.statistics
			>> armor.minimumLevel;
		fin >> armor.minimumStatistics
			>> armor.position
			>> armor.texture;
		return fin;
	}

	namespace SingleUse
	{
		std::ofstream& operator<<(std::ofstream& fout, const Potion& potion)
		{
			fout << potion.name << std::endl
				<< potion.description << std::endl
				<< potion.mana << ' '
				<< potion.health << ' '
				<< potion.minimumLevel << std::endl;
			fout << potion.position << std::endl;
			fout << potion.texture;
			return fout;
		}
		std::ifstream& operator>>(std::ifstream& fin, Potion& potion)
		{
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, potion.name);
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, potion.description);
			fin >> potion.mana
				>> potion.health
				>> potion.minimumLevel;
			fin >> potion.position
				>> potion.texture;
			return fin;
		}

		std::ofstream& operator<<(std::ofstream& fout, const StatsPotion& potion)
		{
			fout << potion.statisticsChange << std::endl;
			fout << static_cast<const Potion&>(potion);
			return fout;
		}
		std::ifstream& operator>>(std::ifstream& fin, StatsPotion& potion)
		{
			fin >> potion.statisticsChange >> static_cast<Potion&>(potion);
			return fin;
		}
	}
	namespace Magic
	{
		std::ofstream& operator<<(std::ofstream& fout, const SpellScroll& scroll)
		{
			fout << scroll.name << std::endl
				<< scroll.description << std::endl
				<< scroll.manaCost << ' '
				<< scroll.duration << ' '
				<< scroll.minimumLevel << std::endl;
			fout << scroll.damageOverTime << std::endl;
			fout << scroll.statisticsChangeOverTime << std::endl;
			fout << scroll.minimumStatistics << std::endl;
			fout << scroll.position << std::endl;
			fout << scroll.texture;
			return fout;
		}
		std::ifstream& operator>>(std::ifstream& fin, SpellScroll& scroll)
		{
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, scroll.name);
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, scroll.description);
			fin >> scroll.manaCost
				>> scroll.duration
				>> scroll.minimumLevel;
			fin >> scroll.damageOverTime
				>> scroll.statisticsChangeOverTime
				>> scroll.minimumStatistics
				>> scroll.position
				>> scroll.texture;
			return fin;
		}

		std::ofstream& operator<<(std::ofstream& fout, const EffectScroll& scroll)
		{
			fout << scroll.name << std::endl
				<< scroll.description << std::endl
				<< scroll.manaCost << ' '
				<< scroll.minimumLevel << std::endl;
			fout << scroll.minimumStatistics << std::endl;
			fout << scroll.position << std::endl;
			fout << scroll.texture;
			return fout;
		}
		std::ifstream& operator>>(std::ifstream& fin, EffectScroll& scroll)
		{
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, scroll.name);
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, scroll.description);
			fin >> scroll.manaCost
				>> scroll.minimumLevel;
			fin >> scroll.minimumStatistics
				>> scroll.position
				>> scroll.texture;
			return fin;
		}

		std::ofstream& operator<<(std::ofstream& fout, const AttackScroll& scroll)
		{
			fout << scroll.name << std::endl
				<< scroll.description << std::endl
				<< scroll.manaCost << ' '
				<< scroll.minimumLevel << std::endl;
			fout << scroll.damage << std::endl;
			fout << scroll.minimumStatistics << std::endl;
			fout << scroll.position << std::endl;
			fout << scroll.texture;
			return fout;
		}
		std::ifstream& operator>>(std::ifstream& fin, AttackScroll& scroll)
		{
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, scroll.name);
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, scroll.description);
			fin >> scroll.manaCost
				>> scroll.minimumLevel;
			fin >> scroll.damage
				>> scroll.minimumStatistics
				>> scroll.position
				>> scroll.texture;
			return fin;
		}
	}
}