/*
Author: Marek Machliñski
Date:	9.02.2021
*/
#pragma once
#include "Texture.h"

namespace LOTSH::Renders
{
	class IRenderable
	{
	public:
		IRenderable() = default;
		virtual ~IRenderable() = default;

		virtual const SDL_Rect& Position() const = 0;
		virtual Renders::Texture& Texture() = 0;
		virtual void SetPosition(int x, int y) = 0;
		virtual void ChangePosition(int x, int y) = 0;
	};
}