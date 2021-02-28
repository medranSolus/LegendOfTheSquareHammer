/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "Label.h"
#include <vector>
#include <utility>

namespace LOTSH::ProgramData
{
	struct LabelsData
	{
		std::vector<Renders::Label> Mini;
		std::vector<Renders::Label> Menu;
		std::vector<Renders::Label> Items;
		std::vector<Renders::Label> Stats;
		std::vector<Renders::Label> Resistance;
		std::vector<Renders::Label> Damage;
		std::vector<std::pair<Renders::Label, Renders::Label>> Arrows;
		std::vector<Renders::Label> Info;
		std::vector<Renders::Label> Armor;
		std::vector<Renders::Label> Weapon;
		std::vector<Renders::Label> Potion;
	};
}