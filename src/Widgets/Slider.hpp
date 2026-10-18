#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <type_traits>

#include "Widget.hpp"

template <typename Value = float>
class Slider : public Widget
{
	static_assert(std::is_arithmetic_v<Value>, "Slider values must be numeric");

public:
	// The value and both textures must outlive this slider.
	Slider(const sf::FloatRect& bounds, Value& value, Value minimum, Value maximum,
		const sf::Texture& trackTexture, const sf::Texture& knobTexture)
		: bounds(bounds), value(value), minimum(minimum), maximum(maximum),
		  track(trackTexture), knob(knobTexture)
	{
		fitSpritesToBounds();
		updateKnobPosition();
	}

	void setTrackColor(const sf::Color& color)
	{
		track.setColor(color);
	}

	void setKnobColor(const sf::Color& color)
	{
		knob.setColor(color);
	}

	void render(sf::RenderWindow& window) override
	{
		updateKnobPosition();
		window.draw(track);
		window.draw(knob);
	}

	void update(const sf::Event& event, const sf::RenderWindow& window, sf::Time) override
	{
		if (const auto* press = event.getIf<sf::Event::MouseButtonPressed>())
		{
			if (press->button != sf::Mouse::Button::Left)
				return;

			const auto mouse = window.mapPixelToCoords(press->position);
			if (bounds.contains(mouse))
			{
				isDragging = true;
				setValueFromMouse(mouse.x);
			}
		}
		else if (const auto* move = event.getIf<sf::Event::MouseMoved>())
		{
			if (isDragging)
				setValueFromMouse(window.mapPixelToCoords(move->position).x);
		}
		else if (const auto* release = event.getIf<sf::Event::MouseButtonReleased>())
		{
			if (release->button == sf::Mouse::Button::Left)
				isDragging = false;
		}
		else if (event.is<sf::Event::FocusLost>())
		{
			isDragging = false;
		}
	}

private:
	[[nodiscard]] double getRatio() const
	{
		const double minValue = static_cast<double>(minimum);
		const double maxValue = static_cast<double>(maximum);
		if (maxValue <= minValue)
			return 0.0;

		return std::clamp((static_cast<double>(value) - minValue) / (maxValue - minValue), 0.0, 1.0);
	}

	void setValueFromMouse(float mouseX)
	{
		if (maximum <= minimum || bounds.size.x <= 0.f)
			return;

		const double ratio = std::clamp(
			static_cast<double>((mouseX - bounds.position.x) / bounds.size.x), 0.0, 1.0);
		const double minValue = static_cast<double>(minimum);
		const double newValue = minValue +
			(static_cast<double>(maximum) - minValue) * ratio;

		if constexpr (std::is_integral_v<Value>)
			value = static_cast<Value>(std::round(newValue));
		else
			value = static_cast<Value>(newValue);

		updateKnobPosition();
	}

	void updateKnobPosition()
	{
		const float x = bounds.position.x + bounds.size.x * static_cast<float>(getRatio());
		const float y = bounds.position.y + bounds.size.y / 2.f;
		knob.setPosition({ x, y });
	}

	void fitSpritesToBounds()
	{
		const auto trackBounds = track.getLocalBounds();
		if (trackBounds.size.x > 0.f && trackBounds.size.y > 0.f)
		{
			const float trackHeight = std::max(4.f, bounds.size.y * 0.18f);
			track.setScale({ bounds.size.x / trackBounds.size.x, trackHeight / trackBounds.size.y });
			track.setPosition({ bounds.position.x,
				bounds.position.y + (bounds.size.y - trackHeight) / 2.f });
		}

		const auto knobBounds = knob.getLocalBounds();
		if (knobBounds.size.x > 0.f && knobBounds.size.y > 0.f)
		{
			knob.setOrigin({ knobBounds.position.x + knobBounds.size.x / 2.f,
				knobBounds.position.y + knobBounds.size.y / 2.f });
			knob.setScale({ bounds.size.y / knobBounds.size.x,
				bounds.size.y / knobBounds.size.y });
		}
	}

	sf::FloatRect bounds;
	Value& value;
	Value minimum;
	Value maximum;
	sf::Sprite track;
	sf::Sprite knob;
	bool isDragging = false;
};

template <typename Value = float>
struct SliderData
{
	// The referenced value and textures must outlive the created slider.
	SliderData(const sf::FloatRect& sliderBounds, Value& sliderValue,
		Value minimumValue, Value maximumValue,
		const sf::Texture& sliderTrackTexture, const sf::Texture& sliderKnobTexture,
		const sf::Color& sliderTrackColor = sf::Color(70, 70, 70),
		const sf::Color& sliderKnobColor = sf::Color(100, 160, 240),
		std::uint8_t sliderRenderLayer = 0)
		: bounds(sliderBounds), value(sliderValue), minimum(minimumValue), maximum(maximumValue),
		  trackTexture(sliderTrackTexture), knobTexture(sliderKnobTexture),
		  trackColor(sliderTrackColor), knobColor(sliderKnobColor),
		  renderLayer(sliderRenderLayer)
	{
	}

	sf::FloatRect bounds;
	Value& value;
	Value minimum;
	Value maximum;
	const sf::Texture& trackTexture;
	const sf::Texture& knobTexture;
	sf::Color trackColor;
	sf::Color knobColor;
	std::uint8_t renderLayer;

	void applyStyleTo(Slider<Value>& slider) const
	{
		slider.setTrackColor(trackColor);
		slider.setKnobColor(knobColor);
		slider.setRenderLayer(renderLayer);
	}
};

template <typename Value = float>
class SliderFactory : public WidgetFactory<Slider<Value>, SliderData<Value>>
{
public:
	[[nodiscard]] std::unique_ptr<Slider<Value>> create(const SliderData<Value>& data) const override
	{
		auto slider = std::make_unique<Slider<Value>>(
			data.bounds, data.value, data.minimum, data.maximum,
			data.trackTexture, data.knobTexture);
		data.applyStyleTo(*slider);
		return slider;
	}
};
