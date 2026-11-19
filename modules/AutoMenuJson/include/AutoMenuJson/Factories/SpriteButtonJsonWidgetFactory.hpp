#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/SpriteButton.hpp"

namespace AutoMenu::json
{
class SpriteButtonJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<SpriteButtonJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "sprite_button";
	static std::unique_ptr<widgets::Widget> create(const Json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto texture = values.get<sf::Texture>(data.at("texture").get<std::string>());
		widgets::SpriteButtonData config(buttonData(data), *texture, color(data,"tint",sf::Color::White));
		widgets::SpriteButtonFactory factory;
		return keep(factory.create(config), texture);
	}
};

} // namespace AutoMenu::json
