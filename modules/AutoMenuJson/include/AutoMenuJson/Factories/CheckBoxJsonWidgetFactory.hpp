#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/CheckBox.hpp"

class CheckBoxJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<CheckBoxJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "check_box";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto font = values.get<sf::Font>(data.at("font").get<std::string>());
		auto checked = values.get<bool>(data.at("value").get<std::string>());
		std::shared_ptr<sf::Texture> indicator, mark;
		if (data.contains("indicatorTexture")) indicator = values.get<sf::Texture>(data.at("indicatorTexture").get<std::string>());
		if (data.contains("checkedMarkTexture")) mark = values.get<sf::Texture>(data.at("checkedMarkTexture").get<std::string>());
		CheckBoxDesign design(indicator.get(), mark.get(), color(data,"outlineColor",sf::Color::White),
			color(data,"hoverColor",{120,160,240,255}), color(data,"hoverFillColor",{120,160,240,70}),
			color(data,"checkedColor",{80,190,120,255}), color(data,"uncheckedFillColor",sf::Color::Transparent),
			color(data,"textColor",sf::Color::White), data.value("outlineThickness",2.f),
			data.value("indicatorSizeRatio",0.8f), data.value("characterSize",22u));
		CheckBoxData config(bounds(data), *font, sf::String(data.value("text",std::string{})), *checked, design, layer(data));
		CheckBoxFactory factory;
		auto widget = factory.create(config);
		widget->retainLifetime(font); widget->retainLifetime(checked);
		widget->retainLifetime(indicator); widget->retainLifetime(mark);
		return std::unique_ptr<Widget>(std::move(widget));
	}
};
