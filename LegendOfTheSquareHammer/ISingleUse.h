/*
Author: Marek Machliñski
Date:	9.02.2021
*/
#pragma once
#include "Item.h"

namespace LOTSH::Renders::Items::SingleUse
{
	class ISingleUse : public Item
	{
	protected:
		int mana = 0;
		int health = 0;

	public:
		ISingleUse() = default;
		ISingleUse(const std::string& newName, const std::string& newDescription,
			size_t levelRequired, const std::string& path, int x, int y, int mana, int health)
			: Item(newName, newDescription, levelRequired, path, x, y), mana(mana), health(health) {}
		virtual ~ISingleUse() = default;

		constexpr int Mana() const { return mana; }
		constexpr int Health() const { return health; }
	};
}