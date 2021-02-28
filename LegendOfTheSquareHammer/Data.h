/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "LabelsData.h"
#include "TexturesData.h"
#include "Game.h"
#include <SDL_mixer.h>

namespace LOTSH::ProgramData
{
	typedef std::pair<std::string, std::pair<Renders::Label, Renders::Label>> SavePair;

	class Data
	{
		static inline LabelsData labelsData;
		static inline TexturesData texturesData;
		static inline std::vector<SavePair> saveLabels;
		static inline SDL_Color normalColor = { 255,255,255,255 };
		static inline bool konamiCode = false;
		static inline size_t centerX = 0;
		static inline size_t centerY = 0;
		static inline SDL_Window* window = nullptr;
		static inline SDL_Surface* screenSurface = nullptr;
		static inline SDL_Renderer* renderer = nullptr;
		static inline TTF_Font* font = nullptr;
		static inline Mix_Music* codeMusic = nullptr;
		static inline std::vector<SDL_Keycode> codeKeys;
		static inline std::vector<Mix_Music*> music;
		static inline std::unique_ptr<Game> currentGame = nullptr;

	public:
		Data() = delete;

		static constexpr size_t FIELD_WIDTH = 3000;
		static constexpr size_t FIELD_HEIGHT = 5000;
		static constexpr double SCREEN_SCALE = 1;
		static constexpr size_t SCREEN_WIDTH = 1920;
		static constexpr size_t SCREEN_HEIGHT = 1080;
		static constexpr SDL_Color HOOVER_COLOR = { 242,243,108,255 };

		static constexpr LabelsData& GetLabels() { return labelsData; }
		static constexpr TexturesData& GetTextures() { return texturesData; }
		static constexpr std::vector<SavePair>& GetSaveLabels() { return saveLabels; }
		static constexpr SDL_Color& NormalColor() noexcept { return normalColor; }
		static constexpr bool& KonamiCode() noexcept { return konamiCode; }
		static constexpr size_t& CenterX() noexcept { return centerX; }
		static constexpr size_t& CenterY() noexcept { return centerY; }
		static constexpr SDL_Window*& Window() noexcept { return window; }
		static constexpr SDL_Surface*& ScreenSurface() noexcept { return screenSurface; }
		static constexpr SDL_Renderer*& Renderer() noexcept { return renderer; }
		static constexpr TTF_Font*& Font() noexcept { return font; }
		static constexpr Mix_Music*& CodeMusic() noexcept { return codeMusic; }
		static constexpr std::vector<SDL_Keycode>& CodeKeys() noexcept { return codeKeys; }
		static constexpr std::vector<Mix_Music*>& Music() noexcept { return music; }
		static constexpr std::unique_ptr<Game>& CurrentGame() noexcept { return currentGame; }
	};
}