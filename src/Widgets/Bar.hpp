#pragma once

#include <algorithm>
#include <type_traits>

#include "Widget.hpp"

template <typename Value = float>
class Bar : public Widget
{
	static_assert(std::is_arithmetic_v<Value>, "Bar values must be numeric");

public:
	// Both values are observed by reference and must outlive this bar.
	Bar(const sf::FloatRect& bounds, const Value& value, const Value& maximum)
		: value(value), maximum(maximum), background(bounds.size), fill(bounds.size),
		  displayedRatio(getRatio())
	{
		background.setPosition(bounds.position);
		fill.setPosition(bounds.position);
	}

	void setPosition(const sf::Vector2f& position)
	{
		background.setPosition(position);
		fill.setPosition(position);
	}

	void setSize(const sf::Vector2f& size)
	{
		background.setSize(size);
		fill.setSize(size);
	}

	void setBackgroundColor(const sf::Color& color)
	{
		background.setFillColor(color);
	}

	void setFillColor(const sf::Color& color)
	{
		fill.setFillColor(color);
	}

	// Speed is measured in the fraction of the bar traversed per second.
	void setAnimationSpeed(float fractionPerSecond)
	{
		animationSpeed = std::max(0.f, fractionPerSecond);
	}

	void setOutline(const sf::Color& color, float thickness)
	{
		background.setOutlineColor(color);
		background.setOutlineThickness(thickness);
	}

	void render(sf::RenderWindow& window) override
	{
		fill.setSize({ background.getSize().x * displayedRatio, background.getSize().y });
		window.draw(background);
		window.draw(fill);
	}

	void update(const sf::Event&, const sf::RenderWindow&, sf::Time deltaTime) override
	{
		const float targetRatio = getRatio();
		const float step = animationSpeed * std::max(0.f, deltaTime.asSeconds());

		if (displayedRatio < targetRatio)
			displayedRatio = std::min(displayedRatio + step, targetRatio);
		else
			displayedRatio = std::max(displayedRatio - step, targetRatio);
	}

private:
	[[nodiscard]] float getRatio() const
	{
		const double maxValue = static_cast<double>(maximum);
		if (maxValue <= 0.0)
			return 0.f;

		const double currentValue = static_cast<double>(value);
		return static_cast<float>(std::clamp(currentValue / maxValue, 0.0, 1.0));
	}

	const Value& value;
	const Value& maximum;
	sf::RectangleShape background;
	sf::RectangleShape fill;
	float displayedRatio = 0.f;
	float animationSpeed = 0.5f;
};

template <typename Value = float>
struct BarData
{
	// The referenced values must outlive both this data object and the created bar.
	BarData(const sf::FloatRect& barBounds, const Value& currentValue, const Value& maxValue,
		const sf::Color& background = sf::Color(70, 70, 70),
		const sf::Color& fill = sf::Color(70, 200, 100),
		const sf::Color& outline = sf::Color::White, float outlineThickness = 0.f,
		std::uint8_t renderLayer = 0, float animationSpeed = 0.5f)
		: bounds(barBounds), value(currentValue), maximum(maxValue),
		  backgroundColor(background), fillColor(fill), outlineColor(outline),
		  outlineThickness(outlineThickness), renderLayer(renderLayer),
		  animationSpeed(animationSpeed)
	{
	}

	sf::FloatRect bounds;
	const Value& value;
	const Value& maximum;
	sf::Color backgroundColor;
	sf::Color fillColor;
	sf::Color outlineColor;
	float outlineThickness;
	std::uint8_t renderLayer;
	float animationSpeed;

	void applyStyleTo(Bar<Value>& bar) const
	{
		bar.setBackgroundColor(backgroundColor);
		bar.setFillColor(fillColor);
		bar.setOutline(outlineColor, outlineThickness);
		bar.setRenderLayer(renderLayer);
		bar.setAnimationSpeed(animationSpeed);
	}
};

template <typename Value = float>
class BarFactory : public WidgetFactory<Bar<Value>, BarData<Value>>
{
public:
	[[nodiscard]] std::unique_ptr<Bar<Value>> create(const BarData<Value>& data) const override
	{
		auto bar = std::make_unique<Bar<Value>>(data.bounds, data.value, data.maximum);
		data.applyStyleTo(*bar);
		return bar;
	}
};
