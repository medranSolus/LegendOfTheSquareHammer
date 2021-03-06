/*
Author: Marek Machliński
Date:	20.04.2018
*/
#include "stdafx.h"

namespace LOTSH
{
	size_t Distance(const Renders::Objects::IObject& object1, const Renders::Objects::IObject& object2)
	{
		const int object1Width = object1.Collision().x + object1.Collision().w;
		const int object1Height = object1.Collision().y + object1.Collision().h;
		const int object2Width = object2.Collision().x + object2.Collision().w;
		const int object2Height = object2.Collision().y + object2.Collision().h;

		if (object1.Collision().x < object2.Collision().x)
		{
			if (object1.Collision().y < object2.Collision().y)
				return size_t(sqrt(pow(abs(object2.Collision().x - object1Width), 2) + pow(abs(object2.Collision().y - object1Height), 2)));
			if (object1Height > object2Height)
				return size_t(sqrt(pow(abs(object2.Collision().x - object1Width), 2) + pow(abs(object1.Collision().y - object2Height), 2)));
			return size_t(abs(object2.Collision().x - object1Width));
		}
		if (object1Width > object2Width)
		{
			if (object1.Collision().y < object2.Collision().y)
				return size_t(sqrt(pow(abs(object1.Collision().x - object2Width), 2) + pow(abs(object2.Collision().y - object1Height), 2)));
			if (object1Height > object2Height)
				return size_t(sqrt(pow(abs(object1.Collision().x - object2Width), 2) + pow(abs(object1.Collision().y - object2Height), 2)));
			return size_t(abs(object1.Collision().x - object2Width));
		}
		if (object1.Collision().x >= object2.Collision().x && object1Width <= object2Width)
		{
			if (object1.Collision().y < object2.Collision().y)
				return size_t(abs(object1Height - object2.Collision().y));
			if (object1Height > object2Height)
				return size_t(abs(object1.Collision().y - object2Height));
		}
		return size_t(sqrt(pow(abs((object1.Collision().x + object1.Collision().w / 2) - (object2.Collision().x + object2.Collision().w / 2)), 2) + pow(abs((object1.Collision().y + object1.Collision().h / 2) - (object2.Collision().y + object2.Collision().h / 2)), 2)));
	}

	std::ofstream& operator<<(std::ofstream& fout, const SDL_Rect& rectangle)
	{
		fout << rectangle.x << ' ' << rectangle.y << ' ' << rectangle.w << ' ' << rectangle.h;
		return fout;
	}

	std::ifstream& operator>>(std::ifstream& fin, SDL_Rect& rectangle)
	{
		fin >> rectangle.x >> rectangle.y >> rectangle.w >> rectangle.h;
		return fin;
	}
}