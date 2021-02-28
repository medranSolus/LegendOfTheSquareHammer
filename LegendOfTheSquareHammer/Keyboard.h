/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include <SDL.h>
#include <map>

namespace LOTSH::Events
{
	class Keyboard
	{
		std::map<SDL_Keycode, bool> keyPressed{ { SDLK_ESCAPE, false },{ SDLK_UP, false },{ SDLK_DOWN, false },{ SDLK_LEFT, false },{ SDLK_RIGHT, false },{ SDLK_RETURN, false },
		{ SDLK_w, false },{ SDLK_s, false },{ SDLK_a, false },{ SDLK_d, false },{ SDLK_q, false },{ SDLK_e, false },{ SDLK_r, false },{ SDLK_t, false },{ SDLK_y, false },
		{ SDLK_u, false },{ SDLK_i, false },{ SDLK_o, false },{ SDLK_p, false },{ SDLK_f, false },{ SDLK_g, false },{ SDLK_h, false },{ SDLK_j, false },{ SDLK_k, false },
		{ SDLK_l, false },{ SDLK_z, false },{ SDLK_x, false },{ SDLK_c, false },{ SDLK_v, false },{ SDLK_b, false },{ SDLK_n, false },{ SDLK_m, false },{ SDLK_1, false },
		{ SDLK_2, false },{ SDLK_3, false },{ SDLK_4, false },{ SDLK_5, false },{ SDLK_6, false },{ SDLK_7, false },{ SDLK_8, false },{ SDLK_9, false },{ SDLK_0, false },
		{ SDLK_TAB, false },{ SDLK_BACKSPACE, false },{ SDLK_SPACE, false } };

	public:
		void HandleKey(const SDL_Event& e);
		bool GetKeyState(SDL_Keycode code);
		void SetKeyState(SDL_Keycode code, bool state);
	};
}