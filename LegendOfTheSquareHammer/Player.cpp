/*
Author: Marek Machliñski
Date:	27.02.2021
*/
#include "stdafx.h"
#include "Player.h"
#include "StatsPotion.h"
#include "AttackScroll.h"
#include "EffectScroll.h"
#include "SpellScroll.h"
#include "Data.h"

namespace LOTSH::Renders::Objects
{
	Player::Player(const std::string& newName, const Stats& statistics)
		: name(newName), playerStatistics(statistics)
	{
		for (short i = 0; i < 7; ++i)
			currentArmor.at(i) = nullptr;
		for (short i = 1; i < 5; ++i)
			textures.emplace_back("Textures/Player" + std::to_string(i) + ".png");

		health = maxHealth = playerStatistics.Vitality * 12;
		mana = maxMana = playerStatistics.MainStatistics.Inteligence * 7;
		nextLevelExperience = 200;
		damage.Normal = (playerStatistics.MainStatistics.Strenght + playerStatistics.MainStatistics.Dextermity) / 2;

		X = 2000;
		Y = 2000;
		position = { 905, 466, static_cast<int>(textures.at(0).Width() * 0.4 * ProgramData::Data::SCREEN_SCALE), static_cast<int>(textures.at(0).Height() * 0.4 * ProgramData::Data::SCREEN_SCALE) };
		collision.w = static_cast<int>(0.46 * position.w);
		collision.h = static_cast<int>(0.18 * position.h);
		collision.x = position.x + (position.w - collision.w) / 2;
		collision.y = position.y + position.h - collision.h;
		ProgramData::Data::CenterX() = (ProgramData::Data::SCREEN_WIDTH - position.w) / 2;
		ProgramData::Data::CenterY() = (ProgramData::Data::SCREEN_HEIGHT - position.h) / 2;
	}

	std::pair<Items::Weapon*, std::array<Items::Armor*, 7>> Player::Equipment()
	{
		std::array<Items::Armor*, 7> armor = { nullptr };
		for (uint8_t i = 0; i < 7; ++i)
			armor.at(i) = currentArmor.at(i).get();
		return { currentWeapon.get(), armor };
	}

	void Player::Render()
	{
		textures.at(textureNumber).Render(position, flip);
		if (animationDelay == 0)
		{
			if (!reverseAnimation && textureNumber < textures.size() - 1)
				++textureNumber;
			else if (reverseAnimation && textureNumber > 0)
				--textureNumber;
			else if (textureNumber == textures.size() - 1)
				reverseAnimation = true;
			else if (textureNumber == 0)
				reverseAnimation = false;
			animationDelay = 4;
		}
		else
			--animationDelay;
	}

	bool Player::AddExperience(size_t earnedExperience)
	{
		experience += earnedExperience;
		if (experience >= nextLevelExperience)
		{
			experience -= nextLevelExperience;
			++level;
			++levelPoints;
			nextLevelExperience = level * 190 + static_cast<size_t>(pow(level, 2)) * 10;
			return true;
		}
		return false;
	}

	void Player::LevelUp(const Stats& additionalStatistics)
	{
		Stats equipmentStatistics;
		if (currentWeapon != nullptr)
			equipmentStatistics = currentWeapon->Statistics();
		for (short i = 0; i < 7; ++i)
			if (currentArmor.at(i) != nullptr)
				equipmentStatistics += currentArmor.at(i)->Statistics();
		playerStatistics -= equipmentStatistics;
		playerStatistics.LevelUp(additionalStatistics);
		playerStatistics += equipmentStatistics;
		health = maxHealth = playerStatistics.Vitality * 12;
		mana = maxMana = playerStatistics.MainStatistics.Inteligence * 7;
		damage.Normal = (playerStatistics.MainStatistics.Strenght + playerStatistics.MainStatistics.Dextermity) / 2;
		levelPoints = 0;
	}

