#pragma once
#include <AutoMenuJson/JsonWidgetFactoryHelpers.hpp>
#include "Widgets/Button.hpp"

namespace json
{
class ButtonJsonWidgetFactory final : public JsonWidgetDetails::BuiltInJsonWidgetFactory<ButtonJsonWidgetFactory>
{
public:
	static constexpr std::string_view type = "button";
	static std::unique_ptr<widgets::Widget> create(const Json& data, const ValueRegistry&)
	{
		const auto config = JsonWidgetDetails::buttonData(data);
		widgets::ButtonFactory factory;
		return factory.create(config);
	}
};

} // namespace json
