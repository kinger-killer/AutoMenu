#pragma once

#include <memory>
#include <algorithm>
#include <utility>
#include <vector>

#include "Widgets/Widget.hpp"

class UI
{
public:
	void addWidget(std::unique_ptr<Widget> widget)
	{
		if (widget)
		{
			widgets.push_back(std::move(widget));
			sortWidgets();
		}
	}

	void update(const sf::Event& event, const sf::RenderWindow& window, sf::Time deltaTime)
	{
		for (const auto& widget : widgets)
			widget->update(event, window, deltaTime);

		// A widget may change its layer during update.
		sortWidgets();
	}

	void render(sf::RenderWindow& window)
	{
		sortWidgets();
		for (const auto& widget : widgets)
			widget->render(window);
	}

private:
	void sortWidgets()
	{
		std::stable_sort(widgets.begin(), widgets.end(),
			[](const auto& left, const auto& right)
			{
				return left->getRenderLayer() < right->getRenderLayer();
			});
	}

	std::vector<std::unique_ptr<Widget>> widgets;
};
