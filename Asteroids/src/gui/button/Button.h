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

	class Button : public Widget
	{
	public:
		Button(sf::RectangleShape shape, const sf::Font& font,
			   const ColorHolder& textColors, const ColorHolder& buttonColors);

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

	private:
		ColorHolder m_TextColors, m_ButtonColors;
		callback m_Callback;

		sf::RectangleShape m_Shape;
		sf::Text m_Text;

		bool m_IsToggle = false;
	};
}
