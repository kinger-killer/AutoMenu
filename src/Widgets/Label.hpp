#pragma once

#include <memory>

#include "Widget.hpp"

class Label : public Widget
{
public:
	// The font must remain alive for as long as this label uses it.
	Label(const sf::Font& font, const sf::String& label, const sf::Vector2f& position,
		unsigned int characterSize = 24)
		: text(font, label, characterSize)
	{
		this->text.setPosition(position);
	}

	void setText(const sf::String& value)
	{
		text.setString(value);
	}

	void setPosition(const sf::Vector2f& position)
	{
		text.setPosition(position);
	}

	void setColor(const sf::Color& color)
	{
		text.setFillColor(color);
	}

	void setCharacterSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}

	void render(sf::RenderWindow& window) override
	{
		window.draw(text);
	}

	void update(const sf::Event&, const sf::RenderWindow&, sf::Time) override
	{
	}

private:
	sf::Text text;
};

struct LabelData
{
	// The font must remain alive for as long as the created label uses it.
	LabelData(const sf::Font& labelFont, const sf::String& labelText,
		const sf::Vector2f& labelPosition, unsigned int labelCharacterSize = 24,
		const sf::Color& labelColor = sf::Color::White, std::uint8_t labelRenderLayer = 0)
		: font(labelFont), text(labelText), position(labelPosition),
		  characterSize(labelCharacterSize), color(labelColor), renderLayer(labelRenderLayer)
	{
	}

	const sf::Font& font;
	sf::String text;
	sf::Vector2f position;
	unsigned int characterSize;
	sf::Color color;
	std::uint8_t renderLayer;

	void applyStyleTo(Label& label) const
	{
		label.setColor(color);
		label.setRenderLayer(renderLayer);
	}
};

class LabelFactory : public WidgetFactory<Label, LabelData>
{
public:
	[[nodiscard]] std::unique_ptr<Label> create(const LabelData& data) const override
	{
		auto label = std::make_unique<Label>(
			data.font, data.text, data.position, data.characterSize);
		data.applyStyleTo(*label);
		return label;
	}
};
