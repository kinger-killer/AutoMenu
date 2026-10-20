#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/SpriteButton.hpp"

class SpriteButtonJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<SpriteButtonJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "sprite_button";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto texture = values.get<sf::Texture>(data.at("texture").get<std::string>());
		SpriteButtonData config(buttonData(data), *texture, color(data,"tint",sf::Color::White));
		SpriteButtonFactory factory;
		return keep(factory.create(config), texture);
	}
};
