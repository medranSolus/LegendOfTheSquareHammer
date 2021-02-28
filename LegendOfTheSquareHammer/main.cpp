/*
Author: Marek Machliñski
Date:	28.02.2021
*/
#include "Data.h"
#include "EventHandler.h"
#include "Game.h"
#include <ctime>
#include <filesystem>

namespace LOTSH
{
	void Initialize();
	void Close();
	void LoadMedia();
	void Menu();
	void NewGame();
	void LoadGame();
	std::unique_ptr<Renders::Objects::Player> CreatePlayer();
	void GetSaves();

	std::map<std::string, void(*)()> MenuOptions{ { "close",Close },{ "menu",Menu },{ "newGame",NewGame },{ "loadGame",LoadGame } };
}

int main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(NULL)));
	LOTSH::Initialize();
	LOTSH::LoadMedia();
	LOTSH::Menu();
	LOTSH::Close();
	return 0;
}

namespace LOTSH
{
	void Initialize()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "SDL_Init error: " << SDL_GetError() << std::endl;
			fout.close();
			exit(0b1);
		}
		ProgramData::Data::Window() = SDL_CreateWindow("Legend of The Square Hammer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			ProgramData::Data::SCREEN_WIDTH, ProgramData::Data::SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
		if (ProgramData::Data::Window() == nullptr)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
			fout.close();
			exit(0b10);
		}
		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "SDL_Image load error: " << IMG_GetError() << std::endl;
			fout.close();
			exit(0b100);
		}
		ProgramData::Data::ScreenSurface() = SDL_GetWindowSurface(ProgramData::Data::Window());
		if (ProgramData::Data::ScreenSurface() == nullptr)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "SDL_GetWindowSurface error: " << SDL_GetError() << std::endl;
			fout.close();
			exit(0b1000);
		}
		ProgramData::Data::Renderer() = SDL_CreateRenderer(ProgramData::Data::Window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		if (ProgramData::Data::Renderer() == nullptr)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
			fout.close();
			exit(0b10000);
		}
		SDL_RenderSetLogicalSize(ProgramData::Data::Renderer(), ProgramData::Data::SCREEN_WIDTH, ProgramData::Data::SCREEN_HEIGHT);
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "Mix_OpenAudio error: " << Mix_GetError() << std::endl;
			fout.close();
			exit(0b100000);
		}
		if (TTF_Init() == -1)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "TTF_Init error: " << SDL_GetError() << std::endl;
			fout.close();
			exit(0b1000000);
		}
		ProgramData::Data::CodeKeys().reserve(10);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_UP);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_UP);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_DOWN);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_DOWN);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_LEFT);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_RIGHT);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_LEFT);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_RIGHT);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_b);
		ProgramData::Data::CodeKeys().emplace_back(SDLK_a);
	}

	void Close()
	{
		for (auto& x : ProgramData::Data::Music())
			Mix_FreeMusic(x);
		Mix_FreeMusic(ProgramData::Data::CodeMusic());
		SDL_DestroyRenderer(ProgramData::Data::Renderer());
		TTF_CloseFont(ProgramData::Data::Font());
		SDL_FreeSurface(ProgramData::Data::ScreenSurface());
		SDL_DestroyWindow(ProgramData::Data::Window());
		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
		exit(0);
	}

	void LoadMedia()
	{
		auto& textures = ProgramData::Data::GetTextures();
		textures.Background.LoadFile("Textures/LoadBackground.png");
		textures.Background.SetBlendMode(SDL_BLENDMODE_BLEND);
		textures.Background.Render();
		SDL_RenderPresent(ProgramData::Data::Renderer());

#pragma region Music
		ProgramData::Data::Music().reserve(11);
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/Square Hammer.mp3"));
		Mix_PlayMusic(ProgramData::Data::Music().front(), -1);
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/Absolution.mp3"));
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/Cirice.mp3"));
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/From the Pinnacle to the Pit.mp3"));
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/Ghuleh Zombie Queen.mp3"));
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/He Is.mp3"));
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/Infestissumam.mp3"));
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/Majesty.mp3"));
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/Monstrance Clock.mp3"));
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/Ritual.mp3"));
		ProgramData::Data::Music().emplace_back(Mix_LoadMUS("Music/Year Zero.mp3"));
		ProgramData::Data::CodeMusic() = Mix_LoadMUS("Music/Sax.mp3");
#pragma endregion
		ProgramData::Data::Font() = TTF_OpenFont("SEASRN.ttf", 200);
#pragma region Textures
		textures.Title.LoadFile("Textures/Title.png");
		textures.OutFloor.LoadFile("Textures/OutFloor.png");
		std::ifstream fin("Objects/Menu.ttr");
		size_t I = 0;
		fin >> I;
		textures.MenuBackground.resize(I);
		for (auto& tex : textures.MenuBackground)
			fin >> tex;
		fin.close();
		I = 0;
		fin.open("Objects/Sax.ttr");
		fin >> I;
		textures.CodeMenuBackground.resize(I);
		for (auto& tex : textures.CodeMenuBackground)
			fin >> tex;
		fin.close();
#pragma endregion
#pragma region Labels
#define LOAD_LABELS(X) fin.open("Objects/" #X ".lab"); \
	fin >> I; \
	labels.X.resize(I); \
	for (auto& lab : labels.X) \
		fin >> lab; \
	fin.close()

		auto& labels = ProgramData::Data::GetLabels();
		LOAD_LABELS(Menu);
		LOAD_LABELS(Items);
		LOAD_LABELS(Stats);
		LOAD_LABELS(Resistance);
		LOAD_LABELS(Damage);
		LOAD_LABELS(Mini);
		LOAD_LABELS(Info);
		LOAD_LABELS(Armor);
		LOAD_LABELS(Weapon);
		LOAD_LABELS(Potion);
		ProgramData::Data::GetSaveLabels().resize(5);
		fin.open("Objects/NewGame.lab");
		for (auto& x : ProgramData::Data::GetSaveLabels())
			fin >> x.second.first >> x.second.second;
		fin.close();
		labels.Arrows.resize(5);
		fin.open("Objects/UpgradeArrows.lab");
		for (auto& x : labels.Arrows)
			fin >> x.first >> x.second;
		fin.close();
#pragma endregion
	}

	void GetSaves()
	{
		short i = 0;
		auto& labels = ProgramData::Data::GetSaveLabels();
		if (!std::filesystem::exists("Save"))
			if (!std::filesystem::create_directory("Save"))
			{
				std::ofstream fout("error.txt", std::ios_base::app);
				fout << "Cannot create directory \"Save\"! Create it manually." << std::endl;
				fout.close();
				exit(-1);
			}
		for (auto& iterator : std::filesystem::directory_iterator("Save"))
		{
			std::string name = iterator.path().string();
			if (name.size() > 5
				&& name.back() == 'v'
				&& name.at(name.size() - 2) == 'a'
				&& name.at(name.size() - 3) == 's'
				&& name.at(name.size() - 4) == '.')
			{
				size_t position = name.find('\\');
				while (position != std::string::npos)
				{
					name.erase(0, position + 1);
					position = name.find('\\');
				}
				name.pop_back();
				name.pop_back();
				name.pop_back();
				name.pop_back();
				labels.at(i++).first = name;
			}
			if (labels.size() == 5)
				break;
		}
		for (auto& lab : labels)
		{
			if (lab.first.size() == 0)
				lab.first = "Pusty...  ";
			lab.second.first.Texture().LoadText(lab.first, ProgramData::Data::NormalColor());
		}
	}

