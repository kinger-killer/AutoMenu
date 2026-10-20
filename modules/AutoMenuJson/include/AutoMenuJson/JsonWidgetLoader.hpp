#pragma once

#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <AutoMenuJson/JsonWidgetFactory.hpp>
#include "UI.hpp"

// Reads JSON documents and hands each widget description to its registered factory.
class JsonWidgetLoader
{
public:
	JsonWidgetLoader(const ValueRegistry& values, const JsonWidgetFactoryRegistry& factories)
		: values(values), factories(factories)
	{
	}

	[[nodiscard]] std::vector<Widget*> loadFile(const std::string& path, UI& ui) const
	{
		std::ifstream file(path);
		if (!file)
			throw std::runtime_error("Could not open UI description: " + path);

		json document;
		file >> document;
		return load(document, ui);
	}

	[[nodiscard]] std::vector<Widget*> load(const json& document, UI& ui) const
	{
		if (!document.is_object() || !document.contains("widgets") ||
			!document.at("widgets").is_array())
			throw std::invalid_argument("UI JSON must contain a 'widgets' array");

		std::vector<std::unique_ptr<Widget>> widgets;
		widgets.reserve(document.at("widgets").size());
		for (const auto& description : document.at("widgets"))
			widgets.push_back(factories.createWidget(description, values));

		std::vector<Widget*> loadedWidgets;
		loadedWidgets.reserve(widgets.size());
		for (auto& widget : widgets)
		{
			loadedWidgets.push_back(widget.get());
			ui.addWidget(std::move(widget));
		}
		return loadedWidgets;
	}

private:
	const ValueRegistry& values;
	const JsonWidgetFactoryRegistry& factories;
};
