#include "pch.h"
#include "Button.h"

ast::gui::Button::Button(sf::RectangleShape shape, const sf::Font& font, unsigned int characterSize, const ColorHolder& textColors, const ColorHolder& buttonColors, bool renderButton)
	: 
	m_Shape(shape), 
	m_TextColors(textColors), 
	m_ButtonColors(buttonColors), 
	m_Text("", font, characterSize),
	m_RenderButton(renderButton)
{
	m_Shape.setFillColor(buttonColors.idleColor);
	m_Text.setFillColor(textColors.idleColor);

	sf::FloatRect bounds = shape.getLocalBounds();
	m_Text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

void ast::gui::Button::Select()
{
	Widget::Select();

	m_Shape.setFillColor(m_ButtonColors.selectedColor);
	m_Text.setFillColor(m_TextColors.selectedColor);
}

void ast::gui::Button::Deselect()
{
	Widget::Deselect();

	m_Shape.setFillColor(m_ButtonColors.idleColor);
	m_Text.setFillColor(m_TextColors.idleColor);
}

void ast::gui::Button::Activate()
{
	Widget::Activate();

	if (m_IsToggle)
	{
		m_Shape.setFillColor(m_ButtonColors.pressedColor);
		m_Text.setFillColor(m_TextColors.pressedColor);
	}

	if (m_Callback)
		m_Callback();

	if (!m_IsToggle)
		Deactivate();
}

void ast::gui::Button::Deactivate()
{
	Widget::Deactivate();

	if (m_IsToggle)
	{
		if (IsSelected())
		{
			m_Shape.setFillColor(m_ButtonColors.selectedColor);
			m_Text.setFillColor(m_TextColors.selectedColor);
		}
		else
		{
			m_Shape.setFillColor(m_ButtonColors.idleColor);
			m_Text.setFillColor(m_TextColors.idleColor);
		}
	}
}

void ast::gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if(m_RenderButton)
		target.draw(m_Shape, states);

	target.draw(m_Text, states);
}
