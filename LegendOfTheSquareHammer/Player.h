/*
Author: Marek Machliñski
Date:	27.02.2021
*/
#pragma once
#include "IObject.h"
#include "Armor.h"
#include "Weapon.h"
#include <map>
#include <vector>
#include <array>
#include <utility>
#include <algorithm>

namespace LOTSH::Renders::Objects
{
	typedef std::map<std::string, std::unique_ptr<Items::Item>> ItemMap;

	class Player : public IObject
	{
		std::string name = "player";
		size_t health = 0;
		size_t maxHealth = 0;
		size_t mana = 0;
		size_t maxMana = 0;
		size_t money = 0;
		size_t experience = 0;
		size_t nextLevelExperience = 0;
		size_t level = 1;
		size_t levelPoints = 0;
		Stats playerStatistics;
		Damage damage;
		std::unique_ptr<Items::Weapon> currentWeapon = nullptr;
		std::array<std::unique_ptr<Items::Armor>, 7> currentArmor = { nullptr };
		ItemMap items;
		SDL_Rect position = {};
		std::vector<Renders::Texture> textures;
		size_t textureNumber = 0;
		size_t animationDelay = 4;
		bool reverseAnimation = false;
		SDL_RendererFlip flip = SDL_FLIP_NONE;

	public:
		Player() = default;
		Player(const std::string& name, const Stats& statistics);
		virtual ~Player() = default;

		constexpr const std::string& Name() const { return name; }
		constexpr const Damage& Damage() const { return damage; }
		constexpr void SetTextureFlip(SDL_RendererFlip textureFlip) { flip = textureFlip; }
		constexpr void TakeMoney(size_t moneyTaken) { money += moneyTaken; }
		constexpr size_t Health() const { return health; }
		constexpr size_t Mana() const { return mana; }
		constexpr size_t MaxHealth() const { return maxHealth; }
		constexpr size_t MaxMana() const { return maxMana; }
		constexpr size_t Money() const { return money; }
		constexpr size_t Experience() const { return experience; }
		constexpr size_t NextLevelExperience() const { return nextLevelExperience; }
		constexpr size_t PointsLefToUse() const { return levelPoints; }
		constexpr const Stats& Statistics() const { return playerStatistics; }
		constexpr size_t Level() const { return level; }
		constexpr ItemMap& Items() { return items; }
		inline const SDL_Rect& Position() const override { return position; }
		inline Renders::Texture& Texture() override { return textures.at(0); }

		constexpr void Heal(size_t healthHealed);
		constexpr void RegenerateMana(size_t regeneratedMana);
		constexpr bool UseMana(size_t requestedMana);
		constexpr bool SpendMoney(size_t requestedMoney);

		std::pair<Items::Weapon*, std::array<Items::Armor*, 7>> Equipment();
		void Render() override;
		bool AddExperience(size_t earnedExperience);
		void LevelUp(const Stats& additionalStatistics);
		bool Wear(const std::string& name, short ringNumber = 2);
		bool TakeOff(BodyPart bodyPart, short ringNumber = 2);
		std::unique_ptr<Items::Item> ThrowOut(const std::string& name);
		void GetItem(std::unique_ptr<Items::Item> item);
		void SetPosition(int x, int y) override;
		void ChangePosition(int x, int y) override;
		bool Wound(const LOTSH::Damage& damageTaken);
		bool Use(const std::string& name);

		friend std::ofstream& operator<<(std::ofstream& fout, const Player& player);
		friend std::ifstream& operator>>(std::ifstream& fin, Player& player);
	};

	constexpr void Player::Heal(size_t healthHealed)
	{
		if (healthHealed + health > maxHealth)
			health = maxHealth;
		else
			health += healthHealed;
	}

	constexpr void Player::RegenerateMana(size_t regeneratedMana)
	{
		if (regeneratedMana + mana > maxMana)
			mana = maxMana;
		else
			mana += regeneratedMana;
	}

	constexpr bool Player::UseMana(size_t requestedMana)
	{
		if (requestedMana > mana)
			return false;
		mana -= requestedMana;
		return true;
	}

	constexpr bool Player::SpendMoney(size_t requestedMoney)
	{
		if (requestedMoney > money)
			return false;
		money -= requestedMoney;
		return true;
	}
}