/*
Author: Marek Machliñski
Date:	27.02.2021
*/
#include "stdafx.h"
#include "Game.h"
#include "EventHandler.h"
#include "Info.h"

namespace LOTSH
{
	extern void LoadGame();

	Game::Game(std::unique_ptr<Renders::Objects::Player> newPlayer) : gameSave(std::move(newPlayer))
	{
		gameSave.SaveGame("main");
		if (ProgramData::Data::KonamiCode())
			movement = 9;
	}

	Game::Game(const std::string& path, std::string& startingPlace) : gameSave(path, startingPlace)
	{
		if (ProgramData::Data::KonamiCode())
			movement = 9;
	}

	short Game::CheckCollision(const Renders::Objects::IObject& object, const NpcMap& monsters)
	{
		short collisionDirection = Direction::None;
		for (auto& monster : monsters)
			if (Distance(object, monster.second) <= 20)
				collisionDirection |= monster.second.CollisionDirection(object);
		return collisionDirection;
	}

	CollisionEntrance Game::CheckCollision(const Renders::Objects::IObject& object, BuildingsMap& buildings)
	{
		CollisionEntrance collision;
		for (auto& building : buildings)
			if (Distance(object, building.second) <= 50)
			{
				auto col = building.second.CollisionDirection(object);
				collision.Direction |= col.Direction;
				collision.Building = col.Building;
			}
		return collision;
	}

	constexpr bool Game::IsInsideScreen(const SDL_Rect& pos)
	{
		const int farX = pos.x + pos.w;
		const int farY = pos.y + pos.h;
		return ((pos.x < static_cast<int>(ProgramData::Data::SCREEN_WIDTH) && pos.x >= 0)
				|| (farX >= 0 && farX < static_cast<int>(ProgramData::Data::SCREEN_WIDTH))
				|| (pos.x < 0 && farX >= static_cast<int>(ProgramData::Data::SCREEN_WIDTH)))
			&& ((pos.y < static_cast<int>(ProgramData::Data::SCREEN_HEIGHT) && pos.y >= 0)
				|| (farY >= 0 && farY < static_cast<int>(ProgramData::Data::SCREEN_HEIGHT))
				|| (pos.y < 0 && farY >= static_cast<int>(ProgramData::Data::SCREEN_HEIGHT)));
	}

