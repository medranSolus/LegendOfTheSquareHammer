/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#include "stdafx.h"
#include "Building.h"
#include "Player.h"

namespace LOTSH::Renders::Objects
{
	Building::Building(double scale, size_t absoluteX, size_t absoluteY, int x, int y,
		const SDL_Rect& collider, const SDL_Rect& entrancePosition,
		const std::string& path, const std::string& newBuildingName)
		: IObject(absoluteX, absoluteY,
			{ x + static_cast<int>(collider.x * scale), y + static_cast<int>(collider.y * scale),
			static_cast<int>(collider.w * scale), static_cast<int>(collider.h * scale) }),
		buildingName(newBuildingName)
	{
		if (!texture.LoadFile(path))
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << ": Building\n";
			fout.close();
			exit(0b100000000000000);
		}
		position = { x, y, static_cast<int>(texture.Width() * scale), static_cast<int>(texture.Height() * scale) };
		entrance = { position.x + static_cast<int>(entrancePosition.x * scale), position.y + static_cast<int>(entrancePosition.y * scale),
			static_cast<int>(entrancePosition.w * scale), static_cast<int>(entrancePosition.h * scale) };
		colliderDown = position.h - collision.h - (position.y - collision.y);
		entranceDown = position.h - entrance.h - (position.y - entrance.y);
		colliderRight = position.w - collision.w - (position.x - collision.x);
		entranceRight = position.w - entrance.w - (position.x - entrance.x);
	}

	void Building::SetPosition(int x, int y)
	{
		position.x = x;
		collision.x = static_cast<int>(position.x + position.w - collision.w - colliderRight);
		entrance.x = static_cast<int>(position.x + position.w - entrance.w - entranceRight);
		position.y = y;
		collision.y = static_cast<int>(position.y + position.h - collision.h - colliderDown);
		entrance.y = static_cast<int>(position.y + position.h - entrance.h - entranceDown);
	}

	void Building::ChangePosition(int x, int y)
	{
		position.x += x;
		collision.x += x;
		entrance.x += x;
		position.y += y;
		collision.y += y;
		entrance.y += y;
	}

	CollisionEntrance Building::CollisionDirection(const IObject& object)
	{
		int objectWidth = object.Collision().x + object.Collision().w;
		int objectHeight = object.Collision().y + object.Collision().h;
		int thisWidth = collision.x + collision.w;
		int thisHeight = collision.y + collision.h;
		int entranceWidth = entrance.x + entrance.w;
		int entranceHeight = entrance.y + entrance.h;
		if (objectWidth < collision.x || object.Collision().x > thisWidth || objectHeight < collision.y || object.Collision().y > thisHeight)
			return { Direction::None, nullptr };
		if (dynamic_cast<const Player*>(&object) != nullptr)
		{
			if ((object.Collision().x >= entrance.x && objectWidth <= entranceWidth
					&& ((object.Collision().y <= entranceHeight && object.Collision().y > entrance.y) || (object.Collision().y < entrance.y && objectHeight >= entrance.y)))
				|| (object.Collision().y >= entrance.y && objectHeight <= entranceHeight
					&& ((object.Collision().x < entrance.x && objectWidth >= entrance.x) || (object.Collision().x <= entranceWidth && objectWidth > entranceWidth))))
				return { Direction::Enter, this };
		}
		if (objectWidth >= collision.x && objectWidth < thisWidth && object.Collision().x < collision.x)
		{
			if (objectHeight >= collision.y && objectHeight < thisHeight && object.Collision().y < collision.y)
				return { Direction::S | Direction::E, nullptr };
			if (object.Collision().y <= thisHeight && object.Collision().y > collision.y && objectHeight > thisHeight)
				return { Direction::N | Direction::E, nullptr };
			return { Direction::E, nullptr };
		}
		if (object.Collision().x <= thisWidth && object.Collision().x > collision.x && objectWidth > thisWidth)
		{
			if (objectHeight >= collision.y && objectHeight < thisHeight && object.Collision().y < collision.y)
				return { Direction::S | Direction::W, nullptr };
			if (object.Collision().y <= thisHeight && object.Collision().y > collision.y && objectHeight > thisHeight)
				return { Direction::N | Direction::W, nullptr };
			return { Direction::W, nullptr };
		}
		if (object.Collision().x >= collision.x && objectWidth <= thisWidth)
		{
			if (objectHeight >= collision.y && objectHeight < thisHeight && object.Collision().y < collision.y)
				return { Direction::S, nullptr };
			if (object.Collision().y <= thisHeight && object.Collision().y > collision.y && objectHeight > thisHeight)
				return { Direction::N, nullptr };
		}
		return { Direction::None, nullptr };
	}

	std::ofstream& operator<<(std::ofstream& fout, const Building& building)
	{
		fout << building.X << ' '
			<< building.Y << ' '
			<< building.colliderDown << ' '
			<< building.colliderRight << ' '
			<< building.entranceDown << ' '
			<< building.entranceRight << std::endl
			<< building.buildingName << std::endl;
		fout << building.position << std::endl;
		fout << building.collision << std::endl;
		fout << building.entrance << std::endl;
		fout << building.texture << std::endl;
		return fout;
	}

	std::ifstream& operator>>(std::ifstream& fin, Building& building)
	{
		fin >> building.X >> building.Y >> building.colliderDown >> building.colliderRight >> building.entranceDown >> building.entranceRight;
		if (fin.peek() == 10)
			fin.ignore();
		getline(fin, building.buildingName);
		fin >> building.position >> building.collision >> building.entrance;
		fin >> building.texture;
		return fin;
	}
}