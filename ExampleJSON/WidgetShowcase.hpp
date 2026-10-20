#pragma once

#include <memory>

#include "../Example/ExampleAssets.hpp"
#include "../src/Observer.hpp"
#include "../src/UI.hpp"
#include "../src/Widgets/Label.hpp"
#include <AutoMenuJson/RuntimeBindings.hpp>

struct JsonExampleState
{
	std::shared_ptr<float> health = std::make_shared<float>(75.f);
	std::shared_ptr<const float> maxHealth = std::make_shared<const float>(100.f);
	std::shared_ptr<float> volume = std::make_shared<float>(0.5f);
	std::shared_ptr<int> selectedQuality = std::make_shared<int>(0);
	std::shared_ptr<std::size_t> selectedResolution = std::make_shared<std::size_t>(0);
	std::shared_ptr<bool> fullscreen = std::make_shared<bool>(false);
	std::shared_ptr<bool> subtitles = std::make_shared<bool>(true);
	std::shared_ptr<bool> customCheckboxEnabled = std::make_shared<bool>(false);
	std::shared_ptr<sf::String> playerName = std::make_shared<sf::String>();
};

struct JsonExampleReferences
{
	Label* volumeLabel = nullptr;
	std::shared_ptr<IObserver> buttonObserver;
	ValueRegistry values;
};

JsonExampleReferences addJsonWidgetShowcase(
	UI& ui, const std::shared_ptr<ExampleAssets>& assets, JsonExampleState& state);
void handleJsonExampleInput(const sf::Event& event, JsonExampleState& state);
