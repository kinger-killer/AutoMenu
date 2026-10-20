#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/TextInput.hpp"

class TextInputJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<TextInputJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "text_input";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto font = values.get<sf::Font>(data.at("font").get<std::string>());
		auto value = values.get<sf::String>(data.at("value").get<std::string>());
		TextInputData config(bounds(data), *font, *value, sf::String(data.value("placeholder",std::string{})),
			data.value("maxLength",std::size_t{256}), data.value("characterSize",22u),
			color(data,"background",{45,45,45,255}), color(data,"focusedBackground",{55,55,65,255}),
			color(data,"textColor",sf::Color::White), color(data,"placeholderColor",{160,160,160,255}),
			color(data,"outlineColor",sf::Color::White), data.value("outlineThickness",1.f), layer(data));
		TextInputFactory factory;
		return keep(factory.create(config), font, value);
	}
};
