/*
Author: Marek Machliñski
Date:	9.02.2021
*/
#pragma once
#include "IRenderable.h"

namespace LOTSH::Renders::Objects
{
	class IObject : public IRenderable
	{
	protected:
		size_t X = 0;
		size_t Y = 0;
		SDL_Rect collision = { 0,0,0,0 };

	public:
		IObject() = default;
		constexpr IObject(size_t x, size_t y, const SDL_Rect& collision) noexcept : X(x), Y(y), collision(collision) {}
		constexpr IObject(const IObject& o) noexcept : X(o.X), Y(o.Y), collision(o.collision) {}
		constexpr IObject(IObject&& o) noexcept : X(o.X), Y(o.Y), collision(std::move(o.collision)) {}
		virtual ~IObject() = default;

		constexpr const SDL_Rect& Collision() const { return collision; }
		constexpr size_t AbsoluteX() const { return X; }
		constexpr size_t AbsoluteY() const { return Y; }
		constexpr void SetAbsolutePosition(size_t x, size_t y) { X = x; Y = y; }
		constexpr void ChangeAbsolutePosition(int x, int y) { X += x; Y += y; }

		virtual void Render() = 0;
	};
}