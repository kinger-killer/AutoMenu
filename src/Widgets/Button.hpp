#pragma once

#include <cstdint>
#include <algorithm>
#include <cmath>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "Widget.hpp"
#include "../Observer.hpp"

enum class ButtonEventType
{
	Pressed,
	Released,
	OnHoverBegin,
	OnHoverEnd
};

class Button;

struct ButtonEvent : Event
{
	ButtonEvent(Button& eventSource, ButtonEventType eventType, std::string eventButtonName)
		: source(eventSource), type(eventType), buttonName(std::move(eventButtonName))
	{
	}

	Button& source;
	ButtonEventType type;
	std::string buttonName;
};

class Button : public Widget, public ISubject
{
public:
	explicit Button(const sf::FloatRect& buttonBounds)
		: shape(buttonBounds.size)
	{
		shape.setPosition(buttonBounds.position);
	}

	virtual void setPosition(const sf::Vector2f& position)
	{
		shape.setPosition(position);
		onGeometryChanged();
	}

	virtual void setSize(const sf::Vector2f& size)
	{
		shape.setSize(size);
		onGeometryChanged();
	}

	void setFillColor(const sf::Color& color)
	{
		baseFillColor = color;
		applyInteractionColor();
	}

	void setHoverFillColor(const sf::Color& color)
	{
		hoverFillColor = color;
		applyInteractionColor();
	}

	// Positive values lighten the button; negative values darken it. Values are clamped to [-1, 1].
	void setHoverBrightness(float amount)
	{
		hoverBrightness = std::clamp(amount, -1.f, 1.f);
		applyInteractionColor();
	}

	void setPressedBrightness(float amount)
	{
		pressedBrightness = std::clamp(amount, -1.f, 1.f);
		applyInteractionColor();
	}

	void setName(std::string buttonName)
	{
		name = std::move(buttonName);
	}

	[[nodiscard]] const std::string& getName() const
	{
		return name;
	}

	void setOutline(const sf::Color& color, float thickness)
	{
		shape.setOutlineColor(color);
		shape.setOutlineThickness(thickness);
	}

	void render(sf::RenderWindow& window) override
	{
		window.draw(shape);
	}

	void update(const sf::Event& event, const sf::RenderWindow& window, sf::Time) override
	{
		if (const auto* move = event.getIf<sf::Event::MouseMoved>())
		{
			const auto mouse = window.mapPixelToCoords(move->position);
			updateHoverState(shape.getGlobalBounds().contains(mouse));
		}
		else if (event.is<sf::Event::MouseLeft>())
		{
			updateHoverState(false);
		}

		if (IsPressed(event, window))
		{
			isPressed = true;
			applyInteractionColor();
			emit(ButtonEventType::Pressed);
		}

		if (const auto* release = event.getIf<sf::Event::MouseButtonReleased>();
			release && release->button == sf::Mouse::Button::Left)
		{
			const bool releasedInside = IsReleased(event, window);
			if (isPressed)
			{
				isPressed = false;
				applyInteractionColor();
			}
			if (releasedInside)
				emit(ButtonEventType::Released);
		}
	}

protected:
	[[nodiscard]] bool isButtonHovered() const { return isHovered; }
	[[nodiscard]] bool isButtonPressed() const { return isPressed; }

	[[nodiscard]] sf::Vector2f getButtonPosition() const
	{
		return shape.getPosition();
	}

	[[nodiscard]] sf::Vector2f getButtonSize() const
	{
		return shape.getSize();
	}

	virtual void onGeometryChanged() {}

	bool IsPressed(const sf::Event& event, const sf::RenderWindow& window) const
	{
		const auto* click = event.getIf<sf::Event::MouseButtonPressed>();
		if (!click || click->button != sf::Mouse::Button::Left)
			return false;

		const auto mouse = window.mapPixelToCoords(click->position);
		return shape.getGlobalBounds().contains(mouse);
	}

