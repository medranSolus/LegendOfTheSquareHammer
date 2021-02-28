/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "ISingleUse.h"

namespace LOTSH::Renders::Items::SingleUse
{
	class Potion : public ISingleUse
	{
	public:
		Potion() = default;
		Potion(const std::string& newName, const std::string& newDescription, int mana,
			int health, size_t levelRequired, const std::string& path, int x, int y)
			: ISingleUse(newName, newDescription, levelRequired, path, x, y, mana, health) {}
		virtual ~Potion() = default;

		inline LOTSH::ItemType ItemType() const override { return ItemType::Potion; }
		inline std::ofstream& Write(std::ofstream& fout) const override { return fout << *this; }
		inline std::ifstream& Read(std::ifstream& fin) override { return fin >> *this; }

		friend std::ofstream& operator<<(std::ofstream& fout, const Potion& potion);
		friend std::ifstream& operator>>(std::ifstream& fin, Potion& potion);
	};
}