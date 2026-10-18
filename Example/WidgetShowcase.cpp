#include "WidgetShowcase.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "../src/Widgets/Bar.hpp"
#include "../src/Widgets/CheckBox.hpp"
#include "../src/Widgets/DropdownList.hpp"
#include "../src/Widgets/LabelButton.hpp"
#include "../src/Widgets/RadioButton.hpp"
#include "../src/Widgets/Slider.hpp"
#include "../src/Widgets/SpriteButton.hpp"
#include "../src/Widgets/TextInput.hpp"
#include "../src/Widgets/ToggleSwitch.hpp"

namespace
{
	Label* addLabel(UI& ui, const sf::Font& font, const sf::String& text,
		const sf::Vector2f& position, unsigned int characterSize)
	{
		const LabelData data(font, text, position, characterSize);
		LabelFactory factory;
		auto label = factory.create(data);
		Label* labelView = label.get();
		ui.addWidget(std::move(label));
		return labelView;
	}

	void addHeadings(UI& ui, const sf::Font& font)
	{
		addLabel(ui, font, "AutoMenu Widget Showcase", { 70.f, 28.f }, 30);
		addLabel(ui, font, "Health and controls", { 70.f, 95.f }, 24);
		addLabel(ui, font, "Settings", { 620.f, 95.f }, 24);
	}

	Label* addHealthAndVolumeControls(UI& ui, const ExampleAssets& assets, ExampleState& state)
	{
		addLabel(ui, assets.font, "HP (Space: -10, R: reset)", { 70.f, 145.f }, 22);

		const BarData<float> healthData(
			sf::FloatRect({ 70.f, 185.f }, { 460.f, 28.f }), state.health, state.maxHealth,
			sf::Color(70, 70, 70), sf::Color(70, 200, 100), sf::Color::White, 2.f);
		BarFactory<float> barFactory;
		ui.addWidget(barFactory.create(healthData));

		Label* volumeLabel = addLabel(ui, assets.font, "Volume: 50%", { 70.f, 445.f }, 22);
		const SliderData<float> volumeData(
			sf::FloatRect({ 70.f, 485.f }, { 460.f, 32.f }), state.volume, 0.f, 1.f,
			assets.sliderTrackTexture, assets.iconTexture);
		SliderFactory<float> sliderFactory;
		ui.addWidget(sliderFactory.create(volumeData));
		return volumeLabel;
	}

	void addQualitySettings(UI& ui, const ExampleAssets& assets, ExampleState& state)
	{
		addLabel(ui, assets.font, "Quality", { 620.f, 145.f }, 20);
		const RadioButtonData<int> lowQualityData(
			sf::FloatRect({ 620.f, 180.f }, { 260.f, 32.f }), assets.font,
			"Low quality", state.selectedQuality, 0);
		const RadioButtonData<int> highQualityData(
			sf::FloatRect({ 620.f, 222.f }, { 260.f, 32.f }), assets.font,
			"High quality", state.selectedQuality, 1);
		RadioButtonFactory<int> factory;
		ui.addWidget(factory.create(lowQualityData));
		ui.addWidget(factory.create(highQualityData));
	}

	void addResolutionSettings(UI& ui, const ExampleAssets& assets, ExampleState& state)
	{
		addLabel(ui, assets.font, "Resolution", { 620.f, 294.f }, 20);
		const DropdownListData data(
			sf::FloatRect({ 620.f, 330.f }, { 300.f, 38.f }), assets.font,
			{ "800 x 600", "1280 x 720", "1920 x 1080" }, state.selectedResolution);
		DropdownListFactory factory;
		ui.addWidget(factory.create(data));
	}

