#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/ToggleSwitch.hpp"

class ToggleSwitchJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<ToggleSwitchJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "toggle_switch";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto value = values.get<bool>(data.at("value").get<std::string>());
		std::shared_ptr<sf::Texture> off, on, knob;
		if (data.contains("offTrackTexture")) off = values.get<sf::Texture>(data.at("offTrackTexture").get<std::string>());
		if (data.contains("onTrackTexture")) on = values.get<sf::Texture>(data.at("onTrackTexture").get<std::string>());
		if (data.contains("knobTexture")) knob = values.get<sf::Texture>(data.at("knobTexture").get<std::string>());
		ToggleSwitchData config(bounds(data), *value, color(data,"onColor",{65,170,110,255}),
			color(data,"offColor",{80,80,80,255}), color(data,"knobColor",sf::Color::White),
			color(data,"outlineColor",sf::Color::Transparent), data.value("outlineThickness",0.f),
			layer(data), off.get(), on.get(), knob.get(),
			sf::milliseconds(data.value("animationMilliseconds",160)), data.value("knobScale",0.84f));
		ToggleSwitchFactory factory;
		auto widget = factory.create(config);
		widget->retainLifetime(value); widget->retainLifetime(off);
		widget->retainLifetime(on); widget->retainLifetime(knob);
		return std::unique_ptr<Widget>(std::move(widget));
	}
};
