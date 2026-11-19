#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include "Widget.hpp"

namespace AutoMenu::widgets
{
template <typename Value = float>
class Bar : public Widget
{
	static_assert(std::is_arithmetic_v<Value>, "Bar values must be numeric");

public:
	// The reference-based overload requires both values to outlive the bar.
	Bar(const sf::FloatRect& bounds, const Value& value, const Value& maximum)
		: value(&value), maximum(&maximum), background(bounds.size), fill(bounds.size),
		  indicator(bounds.size), indicatorRatio(getRatio()), displayedRatio(indicatorRatio)
	{
		background.setPosition(bounds.position);
		fill.setPosition(bounds.position);
		indicator.setPosition(bounds.position);
	}

	// Shared values let the bar keep the data it observes alive for its lifetime.
	Bar(const sf::FloatRect& bounds, std::shared_ptr<const Value> value,
		std::shared_ptr<const Value> maximum)
		: value(value.get()), maximum(maximum.get()), valueOwner(std::move(value)),
		  maximumOwner(std::move(maximum)), background(bounds.size), fill(bounds.size),
		  indicator(bounds.size)
	{
		if (!valueOwner || !maximumOwner)
			throw std::invalid_argument("Bar requires non-null shared values");
		indicatorRatio = getRatio();
		displayedRatio = indicatorRatio;
		background.setPosition(bounds.position);
		fill.setPosition(bounds.position);
		indicator.setPosition(bounds.position);
	}

	void setPosition(const sf::Vector2f& position)
	{
		background.setPosition(position);
		fill.setPosition(position);
		indicator.setPosition(position);
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

	void setIndicatorColor(const sf::Color& color)
	{
		indicator.setFillColor(color);
	}

	// Speed is measured in the fraction of the bar traversed per second.
	void setAnimationSpeed(float fractionPerSecond)
	{
		animationSpeed = std::max(0.f, fractionPerSecond);
	}

	// Controls how quickly the main fill responds to changes in the bound value.
	void setResponseSpeed(float fractionPerSecond)
	{
		responseSpeed = std::max(0.f, fractionPerSecond);
	}

	void setOutline(const sf::Color& color, float thickness)
	{
		background.setOutlineColor(color);
		background.setOutlineThickness(thickness);
	}

	void render(sf::RenderWindow& window) override
	{
		indicator.setSize({ background.getSize().x * indicatorRatio, background.getSize().y });
		fill.setSize({ background.getSize().x * std::min(displayedRatio, indicatorRatio),
			background.getSize().y });
		window.draw(background);
		window.draw(indicator);
		window.draw(fill);
	}

	void update(const sf::Event&, const sf::RenderWindow&, sf::Time deltaTime) override
	{
		const float targetRatio = getRatio();
		const float deltaSeconds = std::max(0.f, deltaTime.asSeconds());
		const float indicatorStep = responseSpeed * deltaSeconds;
		const float mainFillStep = animationSpeed * deltaSeconds;

		indicatorRatio = approach(indicatorRatio, targetRatio, indicatorStep);
		displayedRatio = approach(displayedRatio, indicatorRatio, mainFillStep);
	}

private:
	[[nodiscard]] static float approach(float current, float target, float step)
	{
		if (current < target)
			return std::min(current + step, target);
		return std::max(current - step, target);
	}

	[[nodiscard]] float getRatio() const
	{
		const double maxValue = static_cast<double>(*maximum);
		if (maxValue <= 0.0)
			return 0.f;

		const double currentValue = static_cast<double>(*value);
		return static_cast<float>(std::clamp(currentValue / maxValue, 0.0, 1.0));
	}

	const Value* value;
	const Value* maximum;
	std::shared_ptr<const Value> valueOwner;
	std::shared_ptr<const Value> maximumOwner;
	sf::RectangleShape background;
	sf::RectangleShape fill;
	sf::RectangleShape indicator;
	float indicatorRatio = 0.f;
	float displayedRatio = 0.f;
	float animationSpeed = 0.5f;
	float responseSpeed = 3.f;
};

template <typename Value = float>
struct BarData
{
	// Values passed by reference must outlive the created bar. shared_ptr overloads own them.
	BarData(const sf::FloatRect& barBounds, const Value& currentValue, const Value& maxValue,
		const sf::Color& background = sf::Color(70, 70, 70),
		const sf::Color& fill = sf::Color(70, 200, 100),
		const sf::Color& outline = sf::Color::White, float outlineThickness = 0.f,
		std::uint8_t renderLayer = 0, float animationSpeed = 0.5f,
		const sf::Color& indicator = sf::Color(230, 155, 55), float responseSpeed = 3.f)
		: bounds(barBounds), value(currentValue), maximum(maxValue),
		  backgroundColor(background), fillColor(fill), outlineColor(outline),
		  outlineThickness(outlineThickness), renderLayer(renderLayer),
		  animationSpeed(animationSpeed), indicatorColor(indicator), responseSpeed(responseSpeed)
	{
	}

	BarData(const sf::FloatRect& barBounds, std::shared_ptr<const Value> currentValue,
		std::shared_ptr<const Value> maxValue,
		const sf::Color& background = sf::Color(70, 70, 70),
		const sf::Color& fill = sf::Color(70, 200, 100),
		const sf::Color& outline = sf::Color::White, float outlineThickness = 0.f,
		std::uint8_t renderLayer = 0, float animationSpeed = 0.5f,
		const sf::Color& indicator = sf::Color(230, 155, 55), float responseSpeed = 3.f)
		: bounds(barBounds), value(currentValue.get()), maximum(maxValue.get()),
		  valueOwner(std::move(currentValue)), maximumOwner(std::move(maxValue)),
		  backgroundColor(background), fillColor(fill), outlineColor(outline),
		  outlineThickness(outlineThickness), renderLayer(renderLayer),
		  animationSpeed(animationSpeed), indicatorColor(indicator), responseSpeed(responseSpeed)
	{
		if (!valueOwner || !maximumOwner)
			throw std::invalid_argument("BarData requires non-null shared values");
	}

	sf::FloatRect bounds;
	const Value* value;
	const Value* maximum;
	std::shared_ptr<const Value> valueOwner;
	std::shared_ptr<const Value> maximumOwner;
	sf::Color backgroundColor;
	sf::Color fillColor;
	sf::Color outlineColor;
	float outlineThickness;
	std::uint8_t renderLayer;
	float animationSpeed;
	sf::Color indicatorColor;
	float responseSpeed;

	void applyStyleTo(Bar<Value>& bar) const
	{
		bar.setBackgroundColor(backgroundColor);
		bar.setFillColor(fillColor);
		bar.setIndicatorColor(indicatorColor);
		bar.setOutline(outlineColor, outlineThickness);
		bar.setRenderLayer(renderLayer);
		bar.setAnimationSpeed(animationSpeed);
		bar.setResponseSpeed(responseSpeed);
	}
};

template <typename Value = float>
class BarFactory : public WidgetFactory<Bar<Value>, BarData<Value>>
{
public:
	[[nodiscard]] std::unique_ptr<Bar<Value>> create(const BarData<Value>& data) const override
	{
		std::unique_ptr<Bar<Value>> bar;
		if (data.valueOwner && data.maximumOwner)
			bar = std::make_unique<Bar<Value>>(data.bounds, data.valueOwner, data.maximumOwner);
		else
			bar = std::make_unique<Bar<Value>>(data.bounds, *data.value, *data.maximum);
		data.applyStyleTo(*bar);
		return bar;
	}
};

} // namespace AutoMenu::widgets
