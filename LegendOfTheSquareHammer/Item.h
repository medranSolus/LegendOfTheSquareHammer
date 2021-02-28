/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "IRenderable.h"
#include "Structs.h"

namespace LOTSH::Renders::Items
{
	class Item : public IRenderable
	{
	protected:
		std::string name = "item";
		std::string description = "none";
		size_t minimumLevel = 0;
		SDL_Rect position = { 0,0,100,100 };
		Renders::Texture texture;

	public:
		Item() = default;
		Item(const std::string& newName, const std::string& newDescription,
			size_t levelRequired, const std::string& path, int x, int y);
		virtual ~Item() = default;

		constexpr const std::string& Name() const { return name; }
		constexpr const std::string& Description() const { return description; }
		constexpr size_t MinimumLevel() const { return minimumLevel; }

		inline const SDL_Rect& Position() const override { return position; }
		inline Renders::Texture& Texture() override { return texture; }
		inline void SetPosition(int x, int y) override { position.x = x; position.y = y; }
		inline void ChangePosition(int x, int y) override { position.x += x; position.y += y; }

		virtual ItemType ItemType() const = 0;
		virtual std::ofstream& Write(std::ofstream& fout) const = 0;
		virtual std::ifstream& Read(std::ifstream& fin) = 0;
	};
}