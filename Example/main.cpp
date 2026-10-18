#include <cmath>
#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "../src/UI.hpp"
#include "../src/Widgets/Bar.hpp"
#include "../src/Widgets/Label.hpp"
#include "../src/Widgets/LabelButton.hpp"
#include "../src/Widgets/SpriteButton.hpp"
#include "../src/Widgets/Slider.hpp"
#include "../src/Widgets/RadioButton.hpp"
#include "../src/Widgets/DropdownList.hpp"
#include "../src/Widgets/ToggleSwitch.hpp"
#include "../src/Widgets/CheckBox.hpp"
#include "../src/Widgets/TextInput.hpp"

static float distanceToSegment(float x, float y, float ax, float ay, float bx, float by)
{
	const float dx = bx - ax;
	const float dy = by - ay;
	const float lengthSquared = dx * dx + dy * dy;
	const float t = std::clamp(((x - ax) * dx + (y - ay) * dy) / lengthSquared, 0.f, 1.f);
	const float nearestX = ax + t * dx;
	const float nearestY = ay + t * dy;
	return std::sqrt((x - nearestX) * (x - nearestX) + (y - nearestY) * (y - nearestY));
}

static sf::Image makeTriangleOutlineImage()
{
	sf::Image image({ 64u, 64u }, sf::Color::Transparent);
	for (unsigned int y = 0; y < 64; ++y)
	{
		for (unsigned int x = 0; x < 64; ++x)
		{
			const float px = static_cast<float>(x);
			const float py = static_cast<float>(y);
			if (distanceToSegment(px, py, 32.f, 5.f, 7.f, 56.f) <= 2.f
				|| distanceToSegment(px, py, 7.f, 56.f, 57.f, 56.f) <= 2.f
				|| distanceToSegment(px, py, 57.f, 56.f, 32.f, 5.f) <= 2.f)
				image.setPixel({ x, y }, sf::Color::White);
		}
	}
	return image;
}

static sf::Image makeCheckMarkImage()
{
	sf::Image image({ 64u, 64u }, sf::Color::Transparent);
	for (unsigned int y = 0; y < 64; ++y)
	{
		for (unsigned int x = 0; x < 64; ++x)
		{
			const float px = static_cast<float>(x);
			const float py = static_cast<float>(y);
			if (distanceToSegment(px, py, 13.f, 33.f, 27.f, 48.f) <= 3.f
				|| distanceToSegment(px, py, 27.f, 48.f, 53.f, 16.f) <= 3.f)
				image.setPixel({ x, y }, sf::Color::White);
		}
	}
	return image;
}

static sf::Image makePillImage(sf::Vector2u size, const sf::Color& color)
{
	sf::Image image(size, sf::Color::Transparent);
	const float radius = static_cast<float>(size.y) / 2.f;
	const float leftCenter = radius;
	const float rightCenter = static_cast<float>(size.x) - radius;
	for (unsigned int y = 0; y < size.y; ++y)
	{
		for (unsigned int x = 0; x < size.x; ++x)
		{
			const float px = static_cast<float>(x);
			const float py = static_cast<float>(y);
			const float nearestX = std::clamp(px, leftCenter, rightCenter);
			const float dx = px - nearestX;
			const float dy = py - radius;
			if (dx * dx + dy * dy <= radius * radius)
				image.setPixel({ x, y }, color);
		}
	}
	return image;
}

