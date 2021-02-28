/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "PickUp.h"

namespace LOTSH::Renders::Objects
{
	extern const size_t FIELD_WIDTH;
	extern const size_t FIELD_HEIGHT;

	class Npc : public IObject
	{
		SDL_Rect position;
		Renders::Texture texture;
		size_t health = 0;
		size_t maxHealth = 0;
		size_t colliderRight = 0;
		size_t colliderDown = 0;
		short currentDirection = Direction::None;
		size_t spawnTime = 0;
		size_t woundTime = 0;
		size_t moveTime = 0;
		size_t attackTime = 0;
		size_t experience = 0;
		bool active = true;
		Damage resistance;
		Damage damage;
		std::unique_ptr<PickUp> item = nullptr;

	public:
		Npc() = default;
		Npc(double scale, size_t absoluteX, size_t absoluteY, int x, int y,
			size_t playerExperience, size_t newHealth, size_t deathSpawnTime,
			const SDL_Rect& collider, const Damage& newDamage, const Damage& newResistance,
			const std::string& path, std::unique_ptr<PickUp> droppedItem = nullptr);
		Npc(const Npc& n) noexcept;
		Npc(Npc&& n) noexcept;
		virtual ~Npc() = default;

		constexpr bool IsActive() const { return active; }
		constexpr size_t Experience() const { return experience; }

		inline const SDL_Rect& Position() const override { return position; }
		inline Renders::Texture& Texture() override { return texture; }
		inline void Render() override { texture.Render(position); }

		// Time counter till re-appear of enemy
		constexpr void DeathSpawn();
		void SetPosition(int x, int y) override;
		void ChangePosition(int x, int y) override;
		short CollisionDirection(const IObject& object) const;
		// Move in random direction or after player if close enough
		void Move(Player& player, int fieldWidth = FIELD_WIDTH, int fieldHeight = FIELD_HEIGHT, short collisionDirection = Direction::None);
		// Calculates taken damage and return item if dead
		std::unique_ptr<PickUp> Wound(const Damage& damageTaken);

		friend std::ofstream& operator<<(std::ofstream& fout, const Npc& npc);
		friend std::ifstream& operator>>(std::ifstream& fin, Npc& npc);
	};

	constexpr void Npc::DeathSpawn()
	{
		if (spawnTime)
			--spawnTime;
		else
		{
			active = true;
			health = maxHealth;
		}
	}

}