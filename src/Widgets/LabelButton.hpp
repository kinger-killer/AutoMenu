#pragma once

#include <memory>

#include "Button.hpp"

class LabelButton : public Button
{
public:
	// The font must remain alive for as long as this button uses it.
	LabelButton(const sf::FloatRect& buttonBounds,
		const sf::Font& font,
		const sf::String& label,
		unsigned int characterSize = 24)
		: Button(buttonBounds), text(font, label, characterSize)
	{
		centerText();
	}

	void setLabel(const sf::String& label)
	{
		text.setString(label);
		centerText();
	}

	void setTextColor(const sf::Color& color)
	{
		text.setFillColor(color);
	}

	void render(sf::RenderWindow& window) override
	{
		Button::render(window);
		window.draw(text);
	}

protected:
	void onGeometryChanged() override
	{
		centerText();
	}

private:
	void centerText()
	{
		const auto bounds = text.getLocalBounds();
		text.setOrigin({
			bounds.position.x + bounds.size.x / 2.f,
			bounds.position.y + bounds.size.y / 2.f
		});

		const auto position = getButtonPosition();
		const auto size = getButtonSize();
		text.setPosition({ position.x + size.x / 2.f, position.y + size.y / 2.f });
	}

	sf::Text text;
};

struct LabelButtonData
{
	LabelButtonData(
		const ButtonData& buttonData,
		const sf::Font& buttonFont,
		const sf::String& buttonLabel,
		unsigned int buttonCharacterSize = 24,
		const sf::Color& buttonTextColor = sf::Color::White)
		: button(buttonData),
		  font(buttonFont),
		  label(buttonLabel),
		  characterSize(buttonCharacterSize),
		  textColor(buttonTextColor)
	{
	}

	ButtonData button;
	const sf::Font& font;
	sf::String label;
	unsigned int characterSize;
	sf::Color textColor;
};

class LabelButtonFactory : public WidgetFactory<LabelButton, LabelButtonData>
{
public:
	[[nodiscard]] std::unique_ptr<LabelButton> create(const LabelButtonData& data) const override
	{
		auto button = std::make_unique<LabelButton>(
			data.button.bounds, data.font, data.label, data.characterSize);
		data.button.applyStyleTo(*button);
		button->setTextColor(data.textColor);
		return button;
	}
};
