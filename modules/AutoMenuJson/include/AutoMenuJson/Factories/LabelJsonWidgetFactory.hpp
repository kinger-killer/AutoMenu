#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/Label.hpp"

namespace AutoMenu::json
{
class LabelJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<LabelJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "label";
	static std::unique_ptr<widgets::Widget> create(const Json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto font = values.get<sf::Font>(data.at("font").get<std::string>());
		widgets::LabelData config(*font, sf::String(data.at("text").get<std::string>()), vector2(data,"position"),
			data.value("characterSize",24u), color(data,"color",sf::Color::White), layer(data));
		widgets::LabelFactory factory;
		return keep(factory.create(config), font);
	}
};

} // namespace AutoMenu::json
