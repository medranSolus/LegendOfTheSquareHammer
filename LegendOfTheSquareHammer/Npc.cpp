/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#include "stdafx.h"
#include "Npc.h"

namespace LOTSH::Renders::Objects
{
	Npc::Npc(double scale, size_t absoluteX, size_t absoluteY, int x, int y,
		size_t playerExperience, size_t newHealth, size_t deathSpawnTime,
		const SDL_Rect& collider, const Damage& newDamage, const Damage& newResistance,
		const std::string& path, std::unique_ptr<PickUp> droppedItem)
		: IObject(absoluteX, absoluteY, { x + static_cast<int>(scale * collider.x), y + static_cast<int>(scale * collider.y),
			static_cast<int>(scale * collider.w), static_cast<int>(scale * collider.h) }),
		experience(playerExperience), health(newHealth), maxHealth(newHealth),
		spawnTime(deathSpawnTime), damage(newDamage), resistance(newResistance), item(std::move(droppedItem))
	{
		if (!texture.LoadFile(path))
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << ": Building\n";
			fout.close();
			exit(0b1000000000000000);
		}
		position = { x, y, static_cast<int>(scale * texture.Width()), static_cast<int>(scale * texture.Height()) };
		colliderDown = position.h - collision.h - (position.y - collision.y);
		colliderRight = position.w - collision.w - (position.x - collision.x);
	}

	Npc::Npc(Npc&& n) noexcept
		: IObject(std::forward<IObject&&>(n)), position(std::move(n.position)),
		texture(std::move(n.texture)), health(n.health), maxHealth(n.maxHealth),
		colliderRight(n.colliderRight), currentDirection(n.currentDirection),
		spawnTime(n.spawnTime), woundTime(n.woundTime), moveTime(n.moveTime),
		attackTime(n.attackTime), experience(n.experience), active(n.active),
		resistance(std::move(n.resistance)), damage(std::move(n.damage)), item(std::move(n.item))
	{
	}

	void Npc::SetPosition(int x, int y)
	{
		position.x = x;
		collision.x = static_cast<int>(position.x + position.w - collision.w - colliderRight);
		position.y = y;
		collision.y = static_cast<int>(position.y + position.h - collision.h - colliderDown);
	}

	void Npc::ChangePosition(int x, int y)
	{
		position.x += x;
		collision.x += x;
		position.y += y;
		collision.y += y;
	}

	short Npc::CollisionDirection(const IObject& object) const
	{
		int objectWidth = object.Collision().x + object.Collision().w;
		int objectHeight = object.Collision().y + object.Collision().h;
		int thisWidth = collision.x + collision.w;
		int thisHeight = collision.y + collision.h;
		if (objectWidth < collision.x || object.Collision().x > thisWidth || objectHeight < collision.y || object.Collision().y > thisHeight)
			return Direction::None;
		if (objectWidth >= collision.x && objectWidth < thisWidth && object.Collision().x < collision.x)
		{
			if (objectHeight >= collision.y && objectHeight < thisHeight && object.Collision().y < collision.y)
				return Direction::S | Direction::E;
			if (object.Collision().y <= thisHeight && object.Collision().y > collision.y && objectHeight > thisHeight)
				return Direction::N | Direction::E;
			return Direction::E;
		}
		if (object.Collision().x <= thisWidth && object.Collision().x > collision.x && objectWidth > thisWidth)
		{
			if (objectHeight >= collision.y && objectHeight < thisHeight && object.Collision().y < collision.y)
				return Direction::S | Direction::W;
			if (object.Collision().y <= thisHeight && object.Collision().y > collision.y && objectHeight > thisHeight)
				return Direction::N | Direction::W;
			return Direction::W;
		}
		if (object.Collision().x >= collision.x && objectWidth <= thisWidth)
		{
			if (objectHeight >= collision.y && objectHeight < thisHeight && object.Collision().y < collision.y)
				return Direction::S;
			if (object.Collision().y <= thisHeight && object.Collision().y > collision.y && objectHeight > thisHeight)
				return Direction::N;
		}
		return Direction::None;
	}

	void Npc::Move(Player& player, int fieldWidth, int fieldHeight, short collisionDirection)
	{
		if (active)
		{
			size_t distance = Distance(player, *this);
			bool playerFound = false;
			if (woundTime)
				--woundTime;
			else
				texture.SetColor(255, 255, 255);
			if (moveTime == 0)
			{
				moveTime = rand() % 31 * 10 + 50;
				currentDirection = static_cast<Direction>(rand() % 16);
			}
			if (distance <= 600)
			{
				playerFound = true;
				int playerWidth = player.Collision().x + player.Collision().w;
				int playerHeight = player.Collision().y + player.Collision().h;
				int npcWidth = collision.x + collision.w;
				int npcHeight = collision.y + collision.h;
				if (player.Collision().x > npcWidth)
				{
					if (player.Collision().y > npcHeight)
						currentDirection = Direction::S | Direction::E;
					else if (playerHeight < collision.y)
						currentDirection = Direction::N | Direction::E;
					else
						currentDirection = Direction::E;
				}
				else if (playerWidth < collision.x)
				{
					if (player.Collision().y > npcHeight)
						currentDirection = Direction::S | Direction::W;
					else if (playerHeight < collision.y)
						currentDirection = Direction::N | Direction::W;
					else
						currentDirection = Direction::W;
				}
				else if (player.Collision().x >= collision.x && playerWidth <= npcWidth)
				{
					if (player.Collision().y > npcHeight)
						currentDirection = Direction::S;
					else if (playerHeight < collision.y)
						currentDirection = Direction::N;
				}

				if (attackTime)
					--attackTime;
				if (!attackTime && distance <= 60)
				{
					player.Wound(damage);
					currentDirection = Direction::None;
					moveTime = 10;
					attackTime = 200;
				}
			}
			else
				attackTime = 1;

			--moveTime;
			if (currentDirection != collisionDirection && currentDirection != Direction::None && (moveTime % 5 == 0 || (playerFound && moveTime % 2 == 0)))
			{
				if (currentDirection & Direction::N)
				{
					if (Y > 4)
					{
						Y -= 5;
						position.y -= 5;
						collision.y -= 5;
					}
					else
					{
						collision.y -= static_cast<int>(Y);
						position.y -= static_cast<int>(Y);
						Y = 0;
					}
				}
				else if (currentDirection & Direction::S)
				{
					if (static_cast<int>(Y) < fieldHeight - position.h - 1)
					{
						Y += 5;
						position.y += 5;
						collision.y += 5;
					}
					else
					{
						collision.y += static_cast<int>(Y - fieldHeight + collision.h);
						position.y += static_cast<int>(Y - fieldHeight + position.h);
						Y = fieldHeight - static_cast<size_t>(position.h);
					}
				}
				if (currentDirection & Direction::E)
				{
					if (static_cast<int>(X) < fieldWidth - position.w - 1)
					{
						X += 5;
						position.x += 5;
						collision.x += 5;
					}
					else
					{
						collision.x += static_cast<int>(X - fieldWidth + collision.w);
						position.x += static_cast<int>(X - fieldWidth + position.w);
						X = fieldWidth - static_cast<size_t>(position.w);
					}
				}
				else if (currentDirection & Direction::W)
				{
					if (X > 4)
					{
						X -= 5;
						position.x -= 5;
						collision.x -= 5;
					}
					else
					{
						collision.x -= static_cast<int>(X);
						position.x -= static_cast<int>(X);
						X = 0;
					}
				}
			}
		}
	}

	std::unique_ptr<PickUp> Npc::Wound(const Damage& damageTaken)
	{
		size_t previousHealth = health;
		size_t dmg = damageTaken.Normal > resistance.Normal ? damageTaken.Normal - resistance.Normal : 0;
		if (dmg < health)
		{
			dmg += damageTaken.Darkness * (100 - resistance.Darkness) / 100
				+ damageTaken.Fire * (100 - resistance.Fire) / 100
				+ damageTaken.Ice * (100 - resistance.Ice) / 100
				+ damageTaken.Magic * (100 - resistance.Magic) / 100;
		}
		if (dmg >= health)
		{
			health = 0;
			if (item != nullptr)
			{
				item->SetAbsolutePosition(X, Y);
				item->SetPosition(position.x, position.y);
			}
			active = false;
			spawnTime = 1000;
			return std::move(item);
		}
		health -= dmg;
		if (health < previousHealth)
		{
			woundTime = 50;
			texture.SetColor(255, 0, 0);
		}
		return nullptr;
	}

	std::ofstream& operator<<(std::ofstream& fout, const Npc& npc)
	{
		fout << npc.X << ' '
			<< npc.Y << ' '
			<< npc.spawnTime << ' '
			<< npc.moveTime << ' '
			<< npc.active << ' '
			<< npc.experience << ' '
			<< npc.health << ' '
			<< npc.maxHealth << ' '
			<< npc.currentDirection << ' '
			<< npc.colliderDown << ' '
			<< npc.colliderRight << std::endl;
		fout << npc.position << std::endl;
		fout << npc.collision << std::endl;
		fout << npc.damage << std::endl;
		fout << npc.resistance << std::endl;
		fout << npc.texture << std::endl;
		if (npc.item != nullptr)
		{
			fout << 1 << std::endl;
			fout << *npc.item;
		}
		else
			fout << "0\n";
		return fout;
	}

	std::ifstream& operator>>(std::ifstream& fin, Npc& npc)
	{
		fin >> npc.X >> npc.Y >> npc.spawnTime >> npc.moveTime
			>> npc.active >> npc.experience >> npc.health
			>> npc.maxHealth >> npc.currentDirection
			>> npc.colliderDown >> npc.colliderRight;
		fin >> npc.position >> npc.collision >> npc.damage
			>> npc.resistance >> npc.texture;
		int isLoot;
		fin >> isLoot;
		if (isLoot)
		{
			npc.item = std::make_unique<PickUp>();
			fin >> *npc.item;
		}
		return fin;
	}
}