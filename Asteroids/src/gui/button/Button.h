#pragma once
#include "gui/Widget.h"
#include "utils/Utilities.h"

namespace ast::gui
{
	using callback = std::function<void()>;

	struct ColorHolder
	{
		ColorHolder(const sf::Color& idleColor, const sf::Color& pressedColor, const sf::Color& selectedColor)
			: idleColor(idleColor), pressedColor(pressedColor), selectedColor(selectedColor) { }

		ColorHolder() = default;

		sf::Color idleColor, pressedColor, selectedColor;
	};

	inline ColorHolder standaloneText(sf::Color(255, 255, 255), sf::Color(50, 50, 50), sf::Color(150, 150, 150));
	inline ColorHolder noneHolder(sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0));
	inline ColorHolder redHolder(sf::Color(255, 0, 0), sf::Color(255, 0, 0), sf::Color(255, 0, 0));

	class Button : public Widget
	{
	public:
		Button(sf::RectangleShape shape, const sf::Font& font, unsigned int characterSize,
			   const ColorHolder& textColors, const ColorHolder& buttonColors, bool renderButton = true);

	public:
		inline void SetCallback(callback callback) { m_Callback = std::move(callback); }

		inline void SetText(const std::string& text)
		{
			m_Text.setString(text);
			CenterTextOrigin(m_Text);
		}

		inline void SetToggle(bool flag) { m_IsToggle = flag; }

	public:
		virtual inline void PollEvents(const sf::Event& event) override { }

		virtual inline bool IsSelectable() const override { return true; }

	public:
		virtual void Select() override;

		virtual void Deselect();

		virtual void Activate() override;

		virtual void Deactivate() override;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		inline sf::RectangleShape& shape() { return m_Shape; }

	private:
		ColorHolder m_TextColors, m_ButtonColors;
		callback m_Callback;

		sf::RectangleShape m_Shape;
		sf::Text m_Text;

		bool m_IsToggle = false, m_RenderButton = true;
	};
}
