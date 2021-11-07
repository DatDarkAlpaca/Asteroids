#include "pch.h"
#include "Container.h"

void ast::gui::Container::PollEvents(const sf::Event& event)
{
	if (HasSelection() && m_Children[m_SelectedChild]->IsActive())
		m_Children[m_SelectedChild]->PollEvents(event);

	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
			SelectPrevious();

		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			SelectNext();

		else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
		{
			if (HasSelection())
				m_Children[m_SelectedChild]->Activate();
		}
	}
}

void ast::gui::Container::Update(float dt)
{
	for (auto& child : m_Children)
		child->Update(dt);
}

void ast::gui::Container::Pack(std::shared_ptr<Widget> widget)
{
	m_Children.push_back(widget);

	if (!HasSelection() && widget->IsSelectable())
		Select(m_Children.size() - 1);
}

void ast::gui::Container::Select(int index)
{
	if (m_Children[index]->IsSelectable())
	{
		if (HasSelection())
			m_Children[m_SelectedChild]->Deselect();

		m_Children[index]->Select();
		m_SelectedChild = index;
	}
}

void ast::gui::Container::SelectNext()
{
	if (!HasSelection())
		return;

	int next = m_SelectedChild;
	do
		next = (next + 1) % m_Children.size();
	while (!m_Children[next]->IsSelectable());

	Select(next);
}

void ast::gui::Container::SelectPrevious()
{
	if (!HasSelection())
		return;

	int previous = m_SelectedChild;
	do
		previous = (previous + m_Children.size() - 1) % m_Children.size();
	while (!m_Children[previous]->IsSelectable());

	Select(previous);
}

void ast::gui::Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (auto& children : m_Children)
	{
		target.draw(*children, states);
	}
}