	bool IsReleased(const sf::Event& event, const sf::RenderWindow& window) const
	{
		const auto* release = event.getIf<sf::Event::MouseButtonReleased>();
		if (!release || release->button != sf::Mouse::Button::Left)
			return false;

		const auto mouse = window.mapPixelToCoords(release->position);
		return shape.getGlobalBounds().contains(mouse);
	}

private:
	void updateHoverState(bool isMouseOver)
	{
		if (isHovered == isMouseOver)
			return;

		isHovered = isMouseOver;
		applyInteractionColor();
		emit(isHovered ? ButtonEventType::OnHoverBegin : ButtonEventType::OnHoverEnd);
	}

	protected:
	static sf::Color adjustBrightness(const sf::Color& color, float amount)
	{
		auto adjust = [amount](std::uint8_t channel)
		{
			const float value = amount >= 0.f
				? channel + (255.f - channel) * amount
				: channel * (1.f + amount);
			return static_cast<std::uint8_t>(std::lround(std::clamp(value, 0.f, 255.f)));
		};
		return { adjust(color.r), adjust(color.g), adjust(color.b), color.a };
	}

	private:
	void applyInteractionColor()
	{
		const sf::Color interactionBase = isHovered && hoverFillColor
			? *hoverFillColor : baseFillColor;
		const sf::Color color = isHovered
			? adjustBrightness(interactionBase, isPressed ? pressedBrightness : hoverBrightness)
			: baseFillColor;
		shape.setFillColor(color);
		onInteractionColorChanged(color, isHovered ? (isPressed ? pressedBrightness : hoverBrightness) : 0.f);
	}

	virtual void onInteractionColorChanged(const sf::Color&, float) {}

	void emit(ButtonEventType type)
	{
		ButtonEvent buttonEvent(*this, type, name);
		Emit(buttonEvent);
	}

	sf::RectangleShape shape;
	sf::Color baseFillColor{ sf::Color::White };
	std::optional<sf::Color> hoverFillColor;
	float hoverBrightness = 0.16f;
	float pressedBrightness = -0.18f;
	bool isHovered = false;
	bool isPressed = false;
	std::string name;
};

struct ButtonData
{
	explicit ButtonData(
		const sf::FloatRect& buttonBounds = sf::FloatRect({ 0.f, 0.f }, { 200.f, 80.f }),
		const sf::Color& buttonFillColor = sf::Color(80, 100, 160),
		const sf::Color& buttonOutlineColor = sf::Color::White,
		float buttonOutlineThickness = 3.f,
		std::uint8_t buttonRenderLayer = 0,
		std::string buttonName = {},
		float buttonHoverBrightness = 0.16f,
		float buttonPressedBrightness = -0.18f)
		: bounds(buttonBounds),
		  fillColor(buttonFillColor),
		  outlineColor(buttonOutlineColor),
		  outlineThickness(buttonOutlineThickness),
		  renderLayer(buttonRenderLayer),
		  name(std::move(buttonName)),
		  hoverBrightness(buttonHoverBrightness),
		  pressedBrightness(buttonPressedBrightness)
	{
	}

	sf::FloatRect bounds;
	sf::Color fillColor;
	sf::Color outlineColor;
	float outlineThickness;
	std::uint8_t renderLayer;
	std::string name;
	float hoverBrightness;
	float pressedBrightness;

	void applyStyleTo(Button& button) const
	{
		button.setFillColor(fillColor);
		button.setOutline(outlineColor, outlineThickness);
		button.setRenderLayer(renderLayer);
		button.setName(name);
		button.setHoverBrightness(hoverBrightness);
		button.setPressedBrightness(pressedBrightness);
	}
};

class ButtonFactory : public WidgetFactory<Button, ButtonData>
{
public:
	[[nodiscard]] std::unique_ptr<Button> create(const ButtonData& data) const override
	{
		auto button = std::make_unique<Button>(data.bounds);
		data.applyStyleTo(*button);
		return button;
	}
};

