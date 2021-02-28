/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "Texture.h"
#include <vector>

namespace LOTSH::ProgramData
{
	struct TexturesData
	{
		std::vector<Renders::Texture> CodeMenuBackground;
		std::vector<Renders::Texture> MenuBackground;
		Renders::Texture Title;
		Renders::Texture Background;
		Renders::Texture OutFloor;
	};
}