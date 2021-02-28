/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#include "stdafx.h"
#include "PickUp.h"
#include "StatsPotion.h"
#include "AttackScroll.h"
#include "EffectScroll.h"
#include "SpellScroll.h"

namespace LOTSH::Renders::Objects
{
	PickUp::PickUp(const PickUp& p) noexcept : IObject(p)
	{
		if (p.item != nullptr)
		{
			switch (p.item->ItemType())
			{
			case ItemType::Armor:
			{
				item = std::make_unique<Items::Armor>(static_cast<const Items::Armor&>(*p.item));
				break;
			}
			case ItemType::Weapon:
			{
				item = std::make_unique<Items::Weapon>(static_cast<const Items::Weapon&>(*p.item));
				break;
			}
			case ItemType::AttackScroll:
			{
				item = std::make_unique<Items::Magic::AttackScroll>(static_cast<const Items::Magic::AttackScroll&>(*p.item));
				break;
			}
			case ItemType::EffectScroll:
			{
				item = std::make_unique<Items::Magic::EffectScroll>(static_cast<const Items::Magic::EffectScroll&>(*p.item));
				break;
			}
			case ItemType::SpellScroll:
			{
				item = std::make_unique<Items::Magic::SpellScroll>(static_cast<const Items::Magic::SpellScroll&>(*p.item));
				break;
			}
			case ItemType::Potion:
			{
				item = std::make_unique<Items::SingleUse::Potion>(static_cast<const Items::SingleUse::Potion&>(*p.item));
				break;
			}
			case ItemType::StatsPotion:
			{
				item = std::make_unique<Items::SingleUse::StatsPotion>(static_cast<const Items::SingleUse::StatsPotion&>(*p.item));
				break;
			}
			}
		}
	}

	void PickUp::SetPosition(int x, int y)
	{
		item->SetPosition(x, y);
		collision = item->Position();
	}

	void PickUp::ChangePosition(int x, int y)
	{
		item->ChangePosition(x, y);
		collision = item->Position();
	}

	void PickUp::operator()(Player& player)
	{
		player.GetItem(std::move(item));
		this->~PickUp();
	}

	std::ofstream& operator<<(std::ofstream& fout, const PickUp& pickUp)
	{
		fout << pickUp.X << ' ' << pickUp.Y << ' ' << std::to_string(static_cast<uint8_t>(pickUp.item->ItemType())) << std::endl;
		pickUp.item->Write(fout) << std::endl;
		fout << pickUp.collision << std::endl;
		return fout;
	}

	std::ifstream& operator>>(std::ifstream& fin, PickUp& pickUp)
	{
		uint8_t itemType;
		fin >> pickUp.X >> pickUp.Y >> itemType;
		switch (static_cast<ItemType>(itemType - '0'))
		{
		case ItemType::AttackScroll:
		{
			pickUp.item = std::make_unique<Items::Magic::AttackScroll>();
			break;
		}
		case ItemType::EffectScroll:
		{
			pickUp.item = std::make_unique<Items::Magic::EffectScroll>();
			break;
		}
		case ItemType::SpellScroll:
		{
			pickUp.item = std::make_unique<Items::Magic::SpellScroll>();
			break;
		}
		case ItemType::Potion:
		{
			pickUp.item = std::make_unique<Items::SingleUse::Potion>();
			break;
		}
		case ItemType::StatsPotion:
		{
			pickUp.item = std::make_unique<Items::SingleUse::StatsPotion>();
			break;
		}
		case ItemType::Armor:
		{
			pickUp.item = std::make_unique<Items::Armor>();
			break;
		}
		case ItemType::Weapon:
		{
			pickUp.item = std::make_unique<Items::Weapon>();
			break;
		}
		case ItemType::Item:
		default:
			exit(1534);
		}
		pickUp.item->Read(fin);
		fin >> pickUp.collision;
		return fin;
	}
}