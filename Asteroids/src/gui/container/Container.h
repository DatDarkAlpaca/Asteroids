#pragma once
#include "gui/Widget.h"

namespace ast::gui
{
	class Container : public Widget
	{
	public:
		Container() : m_Children(), m_SelectedChild(-1) { }

	public:
		void PollEvents(const sf::Event& event) override;

		inline bool IsSelectable() const { return false; }

	public:
		void Pack(std::shared_ptr<Widget> widget);

		void Select(int index);

		void SelectNext();

		void SelectPrevious();

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		inline bool HasSelection() const { return m_SelectedChild >= 0; }

	private:
		std::vector<std::shared_ptr<Widget>> m_Children;
		int m_SelectedChild;
	};
}

