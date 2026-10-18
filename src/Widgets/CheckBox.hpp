#pragma once

#include <algorithm>
#include <cmath>
#include <memory>

#include "Button.hpp"

struct CheckBoxDesign
{
	CheckBoxDesign(
		const sf::Texture* indicatorTexture = nullptr,
		const sf::Texture* checkedMarkTexture = nullptr,
		const sf::Color& outlineColor = sf::Color::White,
		const sf::Color& hoverColor = sf::Color(120, 160, 240),
		const sf::Color& hoverFillColor = sf::Color(120, 160, 240, 70),
		const sf::Color& checkedColor = sf::Color(80, 190, 120),
		const sf::Color& uncheckedFillColor = sf::Color::Transparent,
		const sf::Color& textColor = sf::Color::White,
		float outlineThickness = 2.f,
		float indicatorSizeRatio = 0.8f,
		unsigned int characterSize = 22)
		: indicatorTexture(indicatorTexture), checkedMarkTexture(checkedMarkTexture),
		  outlineColor(outlineColor), hoverColor(hoverColor),
		  hoverFillColor(hoverFillColor), checkedColor(checkedColor),
		  uncheckedFillColor(uncheckedFillColor), textColor(textColor),
		  outlineThickness(outlineThickness), indicatorSizeRatio(indicatorSizeRatio),
		  characterSize(characterSize)
	{
	}

	// Optional transparent PNG-style textures, scaled to the indicator bounds.
	// Keep the textures alive for as long as the checkbox uses this design.
	const sf::Texture* indicatorTexture = nullptr;
	const sf::Texture* checkedMarkTexture = nullptr;
	sf::Color outlineColor{ sf::Color::White };
	sf::Color hoverColor{ 120, 160, 240 };
	sf::Color hoverFillColor{ 120, 160, 240, 70 };
	sf::Color checkedColor{ 80, 190, 120 };
	sf::Color uncheckedFillColor{ sf::Color::Transparent };
	sf::Color textColor{ sf::Color::White };
	float outlineThickness = 2.f;
	float indicatorSizeRatio = 0.8f;
	unsigned int characterSize = 22;
};

class CheckBox : public Button
{
public:
	// Font and checked value must outlive this checkbox.
	CheckBox(const sf::FloatRect& bounds, const sf::Font& font,
		const sf::String& label, bool& checked,
		const CheckBoxDesign& design = CheckBoxDesign{})
		: CheckBox(bounds, font, label, design, false, &checked)
	{
	}

	void setDesign(const CheckBoxDesign& newDesign)
	{
		design = newDesign;
		applyDesign();
	}

	[[nodiscard]] const CheckBoxDesign& getDesign() const { return design; }

	void render(sf::RenderWindow& window) override
	{
		const sf::Vector2f center(bounds.position.x + bounds.size.y / 2.f,
			bounds.position.y + bounds.size.y / 2.f);
		const bool selected = isChecked();
		const float size = bounds.size.y * design.indicatorSizeRatio;
		const sf::Vector2f topLeft(center.x - size / 2.f, center.y - size / 2.f);
		const bool hovered = isButtonHovered();
		const sf::Color outline = hovered ? design.hoverColor : design.outlineColor;

		if (design.indicatorTexture)
		{
			const sf::Color tint = selected && !design.checkedMarkTexture
				? design.checkedColor : outline;
			drawTexture(window, *design.indicatorTexture, topLeft, size, tint);
		}
		else
		{
			indicator.setPosition(center);
			indicator.setOutlineColor(outline);
			indicator.setFillColor(radioAppearance
				? (hovered ? design.hoverFillColor : design.uncheckedFillColor)
				: (selected ? design.checkedColor
					: (hovered ? design.hoverFillColor : design.uncheckedFillColor)));
			window.draw(indicator);

			if (radioAppearance && selected && !design.checkedMarkTexture)
			{
				radioDot.setPosition(center);
				radioDot.setFillColor(design.checkedColor);
				window.draw(radioDot);
			}
		}

		if (selected && design.checkedMarkTexture)
			drawTexture(window, *design.checkedMarkTexture, topLeft, size,
			hovered ? design.hoverColor : design.checkedColor);
		window.draw(text);
	}

