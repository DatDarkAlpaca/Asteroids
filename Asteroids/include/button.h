#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace game
{
	enum class ButtonStates { Idle = 0, Hover, Pressed };
}

class Button
{
public:
	Button() = default;

	Button(const sf::Vector2f& position, const sf::Vector2f& size, sf::Font* font, std::string&& label, bool rect = true)
		: m_rect(rect)
	{
		m_shape.setPosition({ position.x, position.y });
		m_shape.setFillColor(m_idleColor);
		m_shape.setSize({ size.x, size.y });
		
		m_font = font;
		m_text.setFont(*m_font);

		m_text.setString(label);
		if(rect)
			m_text.setFillColor(sf::Color::Black);
		else
			m_text.setFillColor(sf::Color::White);

		m_text.setCharacterSize(24);

		if (rect)
		{
			m_text.setPosition({
				m_shape.getPosition().x + (m_shape.getGlobalBounds().width / 2.f) - m_text.getGlobalBounds().width / 2.f,
				m_shape.getPosition().y + (m_shape.getGlobalBounds().height / 2.f) - m_text.getGlobalBounds().height / 2.f - 5,
				});
		}
		else
			m_text.setPosition(position);
	}

	~Button()
	{
		delete m_font;
	}
	
public:
	void update(sf::Vector2i mousePosition) 
	{
		m_buttonState = game::ButtonStates::Idle;

		// Getting the mouse bounds:
		if (m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_buttonState != game::ButtonStates::Pressed)
				m_buttonState = game::ButtonStates::Pressed;
			else
				m_buttonState = game::ButtonStates::Hover;
		}

		// Changing the button color:
		if (m_rect)
		{
			switch (m_buttonState)
			{
			case game::ButtonStates::Idle:
				m_shape.setFillColor(m_idleColor);
				break;
			case game::ButtonStates::Hover:
				m_shape.setFillColor(m_hoverColor);
				break;
			case game::ButtonStates::Pressed:
				m_shape.setFillColor(m_pressedColor);
				break;
			default:
				m_shape.setFillColor(sf::Color::Red);
				break;
			}
		}
		else
		{
			switch (m_buttonState)
			{
			case game::ButtonStates::Idle:
				m_text.setFillColor(m_idleColor);
				break;
			case game::ButtonStates::Hover:
				m_text.setFillColor(m_hoverColor);
				break;
			case game::ButtonStates::Pressed:
				m_text.setFillColor(sf::Color::Red);
				break;
			}
		}
		
	}

	void render(sf::RenderTarget& target) 
	{
		if (m_rect)
		{
			target.draw(m_shape);
		}
		target.draw(m_text);
	}

public:
	bool isPressed() const { return m_buttonState == game::ButtonStates::Pressed; }

	bool isHovered() const { return m_buttonState == game::ButtonStates::Hover; }
	
	bool isIdle() const { return m_buttonState == game::ButtonStates::Idle; }

private:
	sf::RectangleShape m_shape;
	sf::Font* m_font = nullptr;
	sf::Text m_text;

	sf::Color m_idleColor = sf::Color::White, 
		      m_hoverColor = sf::Color(200, 200, 200), 
		      m_pressedColor = sf::Color(100, 100, 100);

	bool m_rect;

private:
	game::ButtonStates m_buttonState = game::ButtonStates::Idle;
};