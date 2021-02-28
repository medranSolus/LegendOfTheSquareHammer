/*
Author: Marek Machliñski
Date:	9.02.2021
*/
#pragma once
#include "Item.h"

namespace LOTSH::Renders::Items::Magic
{
	class IScroll : public Item
	{
	protected:
		short manaCost = 0;
		MainStats minimumStatistics;

	public:
		IScroll() = default;
		inline IScroll(const std::string& newName, const std::string& newDescription, size_t levelRequired,
			const std::string& path, int x, int y, short manaCost, const MainStats& statisticsRequired)
			: Item(newName, newDescription, levelRequired, path, x, y), manaCost(manaCost), minimumStatistics(statisticsRequired) {}
		virtual ~IScroll() = default;

		constexpr short ManaCost() const { return manaCost; }
		constexpr const MainStats& MinimumStatistics() const { return minimumStatistics; }
	};
}