	bool Player::Wear(const std::string& name, short ringNumber)
	{
		ItemMap::iterator iterator = items.find(name);
		if (iterator == items.end())
			return false;
		auto& item = items.at(name);
		if (item->MinimumLevel() > level)
			return false;
		switch (item->ItemType())
		{
		case ItemType::Weapon:
		{
			if (playerStatistics.MainStatistics >= static_cast<Items::Weapon*>(item.get())->MinimumStatistics())
			{
				if (currentWeapon != nullptr)
				{
					std::unique_ptr<Items::Weapon> weapon = std::move(currentWeapon);
					playerStatistics -= weapon->Statistics();
					currentWeapon = std::unique_ptr<Items::Weapon>(static_cast<Items::Weapon*>(item.release()));
					item = std::move(weapon);
				}
				else
					currentWeapon = std::unique_ptr<Items::Weapon>(static_cast<Items::Weapon*>(item.release()));
				playerStatistics += currentWeapon->Statistics();
				break;
			}
			return false;
		}
		case ItemType::Armor:
		{
			if (playerStatistics.MainStatistics >= static_cast<Items::Armor*>(item.get())->MinimumStatistics())
			{
				short part;
				switch (static_cast<Items::Armor*>(item.get())->Part())
				{
				case BodyPart::Necklace:
				{
					part = 0;
					break;
				}
				case BodyPart::Ring:
				{
					if (ringNumber != 0 || ringNumber != 1)
						return false;
					part = 1 + ringNumber;
					break;
				}
				case BodyPart::Head:
				{
					part = 3;
					break;
				}
				case BodyPart::Chest:
				{
					part = 4;
					break;
				}
				case BodyPart::Leg:
				{
					part = 5;
					break;
				}
				case BodyPart::Hands:
				{
					part = 6;
					break;
				}
				default:
					return false;
				}
				if (currentArmor.at(part) != nullptr)
				{
					std::unique_ptr<Items::Armor> armor = std::move(currentArmor.at(part));
					playerStatistics -= armor->Statistics();
					currentArmor.at(part) = std::unique_ptr<Items::Armor>(static_cast<Items::Armor*>(item.release()));
					item = std::move(armor);
				}
				else
					currentArmor.at(part) = std::unique_ptr<Items::Armor>(static_cast<Items::Armor*>(item.release()));
				playerStatistics += currentArmor.at(part)->Statistics();
				break;
			}
		}
		default:
			return false;
		}
		if (item != nullptr)
			items.insert({ item->Name(), std::move(item) });
		items.erase(iterator);
		return true;
	}

	bool Player::TakeOff(BodyPart bodyPart, short ringNumber)
	{
		switch (bodyPart)
		{
		case BodyPart::Weapon:
		{
			playerStatistics -= currentWeapon->Statistics();
			items.insert({ currentWeapon->Name(), std::move(currentWeapon) });
			break;
		}
		default:
		{
			short part;
			switch (bodyPart)
			{
			case BodyPart::Necklace:
			{
				part = 0;
				break;
			}
			case BodyPart::Ring:
			{
				if (ringNumber != 0 || ringNumber != 1)
					return false;
				part = 1 + ringNumber;
				break;
			}
			case BodyPart::Head:
			{
				part = 3;
				break;
			}
			case BodyPart::Chest:
			{
				part = 4;
				break;
			}
			case BodyPart::Leg:
			{
				part = 5;
				break;
			}
			case BodyPart::Hands:
			{
				part = 6;
				break;
			}
			default:
				return false;
			}
			playerStatistics -= currentArmor.at(part)->Statistics();
			items.insert({ currentArmor.at(part)->Name(), std::move(currentArmor.at(part)) });
			break;
		}
		}
		return true;
	}

	std::unique_ptr<Items::Item> Player::ThrowOut(const std::string& name)
	{
		ItemMap::iterator iterator = items.find(name);
		if (iterator != items.end())
		{
			std::unique_ptr<Items::Item> item = std::move(items.at(name));
			items.erase(iterator);
			return item;
		}
		return nullptr;
	}

	void Player::GetItem(std::unique_ptr<Items::Item> item)
	{
		if (item != nullptr)
			items.insert({ item->Name(), std::move(item) });
	}

	void Player::SetPosition(int x, int y)
	{
		position.x = x;
		collision.x = position.x + (position.w - collision.w) / 2;
		position.y = y;
		collision.y = position.y + position.h - collision.h;
	}

	void Player::ChangePosition(int x, int y)
	{
		position.x += x;
		collision.x += x;
		position.y += y;
		collision.y += y;
	}

	bool Player::Wound(const LOTSH::Damage& damageTaken)
	{
		size_t dmg = damageTaken.Normal > playerStatistics.Armor ? damageTaken.Normal - playerStatistics.Armor : 0;
		if (dmg < health)
		{
			dmg += damageTaken.Darkness * (100 - playerStatistics.DarknessResistance) / 100
				+ damageTaken.Fire * (100 - playerStatistics.FireResistance) / 100
				+ damageTaken.Ice * (100 - playerStatistics.IceResistance) / 100
				+ damageTaken.Magic * (100 - playerStatistics.MagicResistance) / 100;
		}
		if (dmg >= health)
		{
			health = 0;
			return false;
		}
		health -= dmg;
		return true;
	}

	bool Player::Use(const std::string& name)
	{
		ItemMap::iterator iterator = items.find(name);
		if (iterator == items.end())
			return false;
		auto& item = items.at(name);
		if (item->MinimumLevel() > level)
			return false;
		switch (item->ItemType())
		{
		case ItemType::StatsPotion:
			LevelUp(static_cast<Items::SingleUse::StatsPotion*>(item.get())->StatisticsChange());
		case ItemType::Potion:
		{
			Heal(static_cast<Items::SingleUse::Potion*>(item.get())->Health());
			RegenerateMana(static_cast<Renders::Items::SingleUse::Potion*>(item.get())->Mana());
			break;
		}
		case ItemType::SpellScroll:
		{
			break;
		}
		case ItemType::EffectScroll:
		{
			break;
		}
		case ItemType::AttackScroll:
		{
			break;
		}
		default:
			return false;
		}
		items.erase(iterator);
		return true;
	}

