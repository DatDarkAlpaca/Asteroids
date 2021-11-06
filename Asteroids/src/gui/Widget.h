#pragma once
#include "pch.h"

namespace ast::gui
{
	class Widget : public sf::Drawable, public sf::Transformable, public sf::NonCopyable
	{
	protected:
		Widget() { }

	public:
		virtual ~Widget() { }

	public:
		virtual void PollEvents(const sf::Event& event) = 0;

		virtual void Update(float dt) { }

		virtual bool IsSelectable() const = 0;

	public:
		inline bool IsSelected() const { return m_IsSelected;  }
		inline virtual void Select()   { m_IsSelected = true;  }
		inline virtual void Deselect() { m_IsSelected = false; }

	public:
		inline virtual bool IsActive()   { return m_IsActive;  }
		inline virtual void Activate()   { m_IsActive = true;  }
		inline virtual void Deactivate() { m_IsActive = false; }

	private:
		bool m_IsSelected = false, m_IsActive = false;
	};
}