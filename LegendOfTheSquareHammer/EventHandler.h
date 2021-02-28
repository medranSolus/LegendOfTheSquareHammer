/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#pragma once
#include "Keyboard.h"

namespace LOTSH::Events
{
	class EventHandler
	{
		static inline Keyboard keyboard;

	public:
		EventHandler() = delete;

		static constexpr Keyboard& GetKeyboard() { return keyboard; }
	};
}