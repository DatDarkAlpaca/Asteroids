#include "pch.h"
#include "Button.h"

ast::gui::Button::Button(sf::RectangleShape shape, const sf::Font& font, const ColorHolder& textColors, const ColorHolder& buttonColors)
	: 
	m_Shape(shape), 
	m_TextColors(textColors), 
	m_ButtonColors(buttonColors), 
	m_Text("", font, 12)
{
	m_Shape.setFillColor(buttonColors.idleColor);

	sf::FloatRect bounds = shape.getLocalBounds();
	m_Text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

void ast::gui::Button::Select()
{
	Widget::Select();

	m_Shape.setFillColor(m_ButtonColors.pressedColor);
}

void ast::gui::Button::Deselect()
{
	Widget::Deselect();

	m_Shape.setFillColor(m_ButtonColors.idleColor);
}

void ast::gui::Button::Activate()
{
	Widget::Activate();

	if (m_IsToggle)
		m_Shape.setFillColor(m_ButtonColors.pressedColor);

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
			m_Shape.setFillColor(m_ButtonColors.selectedColor);
		else
			m_Shape.setFillColor(m_ButtonColors.idleColor);
	}
}

void ast::gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_Shape, states);
	target.draw(m_Text, states);
}
