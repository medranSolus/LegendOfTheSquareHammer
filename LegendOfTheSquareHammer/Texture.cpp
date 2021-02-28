/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#include "Data.h"

namespace LOTSH::Renders
{
	SDL_Surface* Texture::LoadSurface(const std::string& path)
	{
		SDL_Surface* temporarySurface = IMG_Load(path.c_str());
		if (temporarySurface == nullptr)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << path.c_str() << ": Cannot load image: " << IMG_GetError() << std::endl;
			fout.close();
			exit(0b10000000000000);
		}
		SDL_Surface* finalSurface = SDL_ConvertSurface(temporarySurface, ProgramData::Data::ScreenSurface()->format, NULL);
		if (finalSurface == nullptr)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "Cannot convert image: " << SDL_GetError() << std::endl;
			fout.close();
			return temporarySurface;
		}
		SDL_FreeSurface(temporarySurface);
		return finalSurface;
	}

	Texture::Texture(const std::string& newPath) : path(newPath)
	{
		SDL_Surface* surface = LoadSurface(path);
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 1, 1, 1));
		texture = SDL_CreateTextureFromSurface(ProgramData::Data::Renderer(), surface);
		if (texture == nullptr)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << path.c_str() << " SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
			fout.close();
			exit(0b1000000000000);
		}
		width = static_cast<int>(surface->w * ProgramData::Data::SCREEN_SCALE);
		height = static_cast<int>(surface->h * ProgramData::Data::SCREEN_SCALE);
		SDL_FreeSurface(surface);
	}

	Texture::Texture(Texture&& t) noexcept
		: texture(t.texture), textTexture(t.textTexture),
		text(std::move(t.text)), path(std::move(t.path)),
		textColor(std::move(t.textColor)), width(t.width),
		height(t.height), textWidth(t.textWidth), textHeight(t.textHeight)
	{
		t.texture = nullptr;
		t.textTexture = nullptr;
	}

	void Texture::SetColor(uint8_t r, uint8_t g, uint8_t b)
	{
		if (texture != nullptr)
			SDL_SetTextureColorMod(texture, r, g, b);
		if (textTexture != nullptr)
			SDL_SetTextureColorMod(textTexture, r, g, b);
	}

	void Texture::SetBlendMode(SDL_BlendMode blendMode)
	{
		if (texture != nullptr)
			SDL_SetTextureBlendMode(texture, blendMode);
		if (textTexture != nullptr)
			SDL_SetTextureBlendMode(textTexture, blendMode);
	}

	void Texture::SetAlpha(Uint8 alpha)
	{
		if (texture != nullptr)
			SDL_SetTextureAlphaMod(texture, alpha);
		if (textTexture != nullptr)
			SDL_SetTextureAlphaMod(textTexture, alpha);
	}

	bool Texture::LoadFile(const std::string& newPath)
	{
		if (newPath != "noPath")
		{
			Free(true);
			path = newPath;
			SDL_Surface* surface = LoadSurface(path);
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 1, 1, 1));
			texture = SDL_CreateTextureFromSurface(ProgramData::Data::Renderer(), surface);
			if (texture == nullptr)
			{
				std::ofstream fout("error.txt", std::ios_base::app);
				fout << path.c_str() << " SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
				fout.close();
				return false;
			}
			width = static_cast<int>(surface->w * ProgramData::Data::SCREEN_SCALE);
			height = static_cast<int>(surface->h * ProgramData::Data::SCREEN_SCALE);
			SDL_FreeSurface(surface);
		}
		return true;
	}

	bool Texture::LoadText(const std::string& newText, const SDL_Color& color)
	{
		Free(false);
		SDL_Surface* surface = TTF_RenderText_Solid(ProgramData::Data::Font(), newText.c_str(), color);
		if (surface == nullptr)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << newText.c_str() << " TTF_RenderText_Solid error: " << SDL_GetError() << std::endl;
			fout.close();
			return false;
		}
		textTexture = SDL_CreateTextureFromSurface(ProgramData::Data::Renderer(), surface);
		if (textTexture == nullptr)
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << newText.c_str() << " SDL_CreateTextureFromSurface Text error: " << SDL_GetError() << std::endl;
			fout.close();
			return false;
		}
		text = newText;
		textColor = color;
		if (texture != nullptr)
		{
			textWidth = width - abs(width - surface->w);
			textHeight = static_cast<int>(height * 0.6);
		}
		else
		{
			textWidth = width = static_cast<int>(surface->w * ProgramData::Data::SCREEN_SCALE);
			textHeight = height = static_cast<int>(surface->h * ProgramData::Data::SCREEN_SCALE);
		}
		SDL_FreeSurface(surface);
		return true;
	}

	void Texture::Free(bool option)
	{
		if (option && texture != nullptr)
		{
			SDL_DestroyTexture(texture);
			texture = nullptr;
			width = 0;
			height = 0;
		}
		if (textTexture != nullptr)
		{
			SDL_DestroyTexture(textTexture);
			textTexture = nullptr;
			textWidth = 0;
			textHeight = 0;
		}
	}

	void Texture::Render(SDL_RendererFlip flip)
	{
		if (texture != nullptr)
			SDL_RenderCopyEx(ProgramData::Data::Renderer(), texture, nullptr, nullptr, 0.0, nullptr, flip);
		if (textTexture != nullptr)
			SDL_RenderCopy(ProgramData::Data::Renderer(), textTexture, nullptr, nullptr);
	}

	void Texture::Render(int x, int y, SDL_RendererFlip flip)
	{
		if (texture != nullptr)
		{
			SDL_Rect renderQuad = { x, y, width, height };
			SDL_RenderCopyEx(ProgramData::Data::Renderer(), texture, nullptr, &renderQuad, 0.0, nullptr, flip);
		}
		if (textTexture != nullptr)
		{
			SDL_Rect renderQuad = { x + width / 5, y + height / 5, textWidth, textHeight };
			SDL_RenderCopy(ProgramData::Data::Renderer(), textTexture, nullptr, &renderQuad);
		}
	}

	void Texture::Render(SDL_Rect coord, SDL_RendererFlip flip)
	{
		if (texture != nullptr)
		{
			SDL_RenderCopyEx(ProgramData::Data::Renderer(), texture, nullptr, &coord, 0.0, nullptr, flip);
		}
		if (textTexture != nullptr)
		{
			SDL_Rect renderQuad = { coord.x + coord.w / 5, coord.y + coord.h / 5, static_cast<int>(0.6 * coord.w), static_cast<int>(0.6 * coord.h) };
			SDL_RenderCopy(ProgramData::Data::Renderer(), textTexture, nullptr, &renderQuad);
		}
	}
	
	std::ofstream& operator<<(std::ofstream& fout, const Texture& texture)
	{
		fout << texture.path.c_str() << std::endl
			<< texture.width << ' '
			<< texture.height << std::endl
			<< texture.text;
		if (texture.text != "none")
		{
			fout << std::endl << texture.textWidth << ' '
				<< texture.textHeight << std::endl
				<< texture.textColor.r << ' '
				<< texture.textColor.g << ' '
				<< texture.textColor.b << ' '
				<< texture.textColor.a;
		}
		return fout;
	}
	
	std::ifstream& operator>>(std::ifstream& fin, Texture& texture)
	{
		if (fin.peek() == 10)
			fin.ignore();
		getline(fin, texture.path);
		fin >> texture.width >> texture.height;
		fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		getline(fin, texture.text);
		if (texture.path != "noPath" && !texture.LoadFile(texture.path))
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << " :Texture load failed\n";
			fout.close();
		}
		if (texture.text != "none")
		{
			fin >> texture.textWidth
				>> texture.textHeight
				>> texture.textColor.r
				>> texture.textColor.g
				>> texture.textColor.b
				>> texture.textColor.a;
			if (!texture.LoadText(texture.text, texture.textColor))
			{
				std::ofstream fout("error.txt", std::ios_base::app);
				fout << " :Text load failed\n";
				fout.close();
			}
		}
		return fin;
	}
}