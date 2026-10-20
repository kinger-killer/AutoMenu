#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

#include <AutoMenuJson/JsonWidgetFactory.hpp>
#include "Widgets/Button.hpp"

namespace JsonWidgetDetails
{
	template <typename Derived>
	class BuiltInJsonWidgetFactory : public IJsonWidgetFactory
	{
	public:
		[[nodiscard]] std::string_view widgetType() const override { return Derived::type; }
		[[nodiscard]] std::unique_ptr<Widget> createWidget(
			const json& data, const ValueRegistry& values) const override
		{
			return Derived::create(data, values);
		}
	};

	inline sf::Color color(const json& data, const char* key, sf::Color fallback)
	{
		if (!data.contains(key)) return fallback;
		const auto channels = data.at(key).get<std::array<int, 4>>();
		for (int channel : channels)
			if (channel < 0 || channel > 255)
				throw std::invalid_argument(std::string("Color channel out of range in '") + key + "'");
		return { static_cast<std::uint8_t>(channels[0]), static_cast<std::uint8_t>(channels[1]),
			static_cast<std::uint8_t>(channels[2]), static_cast<std::uint8_t>(channels[3]) };
	}

	inline sf::Vector2f vector2(const json& data, const char* key)
	{
		const auto values = data.at(key).get<std::array<float, 2>>();
		return { values[0], values[1] };
	}

	inline sf::FloatRect bounds(const json& data)
	{
		return { vector2(data, "position"), vector2(data, "size") };
	}

	inline std::uint8_t layer(const json& data)
	{
		const auto value = data.value("renderLayer", 0u);
		if (value > 255u) throw std::invalid_argument("renderLayer must be between 0 and 255");
		return static_cast<std::uint8_t>(value);
	}

	inline ButtonData buttonData(const json& data)
	{
		return ButtonData(bounds(data), color(data, "fill", { 80, 100, 160, 255 }),
			color(data, "outline", sf::Color::White), data.value("outlineThickness", 3.f),
			layer(data), data.value("name", std::string{}),
			data.value("hoverBrightness", 0.16f), data.value("pressedBrightness", -0.18f));
	}

	template <typename W, typename... Owners>
	std::unique_ptr<Widget> keep(std::unique_ptr<W> widget, const Owners&... owners)
	{
		(widget->retainLifetime(owners), ...);
		return widget;
	}
}