	void addToggleAndCheckboxes(UI& ui, const ExampleAssets& assets, ExampleState& state)
	{
		const ToggleSwitchData toggleData(
			sf::FloatRect({ 620.f, 510.f }, { 76.f, 36.f }), state.fullscreen,
			sf::Color(65, 170, 110), sf::Color(80, 80, 80), sf::Color::White,
			sf::Color::Transparent, 0.f, 0,
			&assets.toggleOffTexture, &assets.toggleOnTexture, &assets.toggleKnobTexture,
			sf::milliseconds(220), 0.78f);
		ToggleSwitchFactory toggleFactory;
		ui.addWidget(toggleFactory.create(toggleData));
		addLabel(ui, assets.font, "Fullscreen", { 715.f, 510.f }, 22);

		const CheckBoxData subtitlesData(
			sf::FloatRect({ 620.f, 570.f }, { 300.f, 34.f }), assets.font,
			"Subtitles", state.subtitles);
		CheckBoxFactory factory;
		ui.addWidget(factory.create(subtitlesData));

		// This second checkbox uses custom outline and check-mark artwork.
		const CheckBoxDesign customDesign(
			&assets.triangleIndicatorTexture, &assets.customCheckMarkTexture,
			sf::Color(100, 220, 255), sf::Color(255, 210, 80),
			sf::Color(25, 45, 60), sf::Color(20, 80, 110),
			sf::Color(25, 45, 60), sf::Color(180, 235, 255),
			3.f, 0.9f, 20);
		const CheckBoxData customCheckboxData(
			sf::FloatRect({ 620.f, 625.f }, { 340.f, 34.f }), assets.font,
			"Custom design", state.customCheckboxEnabled, customDesign);
		ui.addWidget(factory.create(customCheckboxData));
	}

	class ButtonDemoObserver : public IObserver
	{
	public:
		explicit ButtonDemoObserver(Label& eventStatus) : eventStatus(eventStatus) {}

		void notify(Event& event) override
		{
			const auto* buttonEvent = dynamic_cast<ButtonEvent*>(&event);
			if (!buttonEvent)
				return;

			const char* action = "";
			switch (buttonEvent->type)
			{
			case ButtonEventType::Pressed: action = "pressed"; break;
			case ButtonEventType::Released: action = "released"; break;
			case ButtonEventType::OnHoverBegin: action = "hover begin"; break;
			case ButtonEventType::OnHoverEnd: action = "hover end"; break;
			}
			eventStatus.setText(buttonEvent->buttonName + ": " + action);
		}

	private:
		Label& eventStatus;
	};

	std::shared_ptr<IObserver> addButtons(UI& ui, const ExampleAssets& assets)
	{
		const ButtonData labelStyle(
			sf::FloatRect({ 70.f, 270.f }, { 260.f, 100.f }),
			sf::Color(80, 100, 160), sf::Color::White, 3.f, 0, "LabelButton");
		const LabelButtonData labelData(labelStyle, assets.font, "Text button", 30);
		LabelButtonFactory labelFactory;
		auto labelButton = labelFactory.create(labelData);

		const ButtonData spriteStyle(
			sf::FloatRect({ 390.f, 260.f }, { 120.f, 120.f }),
			sf::Color::White, sf::Color::White, 0.f, 1, "SpriteButton");
		const SpriteButtonData spriteData(spriteStyle, assets.iconTexture);
		SpriteButtonFactory spriteFactory;
		auto spriteButton = spriteFactory.create(spriteData);

		Label* eventStatus = addLabel(
			ui, assets.font, "Move over or click a button", { 70.f, 390.f }, 18);
		auto observer = std::make_shared<ButtonDemoObserver>(*eventStatus);
		labelButton->subscribe(observer);
		spriteButton->subscribe(observer);
		ui.addWidget(std::move(labelButton));
		ui.addWidget(std::move(spriteButton));
		return observer;
	}

	void addTextInput(UI& ui, const ExampleAssets& assets, ExampleState& state)
	{
		const TextInputData data(
			sf::FloatRect({ 620.f, 690.f }, { 380.f, 42.f }), assets.font,
			state.playerName, "Player name");
		TextInputFactory factory;
		ui.addWidget(factory.create(data));
	}
}

ExampleWidgetReferences addWidgetShowcase(UI& ui, const ExampleAssets& assets, ExampleState& state)
{
	addHeadings(ui, assets.font);
	ExampleWidgetReferences references;
	references.volumeLabel = addHealthAndVolumeControls(ui, assets, state);
	addQualitySettings(ui, assets, state);
	addResolutionSettings(ui, assets, state);
	addToggleAndCheckboxes(ui, assets, state);
	addTextInput(ui, assets, state);
	references.buttonObserver = addButtons(ui, assets);
	return references;
}

void handleGameInput(const sf::Event& event, ExampleState& state)
{
	const auto* key = event.getIf<sf::Event::KeyPressed>();
	if (!key)
		return;

	if (key->code == sf::Keyboard::Key::Space)
		state.health = std::max(0.f, state.health - 10.f);
	else if (key->code == sf::Keyboard::Key::R)
		state.health = state.maxHealth;
}
