/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "Item.h"

namespace LOTSH::Renders::Items
{
	class Armor : public Item
	{
		BodyPart part = BodyPart::Necklace;
		Stats statistics;
		MainStats minimumStatistics;

	public:
		Armor() = default;
		Armor(BodyPart type, const std::string& newName, const std::string& newDescription, const Stats& newStatistics,
			size_t levelRequired, const MainStats& statisticsRequired, const std::string& path, int x, int y)
			: Item(newName, newDescription, levelRequired, path, x, y), part(type), statistics(newStatistics), minimumStatistics(statisticsRequired) {}
		virtual ~Armor() = default;

		constexpr const BodyPart& Part() const { return part; }
		constexpr const Stats& Statistics() const { return statistics; }
		constexpr const MainStats& MinimumStatistics() const { return minimumStatistics; }

		inline LOTSH::ItemType ItemType() const override { return ItemType::Armor; }
		inline std::ofstream& Write(std::ofstream& fout) const override { return fout << *this; }
		inline std::ifstream& Read(std::ifstream& fin) override { return fin >> *this; }

		friend std::ofstream& operator<<(std::ofstream& fout, const Armor& armor);
		friend std::ifstream& operator>>(std::ifstream& fin, Armor& armor);
	};
}