	std::ofstream& operator<<(std::ofstream& fout, const Player& player)
	{
		fout << player.name << std::endl
			<< player.health << ' '
			<< player.maxHealth << ' '
			<< player.mana << ' '
			<< player.maxMana << ' '
			<< player.money << ' '
			<< player.experience << ' '
			<< player.nextLevelExperience << ' '
			<< player.level << ' '
			<< player.X << ' '
			<< player.Y << ' '
			<< player.levelPoints << std::endl;
		fout << player.playerStatistics << std::endl;
		fout << player.damage << std::endl;

		if (player.currentWeapon != nullptr)
		{
			fout << "1\n";
			fout << *player.currentWeapon << std::endl;
		}
		else
			fout << "0\n";
		for (auto& armor : player.currentArmor)
		{
			if (armor != nullptr)
			{
				fout << "1\n";
				fout << *armor << std::endl;
			}
			else
				fout << "0\n";
		}
		fout << player.position << std::endl;
		fout << player.collision << std::endl;
		fout << player.textures.size() << std::endl;
		for (auto& tex : player.textures)
			fout << tex << std::endl;

		fout << player.items.size() << std::endl;
		for (auto& item : player.items)
		{
			fout << item.first << std::endl;
			fout << std::to_string(static_cast<uint8_t>(item.second->ItemType())) << std::endl;
			item.second->Write(fout) << std::endl;
		}
		return fout;
	}

	std::ifstream& operator>>(std::ifstream& fin, Player& player)
	{
		if (fin.peek() == 10)
			fin.ignore();
		getline(fin, player.name);
		fin >> player.health
			>> player.maxHealth
			>> player.mana
			>> player.maxMana
			>> player.money
			>> player.experience
			>> player.nextLevelExperience
			>> player.level
			>> player.X
			>> player.Y
			>> player.levelPoints;
		size_t temporary = 0;
		fin >> player.playerStatistics >> player.damage >> temporary;
		if (temporary == 1)
		{
			player.currentWeapon = std::make_unique<Items::Weapon>();
			fin >> *player.currentWeapon;
		}
		for (short i = 0; i < 7; ++i)
		{
			fin >> temporary;
			if (temporary == 1)
			{
				player.currentArmor.at(i) = std::make_unique<Items::Armor>();
				fin >> *player.currentArmor.at(i);
			}
			else
				player.currentArmor.at(i) = nullptr;
		}
		fin >> player.position >> player.collision;
		ProgramData::Data::CenterX() = (ProgramData::Data::SCREEN_WIDTH - player.position.w) / 2;
		ProgramData::Data::CenterY() = (ProgramData::Data::SCREEN_HEIGHT - player.position.h) / 2;
		fin >> temporary;
		for (size_t i = 0; i < temporary; ++i)
		{
			player.textures.emplace_back();
			fin >> player.textures.back();
		}
		fin >> temporary;
		std::string itemName = "";
		int itemType;
		std::unique_ptr<Items::Item> item = nullptr;
		for (size_t i = 0; i < temporary; ++i)
		{
			if (fin.peek() == 10)
				fin.ignore();
			getline(fin, itemName);
			fin >> itemType;
			switch (static_cast<ItemType>(itemType))
			{
			case ItemType::AttackScroll:
			{
				item = std::make_unique<Items::Magic::AttackScroll>();
				break;
			}
			case ItemType::EffectScroll:
			{
				item = std::make_unique<Items::Magic::EffectScroll>();
				break;
			}
			case ItemType::SpellScroll:
			{
				item = std::make_unique<Items::Magic::SpellScroll>();
				break;
			}
			case ItemType::Potion:
			{
				item = std::make_unique<Items::SingleUse::Potion>();
				break;
			}
			case ItemType::StatsPotion:
			{
				item = std::make_unique<Items::SingleUse::StatsPotion>();
				break;
			}
			case ItemType::Armor:
			{
				item = std::make_unique<Items::Armor>();
				break;
			}
			case ItemType::Weapon:
			{
				item = std::make_unique<Items::Weapon>();
				break;
			}
			case ItemType::Item:
			default:
			{
				std::ofstream fout("error.txt", std::ios_base::app);
				fout << "Unkown Item in item list: " << itemName << std::endl;
				fout.close();
				continue;
			}
			}
			item->Read(fin);
			player.items.insert({ itemName, std::move(item) });
			itemName = "";
		}
		return fin;
	}
}