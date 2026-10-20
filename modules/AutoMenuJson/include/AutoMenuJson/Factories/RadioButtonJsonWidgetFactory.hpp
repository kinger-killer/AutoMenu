#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/RadioButton.hpp"

class RadioButtonJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<RadioButtonJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "radio_button";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto font = values.get<sf::Font>(data.at("font").get<std::string>());
		auto selected = values.get<int>(data.at("value").get<std::string>());
		RadioButtonData<int> config(bounds(data), *font, sf::String(data.at("text").get<std::string>()),
			*selected, data.at("option").get<int>(), color(data,"selectedColor",{80,160,240,255}),
			color(data,"outlineColor",sf::Color::White), color(data,"textColor",sf::Color::White),
			data.value("characterSize",22u), layer(data));
		RadioButtonFactory<int> factory;
		auto widget = factory.create(config);
		widget->retainLifetime(font); widget->retainLifetime(selected);
		return std::unique_ptr<Widget>(std::move(widget));
	}
};
