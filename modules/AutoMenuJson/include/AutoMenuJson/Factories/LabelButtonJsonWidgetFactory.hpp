#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/LabelButton.hpp"

class LabelButtonJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<LabelButtonJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "label_button";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto font = values.get<sf::Font>(data.at("font").get<std::string>());
		LabelButtonData config(buttonData(data), *font, sf::String(data.at("text").get<std::string>()),
			data.value("characterSize",24u), color(data,"textColor",sf::Color::White));
		LabelButtonFactory factory;
		return keep(factory.create(config), font);
	}
};
