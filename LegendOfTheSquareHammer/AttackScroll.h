/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "IScroll.h"

namespace LOTSH::Renders::Items::Magic
{
	// One time attack spell
	class AttackScroll : public IScroll
	{
		Damage damage;

	public:
		AttackScroll() = default;
		AttackScroll(const std::string& newName, const std::string& newDescription, short mana, const Damage& newDamage,
			size_t levelRequired, const MainStats& statisticsRequired, const std::string& path, int x, int y)
			: IScroll(newName, newDescription, levelRequired, path, x, y, mana, statisticsRequired), damage(newDamage) {}
		virtual ~AttackScroll() = default;

		constexpr const Damage& Damage() const { return damage; }
		inline LOTSH::ItemType ItemType() const override { return ItemType::AttackScroll; }
		inline std::ofstream& Write(std::ofstream& fout) const override { return fout << *this; }
		inline std::ifstream& Read(std::ifstream& fin) override { return fin >> *this; }

		friend std::ofstream& operator<<(std::ofstream& fout, const AttackScroll& scroll);
		friend std::ifstream& operator>>(std::ifstream& fin, AttackScroll& scroll);
	};
}