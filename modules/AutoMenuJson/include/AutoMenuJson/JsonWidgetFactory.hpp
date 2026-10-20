#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include <AutoMenuJson/RuntimeBindings.hpp>
#include "Widgets/Widget.hpp"

using json = nlohmann::json;

class IJsonWidgetFactory
{
public:
	virtual ~IJsonWidgetFactory() = default;
	[[nodiscard]] virtual std::string_view widgetType() const = 0;
	[[nodiscard]] virtual std::unique_ptr<Widget> createWidget(
		const json& data, const ValueRegistry& values) const = 0;
};

// Dispatches JSON widget descriptions to registered per-type factories.
class JsonWidgetFactoryRegistry
{
public:
	void add(std::unique_ptr<IJsonWidgetFactory> factory)
	{
		if (!factory)
			throw std::invalid_argument("Cannot register a null JSON widget factory");

		const std::string type(factory->widgetType());
		if (factories.contains(type))
			throw std::invalid_argument("A JSON widget factory is already registered for: " + type);

		factories.emplace(type, std::move(factory));
	}

	[[nodiscard]] std::unique_ptr<Widget> createWidget(
		const json& data, const ValueRegistry& values) const
	{
		if (!data.is_object() || !data.contains("type"))
			throw std::invalid_argument("Each widget description must have a 'type'");

		const std::string type = data.at("type").get<std::string>();
		const auto found = factories.find(type);
		if (found == factories.end())
			throw std::invalid_argument("No JSON widget factory registered for type: " + type);

		return found->second->createWidget(data, values);
	}

private:
	std::unordered_map<std::string, std::unique_ptr<IJsonWidgetFactory>> factories;
};