	void Game::Attack(int x, int y, const std::string& place)
	{
		short attackDirection;
		if (x < gameSave.Player().Collision().x)
		{
			attackDirection = Direction::W;
			if (y < gameSave.Player().Collision().y)
				attackDirection |= Direction::N;
			else if (y > gameSave.Player().Collision().y + gameSave.Player().Collision().h)
				attackDirection |= Direction::S;
		}
		else if (x > gameSave.Player().Collision().x + gameSave.Player().Collision().w)
		{
			attackDirection = Direction::E;
			if (y < gameSave.Player().Collision().y)
				attackDirection |= Direction::N;
			else if (y > gameSave.Player().Collision().y + gameSave.Player().Collision().h)
				attackDirection |= Direction::S;
		}
		else
			attackDirection = y < gameSave.Player().Collision().y ? Direction::N : Direction::S;

		NpcMap* monsters = gameSave.Monsters(place);
		if (monsters != nullptr)
		{
			for (auto& x : *monsters)
			{
				const int dX = x.second.Collision().x - gameSave.Player().Collision().x + (x.second.Collision().w - gameSave.Player().Collision().w) / 2;
				const int dY = x.second.Collision().y - gameSave.Player().Collision().y + (x.second.Collision().h - gameSave.Player().Collision().h) / 2;
				if (x.second.IsActive() && 300 >= sqrt(pow(dX, 2) + pow(dY, 2)))
				{
					bool isDamage = false;
					const int monsterFarX = x.second.Collision().x + x.second.Collision().w;
					const int monsterFarY = x.second.Collision().y + x.second.Collision().h;
					const int playerFarX = gameSave.Player().Collision().x + gameSave.Player().Collision().w;
					const int playerFarY = gameSave.Player().Collision().y + gameSave.Player().Collision().h;

					if (attackDirection & Direction::N)
					{
						if (attackDirection & Direction::E)
							isDamage = x.second.Collision().y <= gameSave.Player().Collision().y && monsterFarX >= playerFarX;
						else if (attackDirection & Direction::W)
							isDamage = x.second.Collision().y <= gameSave.Player().Collision().y && x.second.Collision().x <= gameSave.Player().Collision().x;
						else
							isDamage = ((x.second.Collision().x >= gameSave.Player().Collision().x && x.second.Collision().x <= playerFarX)
											|| (monsterFarX >= gameSave.Player().Collision().x && monsterFarX <= playerFarX)
											|| (x.second.Collision().x <= gameSave.Player().Collision().x && monsterFarX >= playerFarX)) 
										&& monsterFarY <= gameSave.Player().Collision().y;
					}
					else if (attackDirection & Direction::S)
					{
						if (attackDirection & Direction::E)
							isDamage = x.second.Collision().y >= playerFarY && monsterFarX >= playerFarX;
						else if (attackDirection & Direction::W)
							isDamage = x.second.Collision().y >= playerFarY && x.second.Collision().x <= gameSave.Player().Collision().x;
						else
							isDamage = ((x.second.Collision().x >= gameSave.Player().Collision().x && x.second.Collision().x <= playerFarX)
											|| (monsterFarX >= gameSave.Player().Collision().x && monsterFarX <= playerFarX)
											|| (x.second.Collision().x <= gameSave.Player().Collision().x && monsterFarX >= playerFarX))
										&& x.second.Collision().y >= playerFarY;
					}
					else if (attackDirection & Direction::E)
					{
						isDamage = ((x.second.Collision().y >= gameSave.Player().Collision().y && x.second.Collision().y <= playerFarY)
										|| (monsterFarY >= gameSave.Player().Collision().y && monsterFarY <= playerFarY)
										|| (x.second.Collision().y <= gameSave.Player().Collision().y && monsterFarY >= playerFarY))
									&& x.second.Collision().x >= playerFarX;
					}
					else if (attackDirection & Direction::W)
					{
						isDamage = ((x.second.Collision().y >= gameSave.Player().Collision().y && x.second.Collision().y <= playerFarY)
										|| (monsterFarY >= gameSave.Player().Collision().y && monsterFarY <= playerFarY)
										|| (x.second.Collision().y <= gameSave.Player().Collision().y && monsterFarY >= playerFarY))
									&& monsterFarX <= gameSave.Player().Collision().x;
					}

					if (isDamage)
					{
						std::unique_ptr<Renders::Objects::PickUp> loot = x.second.Wound(gameSave.Player().Damage());
						if (loot != nullptr)
						{
							if (gameSave.Player().AddExperience(x.second.Experience()))
								isNewLevel = true;
							PickUpMap* pickUps = gameSave.Items(place);
							if (pickUps != nullptr)
							{
								std::string name = loot->Name();
								pickUps->emplace(std::move(name), std::move(*loot.release()));
							}
						}
					}
				}
			}
		}
	}

	void Game::RenderOrder(const std::string& place)
	{
		std::vector<Renders::Objects::IObject*> toRender;
		toRender.emplace_back(&gameSave.Player());

		BuildingsMap* buildings = gameSave.Buildings(place);
		if (buildings != nullptr)
			for (auto& x : *buildings)
				if (IsInsideScreen(x.second.Position()))
					toRender.emplace_back(&x.second);

		NpcMap* monsters = gameSave.Monsters(place);
		if (monsters != nullptr)
			for (auto& x : *monsters)
				if (x.second.IsActive() && IsInsideScreen(x.second.Position()))
					toRender.emplace_back(&x.second);

		PickUpMap* items = gameSave.Items(place);
		if (items != nullptr)
			for (auto& x : *items)
				if (IsInsideScreen(x.second.Position()))
					toRender.emplace_back(&x.second);

		sort(toRender.begin(), toRender.end(), [](Renders::Objects::IObject* x, Renders::Objects::IObject* y) { return x->Collision().y < y->Collision().y; });
		for (auto& x : toRender)
			x->Render();
		auto& hud = ProgramData::Data::GetLabels().Mini.at(1);
		hud.Texture().LoadText("Health:  " + std::to_string(gameSave.Player().Health()), { 255,0,0,255 });
		hud.Render();
		if (isNewLevel)
			ProgramData::Data::GetLabels().Mini.at(2).Render();
	}