static sf::Image makeCircleImage(unsigned int diameter, const sf::Color& color)
{
	sf::Image image({ diameter, diameter }, sf::Color::Transparent);
	const float radius = static_cast<float>(diameter) / 2.f;
	for (unsigned int y = 0; y < diameter; ++y)
	{
		for (unsigned int x = 0; x < diameter; ++x)
		{
			const float dx = static_cast<float>(x) + 0.5f - radius;
			const float dy = static_cast<float>(y) + 0.5f - radius;
			if (dx * dx + dy * dy <= radius * radius)
				image.setPixel({ x, y }, color);
		}
	}
	return image;
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

int runExample()
{
	sf::RenderWindow window(sf::VideoMode({ 1150u, 800u }), "AutoMenu widget showcase");
	sf::Clock frameClock;

	// SFML Text and Sprite keep references to these resources, so they outlive UI.
	sf::Font font;
	if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
		return 1;

	sf::Image iconImage({ 64u, 64u }, sf::Color::Transparent);
	for (unsigned int y = 0; y < 64; ++y)
	{
		for (unsigned int x = 0; x < 64; ++x)
		{
			const int distance = std::abs(static_cast<int>(x) - 32)
				+ std::abs(static_cast<int>(y) - 32);
			if (distance <= 27)
				iconImage.setPixel({ x, y }, distance < 16
					? sf::Color(255, 235, 100)
					: sf::Color(220, 125, 35));
		}
	}
	sf::Texture iconTexture(iconImage);
	sf::Image trackImage({ 1u, 1u }, sf::Color::White);
	sf::Texture sliderTrackTexture(trackImage);
	sf::Image triangleIndicatorImage = makeTriangleOutlineImage();
	sf::Texture triangleIndicatorTexture(triangleIndicatorImage);
	sf::Image customCheckMarkImage = makeCheckMarkImage();
	sf::Texture customCheckMarkTexture(customCheckMarkImage);
	sf::Image toggleOffImage = makePillImage({ 96u, 48u }, sf::Color(80, 80, 80));
	sf::Image toggleOnImage = makePillImage({ 96u, 48u }, sf::Color(65, 170, 110));
	sf::Image toggleKnobImage = makeCircleImage(48u, sf::Color::White);
	sf::Texture toggleOffTexture(toggleOffImage);
	sf::Texture toggleOnTexture(toggleOnImage);
	sf::Texture toggleKnobTexture(toggleKnobImage);
	float health = 75.f;
	const float maxHealth = 100.f;
	int selectedQuality = 0;
	std::size_t selectedResolution = 0;
	bool fullscreen = false;
	bool subtitles = true;
	bool customCheckboxEnabled = false;
	sf::String playerName;

	UI ui;
	float volume = 0.5f;

	const LabelData titleData(font, "AutoMenu Widget Showcase", sf::Vector2f(70.f, 28.f), 30);
	LabelFactory labelFactory;
	ui.addWidget(labelFactory.create(titleData));
	const LabelData leftSectionData(font, "Health and controls", sf::Vector2f(70.f, 95.f), 24);
	const LabelData rightSectionData(font, "Settings", sf::Vector2f(620.f, 95.f), 24);
	ui.addWidget(labelFactory.create(leftSectionData));
	ui.addWidget(labelFactory.create(rightSectionData));

	const LabelData healthLabelData(font, "HP (Space: -10, R: reset)",
		sf::Vector2f(70.f, 145.f), 22);
	auto healthLabel = labelFactory.create(healthLabelData);
	ui.addWidget(std::move(healthLabel));

	auto volumeLabel = std::make_unique<Label>(font, "Volume: 50%", sf::Vector2f(70.f, 445.f), 22);
	Label* volumeLabelView = volumeLabel.get();
	ui.addWidget(std::move(volumeLabel));

	const SliderData<float> volumeSliderData(
		sf::FloatRect({ 70.f, 485.f }, { 460.f, 32.f }), volume, 0.f, 1.f,
		sliderTrackTexture, iconTexture);
	SliderFactory<float> sliderFactory;
	ui.addWidget(sliderFactory.create(volumeSliderData));

	const LabelData qualityLabelData(font, "Quality", sf::Vector2f(620.f, 145.f), 20);
	ui.addWidget(labelFactory.create(qualityLabelData));
	const RadioButtonData<int> lowQualityData(
		sf::FloatRect({ 620.f, 180.f }, { 260.f, 32.f }), font,
		"Low quality", selectedQuality, 0);
	const RadioButtonData<int> highQualityData(
		sf::FloatRect({ 620.f, 222.f }, { 260.f, 32.f }), font,
		"High quality", selectedQuality, 1);
	RadioButtonFactory<int> radioButtonFactory;
	ui.addWidget(radioButtonFactory.create(lowQualityData));
	ui.addWidget(radioButtonFactory.create(highQualityData));

	const LabelData resolutionLabelData(font, "Resolution", sf::Vector2f(620.f, 294.f), 20);
	ui.addWidget(labelFactory.create(resolutionLabelData));
	const DropdownListData resolutionData(
		sf::FloatRect({ 620.f, 330.f }, { 300.f, 38.f }), font,
		{ "800 x 600", "1280 x 720", "1920 x 1080" }, selectedResolution);
	DropdownListFactory dropdownFactory;
	ui.addWidget(dropdownFactory.create(resolutionData));

	const ToggleSwitchData fullscreenData(
		sf::FloatRect({ 620.f, 510.f }, { 76.f, 36.f }), fullscreen,
		sf::Color(65, 170, 110), sf::Color(80, 80, 80), sf::Color::White,
		sf::Color::Transparent, 0.f, 0,
		&toggleOffTexture, &toggleOnTexture, &toggleKnobTexture,
		sf::milliseconds(220), 0.78f);
	ToggleSwitchFactory toggleFactory;
	ui.addWidget(toggleFactory.create(fullscreenData));
	const LabelData fullscreenLabelData(font, "Fullscreen", sf::Vector2f(715.f, 510.f), 22);
	ui.addWidget(labelFactory.create(fullscreenLabelData));

	const CheckBoxData subtitlesData(
		sf::FloatRect({ 620.f, 570.f }, { 300.f, 34.f }), font,
		"Subtitles", subtitles);
	CheckBoxFactory checkBoxFactory;
	ui.addWidget(checkBoxFactory.create(subtitlesData));

	// A second checkbox demonstrates how to customize its appearance through Data.
	const CheckBoxDesign customCheckBoxDesign(
		&triangleIndicatorTexture, &customCheckMarkTexture,
		sf::Color(100, 220, 255), sf::Color(255, 210, 80),
		sf::Color(25, 45, 60), sf::Color(20, 80, 110),
		sf::Color(25, 45, 60), sf::Color(180, 235, 255),
		3.f, 0.9f, 20);
	const CheckBoxData customSubtitlesData(
		sf::FloatRect({ 620.f, 625.f }, { 340.f, 34.f }), font,
		"Custom design", customCheckboxEnabled, customCheckBoxDesign);
	ui.addWidget(checkBoxFactory.create(customSubtitlesData));

	const TextInputData playerNameData(
		sf::FloatRect({ 620.f, 690.f }, { 380.f, 42.f }), font,
		playerName, "Player name");
	TextInputFactory textInputFactory;
	ui.addWidget(textInputFactory.create(playerNameData));

	const BarData<float> healthBarData(
		sf::FloatRect({ 70.f, 185.f }, { 460.f, 28.f }), health, maxHealth,
		sf::Color(70, 70, 70), sf::Color(70, 200, 100), sf::Color::White, 2.f);
	BarFactory<float> barFactory;
	auto healthBar = barFactory.create(healthBarData);
	ui.addWidget(std::move(healthBar));

	// The label button draws the usual rectangle and centers its text.
	const ButtonData labelButtonStyle(
		sf::FloatRect({ 70.f, 270.f }, { 260.f, 100.f }),
		sf::Color(80, 100, 160), sf::Color::White, 3.f, 0, "LabelButton");
	const LabelButtonData labelButtonData(labelButtonStyle, font, "Text button", 30);
	LabelButtonFactory labelButtonFactory;
	auto labelButton = labelButtonFactory.create(labelButtonData);

	// The sprite is scaled to fill its button bounds and also acts as the hit area.
	const ButtonData spriteButtonStyle(
		sf::FloatRect({ 390.f, 260.f }, { 120.f, 120.f }),
		sf::Color::White, sf::Color::White, 0.f, 1, "SpriteButton");
	const SpriteButtonData spriteButtonData(spriteButtonStyle, iconTexture);
	SpriteButtonFactory spriteButtonFactory;
	auto spriteButton = spriteButtonFactory.create(spriteButtonData);
	const LabelData eventStatusData(font, "Move over or click a button", sf::Vector2f(70.f, 390.f), 18);
	auto eventStatus = labelFactory.create(eventStatusData);
	Label* eventStatusView = eventStatus.get();
	ui.addWidget(std::move(eventStatus));

	// One observer can handle events from both buttons; the event identifies its source and name.
	auto buttonObserver = std::make_shared<ButtonDemoObserver>(*eventStatusView);
	labelButton->subscribe(buttonObserver);
	spriteButton->subscribe(buttonObserver);
	ui.addWidget(std::move(labelButton));
	ui.addWidget(std::move(spriteButton));

	while (window.isOpen())
	{
		volumeLabelView->setText("Volume: " + std::to_string(static_cast<int>(volume * 100.f)) + "%");
		const sf::Time deltaTime = frameClock.restart();
		while (const auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
			if (const auto* key = event->getIf<sf::Event::KeyPressed>())
			{
				if (key->code == sf::Keyboard::Key::Space)
					health = std::max(0.f, health - 10.f);
				else if (key->code == sf::Keyboard::Key::R)
					health = maxHealth;
			}

			// Events are processed immediately; frame time is applied once below.
			ui.update(*event, window, sf::Time::Zero);
		}
		// An empty event is harmless for input widgets and advances time-based widgets.
		ui.update(sf::Event{ sf::Event::FocusGained{} }, window, deltaTime);

		window.clear(sf::Color(35, 35, 35));
		ui.render(window);
		window.display();
	}
	return 0;
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return runExample();
}
#else
int main()
{
	return runExample();
}
#endif
