/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#include "Keyboard.h"

namespace LOTSH::Events
{
	void Keyboard::HandleKey(const SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		{
			std::map<SDL_Keycode, bool>::iterator iterator = keyPressed.find(e.key.keysym.sym);
			if (iterator != keyPressed.end())
			{
				if (e.type == SDL_KEYDOWN)
					iterator->second = true;
				else
					iterator->second = false;
			}
		}
	}

	bool Keyboard::GetKeyState(SDL_Keycode code)
	{
		std::map<SDL_Keycode, bool>::iterator iterator = keyPressed.find(code);
		if (iterator != keyPressed.end())
			return iterator->second;
		else
			return false;
	}

	void Keyboard::SetKeyState(SDL_Keycode code, bool state)
	{
		std::map<SDL_Keycode, bool>::iterator iterator = keyPressed.find(code);
		if (iterator != keyPressed.end())
			iterator->second = state;
	}
}