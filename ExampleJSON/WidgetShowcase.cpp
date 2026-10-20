#include "WidgetShowcase.hpp"

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../src/Widgets/Button.hpp"
#include <AutoMenuJson/DefaultJsonWidgetFactoryRegistry.hpp>
#include <AutoMenuJson/JsonWidgetLoader.hpp>

namespace
{
	template <typename T, typename Owner>
	std::shared_ptr<T> shareMember(const std::shared_ptr<Owner>& owner, T& member)
	{
		return std::shared_ptr<T>(owner, &member);
	}

	class ButtonDemoObserver final : public IObserver
	{
	public:
		explicit ButtonDemoObserver(Label& label) : status(label) {}
		void notify(Event& event) override
		{
			const auto* buttonEvent = dynamic_cast<ButtonEvent*>(&event);
			if (!buttonEvent) return;
			const char* action = "";
			switch (buttonEvent->type)
			{
			case ButtonEventType::Pressed: action = "pressed"; break;
			case ButtonEventType::Released: action = "released"; break;
			case ButtonEventType::OnHoverBegin: action = "hover begin"; break;
			case ButtonEventType::OnHoverEnd: action = "hover end"; break;
			}
			status.setText(buttonEvent->buttonName + ": " + action);
		}
	private:
		Label& status;
	};
}

JsonExampleReferences addJsonWidgetShowcase(
	UI& ui, const std::shared_ptr<ExampleAssets>& assets, JsonExampleState& state)
{
	JsonExampleReferences references;
	auto& values = references.values;
	values.bind("player.health", state.health);
	values.bind("player.max_health", state.maxHealth);
	values.bind("settings.volume", state.volume);
	values.bind("settings.quality", state.selectedQuality);
	values.bind("settings.resolution", state.selectedResolution);
	values.bind("settings.fullscreen", state.fullscreen);
	values.bind("settings.subtitles", state.subtitles);
	values.bind("settings.custom_checkbox", state.customCheckboxEnabled);
	values.bind("player.name", state.playerName);

	// Aliasing shared_ptrs keep the ExampleAssets owner alive while a widget uses a member.
	values.bind("asset.font", shareMember(assets, assets->font));
	values.bind("asset.icon", shareMember(assets, assets->iconTexture));
	values.bind("asset.slider_track", shareMember(assets, assets->sliderTrackTexture));
	values.bind("asset.triangle_indicator", shareMember(assets, assets->triangleIndicatorTexture));
	values.bind("asset.check_mark", shareMember(assets, assets->customCheckMarkTexture));
	values.bind("asset.toggle_off", shareMember(assets, assets->toggleOffTexture));
	values.bind("asset.toggle_on", shareMember(assets, assets->toggleOnTexture));
	values.bind("asset.toggle_knob", shareMember(assets, assets->toggleKnobTexture));

	std::ifstream file(AUTOMENU_JSON_EXAMPLE_MENU_FILE);
	if (!file) throw std::runtime_error("Could not open ExampleJSON/menu.json");
	json document;
	file >> document;
	JsonWidgetLoader loader(values, getDefaultJsonWidgetFactoryRegistry());
	const auto loaded = loader.load(document, ui);

	// IDs are app-side metadata; the generic widget loader deliberately ignores them.
	std::unordered_map<std::string, Widget*> byId;
	const auto& descriptions = document.at("widgets");
	for (std::size_t i = 0; i < loaded.size(); ++i)
		if (descriptions.at(i).contains("id"))
			byId.emplace(descriptions.at(i).at("id").get<std::string>(), loaded[i]);

	references.volumeLabel = dynamic_cast<Label*>(byId.at("volume_status"));
	auto* buttonStatus = dynamic_cast<Label*>(byId.at("button_status"));
	references.buttonObserver = std::make_shared<ButtonDemoObserver>(*buttonStatus);
	for (Widget* widget : loaded)
		if (auto* button = dynamic_cast<Button*>(widget))
			button->subscribe(references.buttonObserver);
	return references;
}

void handleJsonExampleInput(const sf::Event& event, JsonExampleState& state)
{
	const auto* key = event.getIf<sf::Event::KeyPressed>();
	if (!key) return;
	if (key->code == sf::Keyboard::Key::Space)
		*state.health = std::max(0.f, *state.health - 10.f);
	else if (key->code == sf::Keyboard::Key::R)
		*state.health = *state.maxHealth;
}
