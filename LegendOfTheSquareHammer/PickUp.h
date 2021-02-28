/*
Author: Marek Machliñski
Date:	9.02.2021
*/
#pragma once
#include "Player.h"

namespace LOTSH::Renders::Objects
{
	class PickUp : public IObject
	{
		std::unique_ptr<Items::Item> item = nullptr;

	public:
		PickUp() = default;
		inline PickUp(size_t absoluteX, size_t absoluteY, std::unique_ptr<Items::Item> loot)
			: IObject(absoluteX, absoluteY, loot->Position()), item(std::move(loot)) {}
		inline PickUp(PickUp&& p) noexcept : IObject(std::forward<IObject&&>(p)), item(std::move(p.item)) {}
		PickUp(const PickUp& p) noexcept;
		virtual ~PickUp() = default;

		constexpr const std::string& Name() const { return item->Name(); }
		inline Items::Item& GetItem() { return *item; }
		inline std::unique_ptr<Items::Item> DropItem() { return std::move(item); }
		inline const SDL_Rect& Position() const override { return item->Position(); }
		inline Renders::Texture& Texture() override { return item->Texture(); }
		inline void Render() override { item->Texture().Render(item->Position()); }

		void SetPosition(int x, int y) override;
		void ChangePosition(int x, int y) override;

		// Player pick ups item
		void operator()(Player& player);
		friend std::ofstream& operator<<(std::ofstream& fout, const PickUp& pickUp);
		friend std::ifstream& operator>>(std::ifstream& fin, PickUp& pickUp);
	};
}