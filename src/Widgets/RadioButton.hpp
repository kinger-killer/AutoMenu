#pragma once

#include <memory>

#include "CheckBox.hpp"

template <typename Value = int>
class RadioButton : public CheckBox
{
public:
	// Font and selectedValue must outlive this radio button.
	RadioButton(const sf::FloatRect& bounds, const sf::Font& font,
		const sf::String& label, Value& selectedValue, Value optionValue)
		: CheckBox(bounds, font, label, CheckBoxDesign{}, true),
		  selectedValue(selectedValue), optionValue(optionValue)
	{
	}

	void setSelectedColor(const sf::Color& color)
	{
		CheckBoxDesign design = getDesign();
		design.checkedColor = color;
		setDesign(design);
	}
	void setOutlineColor(const sf::Color& color)
	{
		CheckBoxDesign design = getDesign();
		design.outlineColor = color;
		setDesign(design);
	}
	void setTextColor(const sf::Color& color)
	{
		CheckBoxDesign design = getDesign();
		design.textColor = color;
		setDesign(design);
	}
	void setCharacterSize(unsigned int size)
	{
		CheckBoxDesign design = getDesign();
		design.characterSize = size;
		setDesign(design);
	}

	void render(sf::RenderWindow& window) override
	{
		setChecked(selectedValue == optionValue);
		CheckBox::render(window);
	}

	void update(const sf::Event& event, const sf::RenderWindow& window,
		sf::Time deltaTime) override
	{
		Button::update(event, window, deltaTime);
		if (IsPressed(event, window))
			selectedValue = optionValue;
	}

	private:
	Value& selectedValue;
	Value optionValue;
};

template <typename Value = int>
struct RadioButtonData
{
	// Font and selectedValue must outlive the created radio button.
	RadioButtonData(const sf::FloatRect& buttonBounds, const sf::Font& buttonFont,
		const sf::String& buttonLabel, Value& currentSelection, Value thisOption,
		const sf::Color& selectedColor = sf::Color(80, 160, 240),
		const sf::Color& outlineColor = sf::Color::White,
		const sf::Color& textColor = sf::Color::White,
		unsigned int characterSize = 22, std::uint8_t renderLayer = 0)
		: bounds(buttonBounds), font(buttonFont), label(buttonLabel),
		  selectedValue(currentSelection), optionValue(thisOption),
		  selectedColor(selectedColor), outlineColor(outlineColor), textColor(textColor),
		  characterSize(characterSize), design(), renderLayer(renderLayer)
	{
		design.checkedColor = selectedColor;
		design.outlineColor = outlineColor;
		design.textColor = textColor;
		design.characterSize = characterSize;
	}

	RadioButtonData(const sf::FloatRect& buttonBounds, const sf::Font& buttonFont,
		const sf::String& buttonLabel, Value& currentSelection, Value thisOption,
		const CheckBoxDesign& buttonDesign, std::uint8_t layer = 0)
		: bounds(buttonBounds), font(buttonFont), label(buttonLabel),
		  selectedValue(currentSelection), optionValue(thisOption),
		  selectedColor(buttonDesign.checkedColor), outlineColor(buttonDesign.outlineColor),
		  textColor(buttonDesign.textColor), characterSize(buttonDesign.characterSize),
		  design(buttonDesign), renderLayer(layer)
	{
	}

	sf::FloatRect bounds;
	const sf::Font& font;
	sf::String label;
	Value& selectedValue;
	Value optionValue;
	sf::Color selectedColor;
	sf::Color outlineColor;
	sf::Color textColor;
	unsigned int characterSize;
	CheckBoxDesign design;
	std::uint8_t renderLayer;

	void applyStyleTo(RadioButton<Value>& button) const
	{
		button.setDesign(design);
		button.setRenderLayer(renderLayer);
	}
};

template <typename Value = int>
class RadioButtonFactory : public WidgetFactory<RadioButton<Value>, RadioButtonData<Value>>
{
public:
	[[nodiscard]] std::unique_ptr<RadioButton<Value>> create(
		const RadioButtonData<Value>& data) const override
	{
		auto button = std::make_unique<RadioButton<Value>>(
			data.bounds, data.font, data.label, data.selectedValue, data.optionValue);
		data.applyStyleTo(*button);
		return button;
	}
};
