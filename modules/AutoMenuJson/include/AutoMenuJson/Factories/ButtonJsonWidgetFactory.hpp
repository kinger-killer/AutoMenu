#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/Button.hpp"

class ButtonJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<ButtonJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "button";
	static std::unique_ptr<Widget> create(const json& data, const ValueRegistry&)
	{
		const auto config = JsonWidgetDetails::buttonData(data);
		ButtonFactory factory;
		return factory.create(config);
	}
};