	void update(const sf::Event& event, const sf::RenderWindow& window,
		sf::Time deltaTime) override
	{
		Button::update(event, window, deltaTime);
		if (IsPressed(event, window))
			setChecked(!isChecked());
	}

protected:
	// Derived controls can reuse the same visuals while defining their own state behavior.
	CheckBox(const sf::FloatRect& bounds, const sf::Font& font,
		const sf::String& label, const CheckBoxDesign& design, bool radioAppearance,
		bool* checkedReference = nullptr)
		: Button(bounds), bounds(bounds), checkedReference(checkedReference),
		  radioAppearance(radioAppearance),
		  indicator(bounds.size.y * design.indicatorSizeRatio / 2.f),
		  radioDot(bounds.size.y * design.indicatorSizeRatio * 0.28f / 2.f),
		  text(font, label, design.characterSize), design(design)
	{
		applyDesign();
		text.setPosition({ bounds.position.x + bounds.size.y + 8.f, bounds.position.y });
	}

	[[nodiscard]] bool isChecked() const
	{
		return checkedReference ? *checkedReference : checkedState;
	}
	void setChecked(bool value)
	{
		checkedState = value;
		if (checkedReference)
			*checkedReference = value;
	}
private:
	static void drawTexture(sf::RenderWindow& window, const sf::Texture& texture,
		sf::Vector2f position, float size, sf::Color tint)
	{
		const sf::Vector2u textureSize = texture.getSize();
		if (textureSize.x == 0 || textureSize.y == 0)
			return;

		sf::Sprite sprite(texture);
		const float scale = size / static_cast<float>(std::max(textureSize.x, textureSize.y));
		sprite.setScale({ scale, scale });
		const sf::Vector2f renderedSize(static_cast<float>(textureSize.x) * scale,
			static_cast<float>(textureSize.y) * scale);
		sprite.setPosition(position + sf::Vector2f((size - renderedSize.x) / 2.f,
			(size - renderedSize.y) / 2.f));
		sprite.setColor(tint);
		window.draw(sprite);
	}

	void applyDesign()
	{
		indicator.setRadius(bounds.size.y * design.indicatorSizeRatio / 2.f);
		indicator.setOrigin({ indicator.getRadius(), indicator.getRadius() });
		indicator.setOutlineThickness(design.outlineThickness);
		radioDot.setRadius(indicator.getRadius() * 0.48f);
		radioDot.setOrigin({ radioDot.getRadius(), radioDot.getRadius() });
		text.setFillColor(design.textColor);
		text.setCharacterSize(design.characterSize);
	}

	sf::FloatRect bounds;
	bool* checkedReference;
	bool checkedState = false;
	bool radioAppearance = false;
	sf::CircleShape indicator;
	sf::CircleShape radioDot;
	sf::Text text;
	CheckBoxDesign design;
};

struct CheckBoxData
{
	// Font and checked value must outlive the created checkbox.
	CheckBoxData(const sf::FloatRect& boxBounds, const sf::Font& boxFont,
		const sf::String& boxLabel, bool& checkedValue,
		const CheckBoxDesign& boxDesign = CheckBoxDesign{}, std::uint8_t renderLayer = 0)
		: bounds(boxBounds), font(boxFont), label(boxLabel), checked(checkedValue),
		  design(boxDesign), renderLayer(renderLayer)
	{
	}

	sf::FloatRect bounds;
	const sf::Font& font;
	sf::String label;
	bool& checked;
	CheckBoxDesign design;
	std::uint8_t renderLayer;

	void applyStyleTo(CheckBox& box) const
	{
		box.setDesign(design);
		box.setRenderLayer(renderLayer);
	}
};

class CheckBoxFactory : public WidgetFactory<CheckBox, CheckBoxData>
{
public:
	[[nodiscard]] std::unique_ptr<CheckBox> create(const CheckBoxData& data) const override
	{
		auto box = std::make_unique<CheckBox>(data.bounds, data.font, data.label, data.checked);
		data.applyStyleTo(*box);
		return box;
	}
};
