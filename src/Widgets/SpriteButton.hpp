#pragma once

#include <memory>

#include "Button.hpp"

class SpriteButton : public Button
{
public:
	// The texture must remain alive for as long as this button uses it.
	SpriteButton(const sf::FloatRect& buttonBounds, const sf::Texture& texture)
		: Button(buttonBounds), sprite(texture)
	{
		fitSpriteToButton();
	}

	void setTint(const sf::Color& color)
	{
		baseTint = color;
		sprite.setColor(baseTint);
	}

	void render(sf::RenderWindow& window) override
	{
		window.draw(sprite);
	}

protected:
	void onInteractionColorChanged(const sf::Color&, float brightness) override
	{
		sprite.setColor(adjustBrightness(baseTint, brightness));
	}

	void onGeometryChanged() override
	{
		fitSpriteToButton();
	}

private:
	void fitSpriteToButton()
	{
		const auto localBounds = sprite.getLocalBounds();
		if (localBounds.size.x == 0.f || localBounds.size.y == 0.f)
			return;

		const auto size = getButtonSize();
		sprite.setOrigin(localBounds.position);
		sprite.setPosition(getButtonPosition());
		sprite.setScale({ size.x / localBounds.size.x, size.y / localBounds.size.y });
	}

	sf::Sprite sprite;
	sf::Color baseTint{ sf::Color::White };
};

struct SpriteButtonData
{
	SpriteButtonData(
		const ButtonData& buttonData,
		const sf::Texture& buttonTexture,
		const sf::Color& buttonTint = sf::Color::White)
		: button(buttonData), texture(buttonTexture), tint(buttonTint)
	{
	}

	ButtonData button;
	const sf::Texture& texture;
	sf::Color tint;
};

class SpriteButtonFactory : public WidgetFactory<SpriteButton, SpriteButtonData>
{
public:
	[[nodiscard]] std::unique_ptr<SpriteButton> create(const SpriteButtonData& data) const override
	{
		auto button = std::make_unique<SpriteButton>(data.button.bounds, data.texture);
		data.button.applyStyleTo(*button);
		button->setTint(data.tint);
		return button;
	}
};
