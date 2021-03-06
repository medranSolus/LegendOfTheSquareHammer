/*
Author: Marek Machliński
Date:	20.04.2018
*/
#pragma once
#include <SDKDDKVer.h>
#include "IObject.h"

namespace LOTSH
{
	size_t Distance(const Renders::Objects::IObject& object1, const Renders::Objects::IObject& object2);

	std::ofstream& operator<<(std::ofstream& fout, const SDL_Rect& rectangle);
	std::ifstream& operator>>(std::ifstream& fin, SDL_Rect& rectangle);
}
using LOTSH::operator<<;
using LOTSH::operator>>;