	void Game::MoveObjects(const std::string& place, CollisionEntrance& collision, int fieldWidth, int fieldHeight, int* x, int* y)
	{
		auto& player = gameSave.Player();
		auto& keyboard = Events::EventHandler::GetKeyboard();
		BuildingsMap* buildings = gameSave.Buildings(place);
		NpcMap* monsters = gameSave.Monsters(place);
		PickUpMap* items = gameSave.Items(place);

		if (keyboard.GetKeyState(SDLK_w) && player.Position().y > 0 && !(collision.Direction & Direction::N))
		{
			if (fieldHeight != ProgramData::Data::SCREEN_HEIGHT)
			{
				if (player.AbsoluteY() <= ProgramData::Data::CenterY())
				{
					if (player.Position().y - movement < 0)
					{
						player.SetAbsolutePosition(player.AbsoluteX(), 0);
						player.SetPosition(player.Position().x, 0);
						if (buildings != nullptr)
							for (auto& x : *buildings)
								x.second.ChangePosition(0, movement - player.Position().y);
						if (monsters != nullptr)
							for (auto& x : *monsters)
								if (x.second.IsActive())
									x.second.ChangePosition(0, movement - player.Position().y);
						if (items != nullptr)
							for (auto& x : *items)
								x.second.ChangePosition(0, movement - player.Position().y);
					}
					else
					{
						player.ChangeAbsolutePosition(0, -1 * movement);
						player.ChangePosition(0, -1 * movement);
					}
				}
				else if (player.AbsoluteY() >= fieldHeight - ProgramData::Data::CenterY())
				{
					player.ChangeAbsolutePosition(0, -1 * movement);
					player.ChangePosition(0, -1 * movement);
				}
				else
				{
					player.SetPosition(player.Position().x, static_cast<int>(ProgramData::Data::CenterY()));
					player.ChangeAbsolutePosition(0, -1 * movement);
					if (buildings != nullptr)
						for (auto& x : *buildings)
							x.second.ChangePosition(0, movement);
					if (monsters != nullptr)
						for (auto& x : *monsters)
							if (x.second.IsActive())
								x.second.ChangePosition(0, movement);
					if (items != nullptr)
						for (auto& x : *items)
							x.second.ChangePosition(0, movement);

					if (y != nullptr)
					{
						if (*y > 68)
							*y -= 68 - movement;
						else
							*y += movement;
					}
				}
			}
			else
			{
				if (player.Position().y - movement < 0)
				{
					player.SetAbsolutePosition(player.AbsoluteX(), 0);
					player.SetPosition(player.Position().x, 0);
				}
				else
				{
					player.ChangeAbsolutePosition(0, -1 * movement);
					player.ChangePosition(0, -1 * movement);
				}
			}
		}
		else if (keyboard.GetKeyState(SDLK_s) && player.Position().y < static_cast<int>(ProgramData::Data::SCREEN_HEIGHT) && !(collision.Direction & S))
		{
			if (fieldHeight != ProgramData::Data::SCREEN_HEIGHT)
			{
				if (player.AbsoluteY() <= ProgramData::Data::CenterY())
				{
					player.ChangeAbsolutePosition(0, movement);
					player.ChangePosition(0, movement);
				}
				else if (player.AbsoluteY() >= fieldHeight - ProgramData::Data::CenterY())
				{
					if (player.Position().y + player.Position().h + movement > static_cast<int>(ProgramData::Data::SCREEN_HEIGHT))
					{
						player.SetAbsolutePosition(player.AbsoluteX(), fieldHeight - player.Position().h);
						player.SetPosition(player.Position().x, ProgramData::Data::SCREEN_HEIGHT - player.Position().h);
						if (buildings != nullptr)
							for (auto& x : *buildings)
								x.second.ChangePosition(0, player.Position().y + player.Position().h - ProgramData::Data::SCREEN_HEIGHT);
						if (monsters != nullptr)
							for (auto& x : *monsters)
								if (x.second.IsActive())
									x.second.ChangePosition(0, player.Position().y + player.Position().h - ProgramData::Data::SCREEN_HEIGHT);
						if (items != nullptr)
							for (auto& x : *items)
								x.second.ChangePosition(0, player.Position().y + player.Position().h - ProgramData::Data::SCREEN_HEIGHT);
					}
					else
					{
						player.ChangeAbsolutePosition(0, movement);
						player.ChangePosition(0, movement);
					}
				}
				else
				{
					player.SetPosition(player.Position().x, static_cast<int>(ProgramData::Data::CenterY()));
					player.ChangeAbsolutePosition(0, movement);
					if (buildings != nullptr)
						for (auto& x : *buildings)
							x.second.ChangePosition(0, -1 * movement);
					if (monsters != nullptr)
						for (auto& x : *monsters)
							if (x.second.IsActive())
								x.second.ChangePosition(0, -1 * movement);
					if (items != nullptr)
						for (auto& x : *items)
							x.second.ChangePosition(0, -1 * movement);

					if (y != nullptr)
					{
						if (*y < -68)
							*y += 68 - movement;
						else
							*y -= movement;
					}
				}
			}
			else
			{
				if (player.Position().y + player.Position().h + movement > static_cast<int>(ProgramData::Data::SCREEN_HEIGHT))
				{
					player.SetAbsolutePosition(player.AbsoluteX(), fieldHeight - player.Position().h);
					player.SetPosition(player.Position().x, ProgramData::Data::SCREEN_HEIGHT - player.Position().h);
				}
				else
				{
					player.ChangeAbsolutePosition(0, movement);
					player.ChangePosition(0, movement);
				}
			}
		}
		if (keyboard.GetKeyState(SDLK_a) && player.Position().x > 0 && !(collision.Direction & W))
		{
			if (fieldWidth != ProgramData::Data::SCREEN_WIDTH)
			{
				if (player.AbsoluteX() <= ProgramData::Data::CenterX())
				{
					if (player.Position().x - movement < 0)
					{
						player.SetAbsolutePosition(0, player.AbsoluteY());
						player.SetPosition(0, player.Position().y);
						if (buildings != nullptr)
							for (auto& x : *buildings)
								x.second.ChangePosition(movement - player.Position().x, 0);
						if (monsters != nullptr)
							for (auto& x : *monsters)
								if (x.second.IsActive())
									x.second.ChangePosition(movement - player.Position().x, 0);
						if (items != nullptr)
							for (auto& x : *items)
								x.second.ChangePosition(movement - player.Position().x, 0);
					}
					else
					{
						player.ChangeAbsolutePosition(-1 * movement, 0);
						player.ChangePosition(-1 * movement, 0);
					}
				}
				else if (player.AbsoluteX() >= fieldWidth - ProgramData::Data::CenterX())
				{
					player.ChangeAbsolutePosition(-1 * movement, 0);
					player.ChangePosition(-1 * movement, 0);
				}
				else
				{
					player.SetPosition(static_cast<int>(ProgramData::Data::CenterX()), player.Position().y);
					player.ChangeAbsolutePosition(-1 * movement, 0);
					if (buildings != nullptr)
						for (auto& x : *buildings)
							x.second.ChangePosition(movement, 0);
					if (monsters != nullptr)
						for (auto& x : *monsters)
							if (x.second.IsActive())
								x.second.ChangePosition(movement, 0);
					if (items != nullptr)
						for (auto& x : *items)
							x.second.ChangePosition(movement, 0);

					if (x != nullptr)
					{
						if (*x > 68)
							*x -= 68 - movement;
						else
							*x += movement;
					}
				}
			}
			else
			{
				if (player.Position().x - movement < 0)
				{
					player.SetAbsolutePosition(0, player.AbsoluteY());
					player.SetPosition(0, player.Position().y);
				}
				else
				{
					player.ChangeAbsolutePosition(-1 * movement, 0);
					player.ChangePosition(-1 * movement, 0);
				}
			}
			player.SetTextureFlip(SDL_FLIP_NONE);
		}
		else if (keyboard.GetKeyState(SDLK_d) && player.Position().x < static_cast<int>(ProgramData::Data::SCREEN_WIDTH) && !(collision.Direction & E))
		{
			if (fieldWidth != ProgramData::Data::SCREEN_WIDTH)
			{
				if (player.AbsoluteX() <= ProgramData::Data::CenterX())
				{
					player.ChangeAbsolutePosition(movement, 0);
					player.ChangePosition(movement, 0);
				}
				else if (player.AbsoluteX() >= fieldWidth - ProgramData::Data::CenterX())
				{
					if (player.Position().x + player.Position().w + movement > static_cast<int>(ProgramData::Data::SCREEN_WIDTH))
					{
						player.SetAbsolutePosition(fieldWidth - player.Position().w, player.AbsoluteY());
						player.SetPosition(ProgramData::Data::SCREEN_WIDTH - player.Position().w, player.Position().y);
						if (buildings != nullptr)
							for (auto& x : *buildings)
								x.second.ChangePosition(player.Position().x + player.Position().w - ProgramData::Data::SCREEN_WIDTH, 0);
						if (monsters != nullptr)
							for (auto& x : *monsters)
								if (x.second.IsActive())
									x.second.ChangePosition(player.Position().x + player.Position().w - ProgramData::Data::SCREEN_WIDTH, 0);
						if (items != nullptr)
							for (auto& x : *items)
								x.second.ChangePosition(player.Position().x + player.Position().w - ProgramData::Data::SCREEN_WIDTH, 0);
					}
					else
					{
						player.ChangeAbsolutePosition(movement, 0);
						player.ChangePosition(movement, 0);
					}
				}
				else
				{
					player.SetPosition(static_cast<int>(ProgramData::Data::CenterX()), player.Position().y);
					player.ChangeAbsolutePosition(movement, 0);
					if (buildings != nullptr)
						for (auto& x : *buildings)
							x.second.ChangePosition(-1 * movement, 0);
					if (monsters != nullptr)
						for (auto& x : *monsters)
							if (x.second.IsActive())
								x.second.ChangePosition(-1 * movement, 0);
					if (items != nullptr)
						for (auto& x : *items)
							x.second.ChangePosition(-1 * movement, 0);

					if (x != nullptr)
					{
						if (*x < -68)
							*x += 68 - movement;
						else
							*x -= movement;
					}
				}
			}
			else
			{
				if (player.Position().x + player.Position().w + movement > static_cast<int>(ProgramData::Data::SCREEN_WIDTH))
				{
					player.SetAbsolutePosition(fieldWidth - player.Position().w, player.AbsoluteY());
					player.SetPosition(ProgramData::Data::SCREEN_WIDTH - player.Position().w, player.Position().y);
				}
				else
				{
					player.ChangeAbsolutePosition(movement, 0);
					player.ChangePosition(movement, 0);
				}
			}
			player.SetTextureFlip(SDL_FLIP_HORIZONTAL);
		}
		if (monsters != nullptr)
			for (auto& x : *monsters)
			{
				short dir = x.second.CollisionDirection(player) | CheckCollision(x.second, *monsters);
				if (buildings != nullptr)
					dir |= CheckCollision(x.second, *buildings).Direction;
				x.second.Move(player, fieldWidth, fieldHeight, dir);
			}
	}

