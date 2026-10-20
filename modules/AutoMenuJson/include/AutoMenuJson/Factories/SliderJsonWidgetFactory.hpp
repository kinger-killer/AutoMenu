#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/Slider.hpp"

class SliderJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<SliderJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "slider";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto value = values.get<float>(data.at("value").get<std::string>());
		auto track = values.get<sf::Texture>(data.at("trackTexture").get<std::string>());
		auto knob = values.get<sf::Texture>(data.at("knobTexture").get<std::string>());
		SliderData<float> config(bounds(data), *value, data.at("minimum").get<float>(),
			data.at("maximum").get<float>(), *track, *knob, color(data,"trackColor",{70,70,70,255}),
			color(data,"knobColor",{100,160,240,255}), layer(data));
		SliderFactory<float> factory;
		return keep(factory.create(config), value, track, knob);
	}
};
