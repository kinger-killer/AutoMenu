#pragma once 
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>

#include "../Observer.hpp"

template <typename T>
struct UiField : public Event 
{
	std::string_view key;
	T value;
};


class Widget
{
private:
	std::uint8_t renderLayer = 0;
	std::vector<std::shared_ptr<const void>> retainedResources;

public:
	virtual ~Widget() = default;

	// Keeps shared resources and runtime values alive while this widget uses them.
	template <typename T>
	void retainLifetime(std::shared_ptr<T> resource)
	{
		if (resource)
			retainedResources.emplace_back(std::move(resource));
	}
	
	virtual void setRenderLayer(std::uint8_t newLayer) { renderLayer = newLayer; }
	virtual std::uint8_t getRenderLayer() const { return renderLayer; }

	virtual void render(sf::RenderWindow& window) = 0;

	virtual void update(const sf::Event& event, const sf::RenderWindow& window,
		sf::Time deltaTime) = 0;
};

template <typename Product, typename Data>
class WidgetFactory
{
public:
	virtual ~WidgetFactory() = default;

	[[nodiscard]] virtual std::unique_ptr<Product> create(const Data& data) const = 0;
};

