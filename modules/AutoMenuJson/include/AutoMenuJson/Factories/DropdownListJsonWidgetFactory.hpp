#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/DropdownList.hpp"

class DropdownListJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<DropdownListJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "dropdown_list";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto font = values.get<sf::Font>(data.at("font").get<std::string>());
		auto selected = values.get<std::size_t>(data.at("value").get<std::string>());
		std::vector<sf::String> options;
		for (const auto& option : data.at("options")) options.emplace_back(option.get<std::string>());
		DropdownListData config(bounds(data), *font, std::move(options), *selected,
			data.value("characterSize",20u), color(data,"background",{55,55,55,255}),
			color(data,"optionColor",{65,65,65,255}), color(data,"selectedOptionColor",{80,100,160,255}),
			color(data,"textColor",sf::Color::White), color(data,"outlineColor",sf::Color::White),
			data.value("outlineThickness",1.f), layer(data), color(data,"hoverOptionColor",{120,160,240,255}));
		DropdownListFactory factory;
		return keep(factory.create(config), font, selected);
	}
};
