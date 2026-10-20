#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/Bar.hpp"

class BarJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<BarJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "bar";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry& values)
	{
		using namespace JsonWidgetDetails;
		auto value = values.get<float>(data.at("value").get<std::string>());
		auto maximum = values.get<const float>(data.at("maximum").get<std::string>());
		BarData<float> config(bounds(data), value, maximum, color(data, "background", {70,70,70,255}),
			color(data, "fill", {70,200,100,255}), color(data, "outline", sf::Color::White),
			data.value("outlineThickness", 0.f), layer(data), data.value("animationSpeed", 0.5f));
		BarFactory<float> factory;
		return factory.create(config);
	}
};
