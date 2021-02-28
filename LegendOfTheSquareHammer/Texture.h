/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <fstream>
#undef max

namespace LOTSH::Renders
{
	class Texture
	{
		SDL_Texture* texture = nullptr;
		SDL_Texture* textTexture = nullptr;
		std::string text = "none";
		std::string path = "noPath";
		SDL_Color textColor = { 0,0,0,0 };
		int width = 0;
		int height = 0;
		int textWidth = 0;
		int textHeight = 0;

		static SDL_Surface* LoadSurface(const std::string& path);

	public:
		Texture() = default;
		Texture(const std::string& newPath);
		Texture(const Texture& t) = default;
		Texture(Texture&& t) noexcept;
		inline ~Texture() { Free(true); }

		constexpr int Width() const { return width; }
		constexpr int Height() const { return height; }
		constexpr bool IsText() const { return textTexture != nullptr; }
		constexpr const std::string& Text() const { return text; }
		constexpr const SDL_Color& TextColor() const { return textColor; }
		inline void SetColor(const SDL_Color& color) { SetColor(color.r, color.g, color.b); }
		inline void Render(int x, int y, int w, int h, SDL_RendererFlip flip = SDL_FLIP_NONE) { Render({ x, y, w, h }, flip); }

		//Ustawia pow³okê koloru na teksturze
		void SetColor(uint8_t r, uint8_t g, uint8_t b);
		void SetBlendMode(SDL_BlendMode blendMode);
		void SetAlpha(Uint8 alpha);
		bool LoadFile(const std::string& newPath);
		bool LoadText(const std::string& newText, const SDL_Color& color);
		// Free texture resources. Pass false to delete only text texture
		void Free(bool option);
		void Render(SDL_RendererFlip flip = SDL_FLIP_NONE);
		void Render(int x, int y, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void Render(SDL_Rect coord, SDL_RendererFlip flip = SDL_FLIP_NONE);

		friend std::ofstream& operator<<(std::ofstream& fout, const Texture& texture);
		friend std::ifstream& operator>>(std::ifstream& fin, Texture& texture);
	};
}