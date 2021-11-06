#pragma once
#include "gui/Widget.h"

namespace ast::gui
{
	class Label : public Widget
	{
	public:
		Label(const std::string& text, const sf::Font& font, unsigned int characterSize = 12)
			: m_Text(text, font, characterSize) { }

	public:
		inline void PollEvents(const sf::Event&) { }

	public:
		inline void SetText(const std::string& text) { m_Text.setString(text); }
		inline sf::Text& getText() { return m_Text; }

	public:
		virtual inline bool IsSelectable() const { return false; }

	private:
		inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			states.transform *= getTransform();
			target.draw(m_Text, states);
		}

	private:
		sf::Text m_Text;
	};
}


