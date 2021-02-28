/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "IRenderable.h"

namespace LOTSH::Renders
{
	// Button on screen
	class Label : public IRenderable
	{
		SDL_Rect position = { 0,0,0,0 };
		Renders::Texture texture;
		std::string path = "none";
		std::string function = "none";

	public:
		Label() = default;
		Label(const SDL_Rect& newPosition, const std::string& newPath, const std::string& callFunction = "none");
		virtual ~Label() = default;

		constexpr bool IsMouseOn(int x, int y) const { return x >= position.x && x <= position.x + position.w && y >= position.y && y <= position.y + position.h; }
		inline void Render() { texture.Render(position); }

		inline const SDL_Rect& Position() const override { return position; }
		inline Renders::Texture& Texture() override { return texture; }
		inline void SetPosition(int x, int y) override { position.x = x; position.y = y; }
		inline void ChangePosition(int x, int y) override { position.x += x; position.y += y; }

		// Function after press
		void operator()();
		friend std::ofstream& operator<<(std::ofstream& fout, const Label& label);
		friend std::ifstream& operator>>(std::ifstream& fin, Label& label);
	};
}