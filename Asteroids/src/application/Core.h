#pragma once
#include "pch.h"

namespace ast
{
	// Todo: create a resource codex/manager/holder in the future if needed.
	struct FontHolder
	{
		std::shared_ptr<sf::Font> font;
	} inline fontHolder;

	extern unsigned int WorldWidth, WorldHeight;
	extern sf::String WindowTitle;
}