	void Game::CheckDeath(NpcMap& monsters)
	{
		for (auto& x : monsters)
			if (!x.second.IsActive())
				x.second.DeathSpawn();
	}

	void Game::GetItems(PickUpMap& items)
	{
		std::vector<std::string> toDelete;
		for (auto& x : items)
		{
			if (Distance(gameSave.Player(), x.second) <= 10)
			{
				gameSave.Player().GetItem(x.second.DropItem());
				toDelete.emplace_back(x.first);
			}
		}
		for (auto& x : toDelete)
			items.erase(x);
	}

	void Game::EnterBuilding(const std::string& name)
	{
		if (name == "castle")
			return Castle();
		else
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "Cannot find and call function: " << name << std::endl;
			fout.close();
		}
	}

	void Game::PauseMenu(bool gameOver)
	{
		auto& backgroundTex = ProgramData::Data::GetTextures().Background;
		if (gameOver)
		{
			Mix_PlayMusic(ProgramData::Data::Music().at(1), -1);
			backgroundTex.LoadFile("Textures/GameOver.png");
		}
		else if (!ProgramData::Data::KonamiCode())
			Mix_PlayMusic(ProgramData::Data::Music().at(5), -1);
		backgroundTex.SetBlendMode(SDL_BLENDMODE_BLEND);
		backgroundTex.SetAlpha(170);

		auto& menuLabels = ProgramData::Data::GetLabels().Menu;
		menuLabels.at(0).Texture().LoadText("Resume", ProgramData::Data::NormalColor());
		menuLabels.at(2).Texture().LoadText("Main Menu", ProgramData::Data::NormalColor());
		bool finish = false;
		auto& keyboard = Events::EventHandler::GetKeyboard();
		while (!finish)
		{
			SDL_RenderClear(ProgramData::Data::Renderer());
			backgroundTex.Render();
			if (!gameOver)
				menuLabels.at(0).Render();
			for_each(menuLabels.begin() + 1, menuLabels.end(), [](Renders::Label& x) { x.Render(); });
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					keyboard.HandleKey(e);
					break;
				}
				case SDL_MOUSEMOTION:
				{
					int X, Y;
					SDL_GetMouseState(&X, &Y);
					for (auto& x : menuLabels)
						x.Texture().SetColor(x.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int X, Y;
						SDL_GetMouseState(&X, &Y);
						finish = menuLabels.at(0).IsMouseOn(X, Y);

						if (menuLabels.at(1).IsMouseOn(X, Y))
						{
							if (!ProgramData::Data::KonamiCode())
								Mix_PlayMusic(ProgramData::Data::Music().at(0), -1);
							else if (!gameOver)
								Mix_PlayMusic(ProgramData::Data::CodeMusic(), -1);
							quit = 1;
							finish = true;
						}
						if (menuLabels.at(2).IsMouseOn(X, Y))
						{
							if (!ProgramData::Data::KonamiCode())
								Mix_PlayMusic(ProgramData::Data::Music().at(0), -1);
							else if (!gameOver)
								Mix_PlayMusic(ProgramData::Data::CodeMusic(), -1);
							quit = 2;
							finish = true;
						}
					}
					break;
				}
				}
			}
			if (keyboard.GetKeyState(SDLK_ESCAPE))
			{
				keyboard.SetKeyState(SDLK_ESCAPE, false);
				break;
			}
		}
		backgroundTex.SetAlpha(255);
		menuLabels.at(0).Texture().LoadText("New Game", ProgramData::Data::NormalColor());
		menuLabels.at(2).Texture().LoadText("Exit", ProgramData::Data::NormalColor());
		if (gameOver)
			backgroundTex.LoadFile("Textures/LoadBackground.png");
	}

	void Game::Castle()
	{
		if (!ProgramData::Data::KonamiCode())
			Mix_PlayMusic(ProgramData::Data::Music().at(9), -1);
		Renders::Texture Brick("Textures/Brick.png");
		auto& player = gameSave.Player();;
		isNewLevel = player.PointsLefToUse() > 0;
		auto& keyboard = Events::EventHandler::GetKeyboard();
		while (true)
		{
			SDL_RenderClear(ProgramData::Data::Renderer());
			SDL_RenderFillRect(ProgramData::Data::Renderer(), nullptr);
			for (short i = 0; i < 14; ++i)
				for (short j = 0; j < 7; ++j)
					Brick.Render(260 + i * 100, 140 + j * 100);
			for (short i = 0; i < 2; ++i)
				for (short j = 0; j < 2; ++j)
					Brick.Render(860 + i * 100, 840 + j * 100);
			RenderOrder("castle");
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					keyboard.HandleKey(e);
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int x, y;
						SDL_GetMouseState(&x, &y);
						Attack(x, y, "castle");
					}
					break;
				}
				}
			}

			CollisionEntrance collision = CheckCollision(player, *gameSave.Buildings("castle"));
			collision.Direction |= CheckCollision(player, *gameSave.Monsters("castle"));
			if (keyboard.GetKeyState(SDLK_ESCAPE))
			{
				keyboard.SetKeyState(SDLK_ESCAPE, false);
				PauseMenu();
				if (quit == 1 || quit == 2)
				{
					gameSave.SaveGame("castle");
					break;
				}
				if (!ProgramData::Data::KonamiCode())
					Mix_PlayMusic(ProgramData::Data::Music().at(9), -1);
			}
			if (keyboard.GetKeyState(SDLK_s) && collision.Direction & Direction::Enter)
			{
				keyboard.SetKeyState(SDLK_s, false);
				break;
			}
			if (keyboard.GetKeyState(SDLK_i))
			{
				keyboard.SetKeyState(SDLK_i, false);
				Info::PlayerEquipment(player);
				isNewLevel = player.PointsLefToUse() > 0;
			}
			if (keyboard.GetKeyState(SDLK_k))
			{
				keyboard.SetKeyState(SDLK_k, false);
				Info::PlayerStatistics(player);
				isNewLevel = player.PointsLefToUse() > 0;
			}
			MoveObjects("castle", collision, ProgramData::Data::SCREEN_WIDTH, ProgramData::Data::SCREEN_HEIGHT);
			GetItems(*gameSave.Items("castle"));
			CheckDeath(*gameSave.Monsters("castle"));
			if (player.Health() == 0)
			{
				PauseMenu(true);
				break;
			}
		}
	}

	void Game::Play(const std::string& place)
	{
		gameOn = true;
		auto& player = gameSave.Player();
		if (place == "castle")
		{
			Castle();
			if (!quit)
			{
				player.SetAbsolutePosition(585, 1240);
				player.SetPosition(585, 466);
				if (!ProgramData::Data::KonamiCode())
					Mix_PlayMusic(ProgramData::Data::Music().at(7), -1);
			}
		}
		else
		{
			if (!ProgramData::Data::KonamiCode())
				Mix_PlayMusic(ProgramData::Data::Music().at(7), -1);
			isNewLevel = player.PointsLefToUse() > 0;
		}
		int X = 0, Y = 0;
		auto& keyboard = Events::EventHandler::GetKeyboard();
		if (!quit)
			while (true)
			{
				SDL_RenderClear(ProgramData::Data::Renderer());
				for (int i = -4, I = ProgramData::Data::SCREEN_WIDTH / 68 + ProgramData::Data::SCREEN_WIDTH % 68 + 4; i <= I; ++i)
					for (int j = -4, J = ProgramData::Data::SCREEN_HEIGHT / 68 + ProgramData::Data::SCREEN_HEIGHT % 68 + 4; j <= J; ++j)
						ProgramData::Data::GetTextures().OutFloor.Render(i * 68 + X, j * 68 + Y);
				RenderOrder("main");
				SDL_RenderPresent(ProgramData::Data::Renderer());

				SDL_Event e;
				while (SDL_PollEvent(&e))
				{
					switch (e.type)
					{
					case SDL_KEYUP:
					case SDL_KEYDOWN:
					{
						keyboard.HandleKey(e);
						break;
					}
					case SDL_MOUSEBUTTONDOWN:
					{
						if (e.button.button == SDL_BUTTON_LEFT)
						{
							int x, y;
							SDL_GetMouseState(&x, &y);
							Attack(x, y, "main");
						}
						break;
					}
					}
				}

				CollisionEntrance collision = CheckCollision(player, *gameSave.Buildings("main"));
				collision.Direction |= CheckCollision(player, *gameSave.Monsters("main"));
				if (keyboard.GetKeyState(SDLK_ESCAPE))
				{
					keyboard.SetKeyState(SDLK_ESCAPE, false);
					PauseMenu();
					if (quit)
					{
						gameSave.SaveGame("main");
						break;
					}
					if (!ProgramData::Data::KonamiCode())
						Mix_PlayMusic(ProgramData::Data::Music().at(7), -1);
				}
				if (keyboard.GetKeyState(SDLK_w) && collision.Direction & Direction::Enter)
				{
					keyboard.SetKeyState(SDLK_w, false);
					SDL_Rect pos = player.Position();
					pos.w = static_cast<int>(player.AbsoluteX());
					pos.h = static_cast<int>(player.AbsoluteY());
					player.SetPosition(static_cast<int>(ProgramData::Data::CenterX()), static_cast<int>(ProgramData::Data::CenterY() + 400));
					player.SetAbsolutePosition(ProgramData::Data::CenterX(), ProgramData::Data::CenterY() + 400);
					EnterBuilding(collision.Building->Name());
					if (quit)
						break;
					if (!ProgramData::Data::KonamiCode())
						Mix_PlayMusic(ProgramData::Data::Music().at(7), -1);
					player.SetPosition(pos.x, pos.y);
					player.SetAbsolutePosition(pos.w, pos.h);
					collision = { Direction::N, nullptr };
				}
				if (keyboard.GetKeyState(SDLK_i))
				{
					keyboard.SetKeyState(SDLK_i, false);
					Info::PlayerEquipment(player);
					isNewLevel = player.PointsLefToUse();
				}
				if (keyboard.GetKeyState(SDLK_k))
				{
					keyboard.SetKeyState(SDLK_k, false);
					Info::PlayerStatistics(player);
					isNewLevel = player.PointsLefToUse();
				}
				MoveObjects("main", collision, ProgramData::Data::FIELD_WIDTH, ProgramData::Data::FIELD_HEIGHT, &X, &Y);
				GetItems(*gameSave.Items("main"));
				CheckDeath(*gameSave.Monsters("main"));
				if (player.Health() == 0)
				{
					PauseMenu(true);
					break;
				}
			}
		if (quit == 1)
			return LoadGame();
	}
}