#pragma region Display
	void Menu()
	{
		bool quit = false, reverse = false;
		int frame = 0;
		size_t codeCounter = 0;
		auto& labels = ProgramData::Data::GetLabels();
		while (!quit)
		{
			SDL_RenderClear(ProgramData::Data::Renderer());
			if (!ProgramData::Data::KonamiCode())
				ProgramData::Data::GetTextures().MenuBackground.at(frame).Render();
			else
				ProgramData::Data::GetTextures().CodeMenuBackground.at(frame).Render();
			for (auto& lab : labels.Menu)
				lab.Render();
			ProgramData::Data::GetTextures().Title.Render(0, 0, ProgramData::Data::SCREEN_WIDTH, static_cast<int>(ProgramData::Data::SCREEN_HEIGHT / 4.5));
			SDL_RenderPresent(ProgramData::Data::Renderer());

			if (!ProgramData::Data::KonamiCode())
			{
				if (!reverse && frame < 82)
					++frame;
				else if (reverse && frame > 0)
					--frame;
				else if (frame == 82)
					reverse = true;
				else if (frame == 0)
					reverse = false;
			}
			else if (codeCounter == 0)
			{
				if (!reverse && frame < 9)
					++frame;
				else if (reverse && frame > 0)
					--frame;
				else if (frame == 9)
					reverse = true;
				else if (frame == 0)
					reverse = false;
				codeCounter = 2;
			}
			else
				--codeCounter;

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_QUIT:
				{
					quit = true;
					break;
				}
				case SDL_KEYDOWN:
				{
					if (!ProgramData::Data::KonamiCode() && e.key.keysym.sym == ProgramData::Data::CodeKeys().at(codeCounter))
					{
						++codeCounter;
						ProgramData::Data::KonamiCode() = codeCounter == ProgramData::Data::CodeKeys().size();
					}
					else if (!ProgramData::Data::KonamiCode())
						codeCounter = 0;
					break;
				}
				case SDL_MOUSEMOTION:
				{
					int X, Y;
					SDL_GetMouseState(&X, &Y);
					for (auto& lab : labels.Menu)
						lab.Texture().SetColor(lab.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int X, Y;
						SDL_GetMouseState(&X, &Y);
						for (auto& lab : labels.Menu)
							if (lab.IsMouseOn(X, Y))
								lab();
					}
					break;
				}
				}
			}
			if (ProgramData::Data::KonamiCode() && codeCounter == ProgramData::Data::CodeKeys().size())
			{
				ProgramData::Data::NormalColor() = { 255,105,180,255 };
				Mix_PlayMusic(ProgramData::Data::CodeMusic(), -1);
				codeCounter = 2;
				for (auto& lab : labels.Menu)
				{
					lab.Texture().SetBlendMode(SDL_BLENDMODE_BLEND);
					lab.Texture().SetAlpha(120);
					lab.Texture().SetColor(255, 105, 180);
				}
				for (auto& lab : labels.Mini)
					lab.Texture().SetColor(255, 105, 180);
				frame = 0;
				ProgramData::Data::GetTextures().Title.SetColor(255, 105, 180);
				ProgramData::Data::GetTextures().Background.SetColor(255, 105, 180);
			}
		}
	}

	void NewGame()
	{
		ProgramData::Data::GetTextures().Background.SetBlendMode(SDL_BLENDMODE_BLEND);
		ProgramData::Data::GetTextures().Background.SetAlpha(170);
		GetSaves();
		auto& backButton = ProgramData::Data::GetLabels().Mini.at(0);
		bool stay = true, play = false;
		while (stay)
		{
			SDL_RenderClear(ProgramData::Data::Renderer());
			ProgramData::Data::GetTextures().Background.Render();
			for (auto& x : ProgramData::Data::GetSaveLabels())
			{
				x.second.first.Render();
				if (x.first != "Pusty...  " && x.first != "Cannot delete!")
					x.second.second.Render();
			}
			backButton.Render();
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					Events::EventHandler::GetKeyboard().HandleKey(e);
					break;
				}
				case SDL_MOUSEMOTION:
				{
					int X, Y;
					SDL_GetMouseState(&X, &Y);
					backButton.Texture().SetColor(backButton.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					for (auto& x : ProgramData::Data::GetSaveLabels())
					{
						if (x.first != "Cannot delete!")
						{
							x.second.first.Texture().SetColor(x.second.first.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
							if (x.first != "Pusty...  ")
								x.second.second.Texture().SetColor(x.second.second.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
						}
					}
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int X, Y;
						SDL_GetMouseState(&X, &Y);
						if (backButton.IsMouseOn(X, Y))
							backButton();
						for (auto& x : ProgramData::Data::GetSaveLabels())
						{
							if (x.first != "Cannot delete!")
							{
								if (x.first == "Pusty...  " && x.second.first.IsMouseOn(X, Y))
								{
									auto player = CreatePlayer();
									if (player != nullptr)
									{
										ProgramData::Data::CurrentGame() = std::make_unique<Game>(std::move(player));
										play = true;
										stay = false;
										break;
									}
								}
								else if (x.first != "Pusty...  " && x.second.second.IsMouseOn(X, Y))
								{
									std::error_code err;
									if (std::filesystem::remove("\\Save\\" + x.first + ".sav", err))
									{
										x.first = "Pusty...  ";
										x.second.first.Texture().LoadText(x.first, ProgramData::Data::NormalColor());
									}
									else
									{
										std::ofstream fout("error.txt", std::ios_base::app);
										fout << err << std::endl << std::string(std::filesystem::current_path().string() + "\\Save\\" + x.first + ".sav") << std::endl;
										fout.close();
										x.first = "Cannot delete!";
										x.second.first.Texture().LoadText(x.first, ProgramData::Data::NormalColor());
										x.second.first.Texture().SetColor({ 255,0,0,255 });
									}
								}
							}
						}
					}
					break;
				}
				}
			}
			if (Events::EventHandler::GetKeyboard().GetKeyState(SDLK_ESCAPE))
			{
				Events::EventHandler::GetKeyboard().SetKeyState(SDLK_ESCAPE, false);
				stay = false;
			}
		}
		ProgramData::Data::GetTextures().Background.SetAlpha(255);
		if (play)
			return ProgramData::Data::CurrentGame()->Play();
	}

	void LoadGame()
	{
		ProgramData::Data::GetTextures().Background.SetBlendMode(SDL_BLENDMODE_BLEND);
		ProgramData::Data::GetTextures().Background.SetAlpha(170);
		GetSaves();
		auto& backButton = ProgramData::Data::GetLabels().Mini.at(0);
		bool stay = true;
		while (stay)
		{
			SDL_RenderClear(ProgramData::Data::Renderer());
			ProgramData::Data::GetTextures().Background.Render();
			for (auto& x : ProgramData::Data::GetSaveLabels())
			{
				x.second.first.Render();
				if (x.first != "Pusty...  " && x.first != "Cannot delete!")
					x.second.second.Render();
			}
			backButton.Render();
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					Events::EventHandler::GetKeyboard().HandleKey(e);
					break;
				}
				case SDL_MOUSEMOTION:
				{
					int X, Y;
					SDL_GetMouseState(&X, &Y);
					backButton.Texture().SetColor(backButton.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					for (auto& x : ProgramData::Data::GetSaveLabels())
					{
						if (x.first != "Cannot delete!" && x.first != "Pusty...  ")
						{
							x.second.first.Texture().SetColor(x.second.first.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
							x.second.second.Texture().SetColor(x.second.second.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
						}
					}
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int X, Y;
						SDL_GetMouseState(&X, &Y);
						if (backButton.IsMouseOn(X, Y))
							backButton();
						for (auto& x : ProgramData::Data::GetSaveLabels())
						{
							if (x.first != "Cannot delete!" && x.first != "Pusty...  ")
							{
								if (x.second.first.IsMouseOn(X, Y))
								{
									std::string place;
									ProgramData::Data::CurrentGame() = std::make_unique<Game>("Save/" + x.first + ".sav", place);
									Mix_PlayMusic(ProgramData::Data::Music().at(10), -1);
									return ProgramData::Data::CurrentGame()->Play(place);
								}
								else if (x.second.second.IsMouseOn(X, Y))
								{
									std::error_code err;
									if (std::filesystem::remove("\\Save\\" + x.first + ".sav", err))
									{
										x.first = "Pusty...  ";
										x.second.first.Texture().LoadText(x.first, ProgramData::Data::NormalColor());
									}
									else
									{
										std::ofstream fout("error.txt", std::ios_base::app);
										fout << err << std::endl << std::string(std::filesystem::current_path().string() + "\\Save\\" + x.first + ".sav") << std::endl;
										fout.close();
										x.first = "Cannot delete!";
										x.second.first.Texture().LoadText(x.first, ProgramData::Data::NormalColor());
										x.second.first.Texture().SetColor({ 255,0,0,255 });
									}
								}
							}
						}
					}
					break;
				}
				}
			}
			if (Events::EventHandler::GetKeyboard().GetKeyState(SDLK_ESCAPE))
			{
				Events::EventHandler::GetKeyboard().SetKeyState(SDLK_ESCAPE, false);
				stay = false;
			}
		}
		ProgramData::Data::GetTextures().Background.SetAlpha(255);
	}

	std::unique_ptr<Renders::Objects::Player> CreatePlayer()
	{
		if (!ProgramData::Data::KonamiCode())
			Mix_PlayMusic(ProgramData::Data::Music().at(10), -1);
		Renders::Label labelName;
		Renders::Label labelPoints;
		Renders::Label labelPlay;
		Renders::Label labelBack;
		Renders::Label labelTable;
		std::vector<std::vector<Renders::Label>> labelsStatistics(5);

		std::ifstream fin("Objects/Create.lab");
		fin >> labelName >> labelPoints >> labelPlay >> labelBack >> labelTable;
		if (ProgramData::Data::KonamiCode())
		{
			labelName.Texture().SetColor(ProgramData::Data::NormalColor());
			labelPoints.Texture().SetColor(ProgramData::Data::NormalColor());
			labelPlay.Texture().SetColor(ProgramData::Data::NormalColor());
			labelTable.Texture().SetColor(ProgramData::Data::NormalColor());
		}
		for (auto& lab : labelsStatistics)
		{
			lab.resize(3);
			for (auto& x : lab)
			{
				fin >> x;
				if (ProgramData::Data::KonamiCode())
					x.Texture().SetColor(ProgramData::Data::NormalColor());
			}
		}
		fin.close();

		constexpr size_t NAME_MAX_LENGTH = 16;
		size_t pointsLeft = 10;
		std::string name = "";
		Stats statistics;
		statistics.LevelUp({ 8, 10, 9, 11, 12, 0, 0, 0, 0, 0 });
		bool done = false, changed = false, textFocus = false;
		while (!done)
		{
			SDL_RenderClear(ProgramData::Data::Renderer());
			ProgramData::Data::GetTextures().Background.Render();
			labelName.Render();
			labelTable.Render();
			labelPoints.Render();
			if (name.size() && !pointsLeft)
				labelPlay.Render();
			labelBack.Render();
			for (auto& lab : labelsStatistics)
			{
				lab.front().Render();
				if (pointsLeft > 0)
					lab.at(2).Render();
			}
			if (statistics.MainStatistics.Inteligence > 8)
				labelsStatistics.at(0).at(1).Render();
			if (statistics.MainStatistics.Strenght > 10)
				labelsStatistics.at(1).at(1).Render();
			if (statistics.MainStatistics.Dextermity > 9)
				labelsStatistics.at(2).at(1).Render();
			if (statistics.Endurance > 11)
				labelsStatistics.at(3).at(1).Render();
			if (statistics.Vitality > 12)
				labelsStatistics.at(4).at(1).Render();
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
				{
					if (textFocus)
					{
						if (name.size() < NAME_MAX_LENGTH
							&& ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9)
								|| (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z)
								|| e.key.keysym.sym == SDLK_SPACE
								|| e.key.keysym.sym == SDLK_QUOTE
								|| e.key.keysym.sym == SDLK_MINUS
								|| e.key.keysym.sym == SDLK_UNDERSCORE))
						{
							name += (char)e.key.keysym.sym;
							changed = true;
						}
						if (e.key.keysym.sym == SDLK_BACKSPACE && name.size() > 0)
						{
							name.pop_back();
							changed = true;
						}
					}
				}
				case SDL_KEYUP:
				{
					Events::EventHandler::GetKeyboard().HandleKey(e);
					break;
				}
				case SDL_MOUSEMOTION:
				{
					int X, Y;
					SDL_GetMouseState(&X, &Y);
					labelPlay.Texture().SetColor(labelPlay.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					labelBack.Texture().SetColor(labelBack.IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					for (auto& lab : labelsStatistics)
						for (size_t i = 1; i < lab.size(); ++i)
							lab.at(i).Texture().SetColor(lab.at(i).IsMouseOn(X, Y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int X, Y;
						SDL_GetMouseState(&X, &Y);
						textFocus = labelName.IsMouseOn(X, Y);
						labelName.Texture().SetColor(textFocus ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());

						if (pointsLeft == 0 && name.size() > 0 && labelPlay.IsMouseOn(X, Y))
							done = true;
						else if (labelBack.IsMouseOn(X, Y))
							return nullptr;
						else if (pointsLeft > 0)
						{
							bool clicked = false;
							if (labelsStatistics.at(0).at(2).IsMouseOn(X, Y))
							{
								clicked = true;
								++statistics.MainStatistics.Inteligence;
								labelsStatistics.at(0).at(0).Texture().LoadText("Inteligence:  " + std::to_string(statistics.MainStatistics.Inteligence), ProgramData::Data::NormalColor());
							}
							else if (labelsStatistics.at(1).at(2).IsMouseOn(X, Y))
							{
								clicked = true;
								++statistics.MainStatistics.Strenght;
								labelsStatistics.at(1).at(0).Texture().LoadText("Strenght:  " + std::to_string(statistics.MainStatistics.Strenght), ProgramData::Data::NormalColor());
							}
							else if (labelsStatistics.at(2).at(2).IsMouseOn(X, Y))
							{
								clicked = true;
								++statistics.MainStatistics.Dextermity;
								labelsStatistics.at(2).at(0).Texture().LoadText("Dextermity:  " + std::to_string(statistics.MainStatistics.Dextermity), ProgramData::Data::NormalColor());
							}
							else if (labelsStatistics.at(3).at(2).IsMouseOn(X, Y))
							{
								clicked = true;
								++statistics.Endurance;
								labelsStatistics.at(3).at(0).Texture().LoadText("Endurance:  " + std::to_string(statistics.Endurance), ProgramData::Data::NormalColor());
							}
							else if (labelsStatistics.at(4).at(2).IsMouseOn(X, Y))
							{
								clicked = true;
								++statistics.Vitality;
								labelsStatistics.at(4).at(0).Texture().LoadText("Vitality:  " + std::to_string(statistics.Vitality), ProgramData::Data::NormalColor());
							}
							if (clicked)
								labelPoints.Texture().LoadText("Points left:  " + std::to_string(--pointsLeft), ProgramData::Data::NormalColor());
						}
						bool clicked = false;
						if (statistics.MainStatistics.Inteligence > 8 && labelsStatistics.at(0).at(1).IsMouseOn(X, Y))
						{
							clicked = true;
							--statistics.MainStatistics.Inteligence;
							labelsStatistics.at(0).at(0).Texture().LoadText("Inteligence:  " + std::to_string(statistics.MainStatistics.Inteligence), ProgramData::Data::NormalColor());
						}
						else if (statistics.MainStatistics.Strenght > 10 && labelsStatistics.at(1).at(1).IsMouseOn(X, Y))
						{
							clicked = true;
							--statistics.MainStatistics.Strenght;
							labelsStatistics.at(1).at(0).Texture().LoadText("Strenght:  " + std::to_string(statistics.MainStatistics.Strenght), ProgramData::Data::NormalColor());
						}
						else if (statistics.MainStatistics.Dextermity > 9 && labelsStatistics.at(2).at(1).IsMouseOn(X, Y))
						{
							clicked = true;
							--statistics.MainStatistics.Dextermity;
							labelsStatistics.at(2).at(0).Texture().LoadText("Dextermity:  " + std::to_string(statistics.MainStatistics.Dextermity), ProgramData::Data::NormalColor());
						}
						else if (statistics.Endurance > 11 && labelsStatistics.at(3).at(1).IsMouseOn(X, Y))
						{
							clicked = true;
							--statistics.Endurance;
							labelsStatistics.at(3).at(0).Texture().LoadText("Endurance:  " + std::to_string(statistics.Endurance), ProgramData::Data::NormalColor());
						}
						else if (statistics.Vitality > 12 && labelsStatistics.at(4).at(1).IsMouseOn(X, Y))
						{
							clicked = true;
							--statistics.Vitality;
							labelsStatistics.at(4).at(0).Texture().LoadText("Vitality:  " + std::to_string(statistics.Vitality), ProgramData::Data::NormalColor());
						}
						if (clicked)
							labelPoints.Texture().LoadText("Points left:  " + std::to_string(++pointsLeft), ProgramData::Data::NormalColor());
					}
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					break;
				}
				}
			}
			if (Events::EventHandler::GetKeyboard().GetKeyState(SDLK_ESCAPE))
			{
				Events::EventHandler::GetKeyboard().SetKeyState(SDLK_ESCAPE, false);
				textFocus = false;
			}
			if (textFocus && changed)
			{
				std::string displayName = name;
				for (short i = 0, I = short(NAME_MAX_LENGTH - name.size() - 1); i <= I; ++i)
					displayName += '_';
				labelName.Texture().LoadText("Name:    " + displayName, ProgramData::Data::HOOVER_COLOR);
				changed = false;
			}
		}
		return std::make_unique<Renders::Objects::Player>(name, statistics);
	}
#pragma endregion
}