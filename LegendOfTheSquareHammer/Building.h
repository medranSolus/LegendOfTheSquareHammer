/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "IObject.h"
#include "Structs.h"

namespace LOTSH::Renders::Objects
{
	class Building : public IObject
	{
		SDL_Rect position;
		Renders::Texture texture;
		SDL_Rect entrance;
		size_t colliderDown = 0;
		size_t colliderRight = 0;
		size_t entranceDown = 0;
		size_t entranceRight = 0;
		std::string buildingName = "none";

	public:
		Building() = default;
		Building(double scale, size_t absoluteX, size_t absoluteY, int x, int y,
			const SDL_Rect& collider, const SDL_Rect& entrancePosition,
			const std::string& path, const std::string& newBuildingName);
		inline Building(Building&& b) noexcept
			: IObject(std::forward<IObject&&>(b)), position(b.position),
			texture(std::move(b.texture)), entrance(b.entrance),
			colliderDown(b.colliderDown), colliderRight(b.colliderRight),
			entranceDown(b.entranceDown), entranceRight(b.entranceRight),
			buildingName(b.buildingName) {}
		virtual ~Building() = default;

		constexpr const std::string& Name() const { return buildingName; }
		inline const SDL_Rect& Position() const override { return position; }
		inline Renders::Texture& Texture() override { return texture; }
		inline void Render() override { texture.Render(position); }

		void SetPosition(int x, int y) override;
		void ChangePosition(int x, int y) override;
		CollisionEntrance CollisionDirection(const IObject& object);

		friend std::ofstream& operator<<(std::ofstream& fout, const Building& building);
		friend std::ifstream& operator>>(std::ifstream& fin, Building& building);
	};
}