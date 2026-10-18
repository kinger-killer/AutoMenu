#pragma once

#include <algorithm>
#include <cmath>
#include <memory>

#include "Widget.hpp"

class ToggleSwitch : public Widget
{
public:
	// Referenced value and optional textures must outlive this switch.
	ToggleSwitch(const sf::FloatRect& bounds, bool& value)
		: bounds(bounds), value(value), track(bounds.size), knob(bounds.size.y * 0.42f),
		  animationProgress(value ? 1.f : 0.f), targetProgress(animationProgress)
	{
		track.setPosition(bounds.position);
		knob.setOrigin({ knob.getRadius(), knob.getRadius() });
		updateVisuals();
	}

	void setOnColor(const sf::Color& color) { onColor = color; }
	void setOffColor(const sf::Color& color) { offColor = color; }
	void setKnobColor(const sf::Color& color)
	{
		knobColor = color;
		knob.setFillColor(color);
		if (knobSprite)
			knobSprite->setColor(color);
	}
	void setOutline(const sf::Color& color, float thickness)
	{
		track.setOutlineColor(color);
		track.setOutlineThickness(thickness);
	}
	void setAnimationDuration(sf::Time duration)
	{
		animationDuration = duration > sf::Time::Zero ? duration : sf::Time::Zero;
	}
	void setKnobScale(float scale)
	{
		knobScale = std::max(0.f, scale);
		updateVisuals();
	}
	void setCustomTextures(const sf::Texture* offTrack, const sf::Texture* onTrack,
		const sf::Texture* knobTexture)
	{
		offTrackSprite = offTrack ? std::make_unique<sf::Sprite>(*offTrack) : nullptr;
		onTrackSprite = onTrack ? std::make_unique<sf::Sprite>(*onTrack) : nullptr;
		knobSprite = knobTexture ? std::make_unique<sf::Sprite>(*knobTexture) : nullptr;
		if (knobSprite)
			knobSprite->setColor(knobColor);
		updateVisuals();
	}

	void render(sf::RenderWindow& window) override
	{
		if (offTrackSprite && onTrackSprite)
			window.draw(value ? *onTrackSprite : *offTrackSprite);
		else
		{
			track.setFillColor(value ? onColor : offColor);
			window.draw(track);
		}

		if (knobSprite)
			window.draw(*knobSprite);
		else
		{
			knob.setFillColor(knobColor);
			window.draw(knob);
		}
	}

	void update(const sf::Event& event, const sf::RenderWindow& window, sf::Time deltaTime) override
	{
		const auto* press = event.getIf<sf::Event::MouseButtonPressed>();
		if (press && press->button == sf::Mouse::Button::Left &&
			bounds.contains(window.mapPixelToCoords(press->position)))
			value = !value;

		const float newTarget = value ? 1.f : 0.f;
		if (newTarget != targetProgress)
			targetProgress = newTarget;

		if (animationDuration == sf::Time::Zero)
			animationProgress = targetProgress;
		else
		{
			const float step = static_cast<float>(deltaTime.asSeconds() / animationDuration.asSeconds());
			if (animationProgress < targetProgress)
				animationProgress = std::min(targetProgress, animationProgress + step);
			else if (animationProgress > targetProgress)
				animationProgress = std::max(targetProgress, animationProgress - step);
		}
		updateVisuals();
	}

private:
	void updateVisuals()
	{
		const float knobDiameter = bounds.size.y * knobScale;
		const float knobRadius = knobDiameter / 2.f;
		const float left = bounds.position.x + knobRadius;
		const float right = bounds.position.x + bounds.size.x - knobRadius;
		const float easedProgress = animationProgress * animationProgress * (3.f - 2.f * animationProgress);
		const float centerX = left + (right - left) * easedProgress;
		const float centerY = bounds.position.y + bounds.size.y / 2.f;

		knob.setRadius(knobRadius);
		knob.setOrigin({ knobRadius, knobRadius });
		knob.setPosition({ centerX, centerY });
		if (knobSprite)
			fitSprite(*knobSprite, { centerX - knobRadius, centerY - knobRadius },
				{ knobDiameter, knobDiameter });
		if (offTrackSprite)
			fitSprite(*offTrackSprite, bounds.position, bounds.size);
		if (onTrackSprite)
			fitSprite(*onTrackSprite, bounds.position, bounds.size);
	}

	static void fitSprite(sf::Sprite& sprite, const sf::Vector2f& position, const sf::Vector2f& size)
	{
		const auto localBounds = sprite.getLocalBounds();
		if (localBounds.size.x <= 0.f || localBounds.size.y <= 0.f)
			return;
		sprite.setOrigin(localBounds.position);
		sprite.setPosition(position);
		sprite.setScale({ size.x / localBounds.size.x, size.y / localBounds.size.y });
	}

	sf::FloatRect bounds;
	bool& value;
	sf::RectangleShape track;
	sf::CircleShape knob;
	std::unique_ptr<sf::Sprite> offTrackSprite;
	std::unique_ptr<sf::Sprite> onTrackSprite;
	std::unique_ptr<sf::Sprite> knobSprite;
	sf::Color onColor{ 65, 170, 110 };
	sf::Color offColor{ 80, 80, 80 };
	sf::Color knobColor{ sf::Color::White };
	sf::Time animationDuration{ sf::milliseconds(160) };
	float knobScale = 0.84f;
	float animationProgress = 0.f;
	float targetProgress = 0.f;
};

struct ToggleSwitchData
{
	// The referenced value and optional textures must outlive the created switch.
	ToggleSwitchData(const sf::FloatRect& switchBounds, bool& switchValue,
		const sf::Color& switchOnColor = sf::Color(65, 170, 110),
		const sf::Color& switchOffColor = sf::Color(80, 80, 80),
		const sf::Color& switchKnobColor = sf::Color::White,
		const sf::Color& outlineColor = sf::Color::Transparent,
		float outlineThickness = 0.f, std::uint8_t renderLayer = 0,
		const sf::Texture* offTrackTexture = nullptr,
		const sf::Texture* onTrackTexture = nullptr,
		const sf::Texture* knobTexture = nullptr,
		sf::Time animationDuration = sf::milliseconds(160),
		float knobScale = 0.84f)
		: bounds(switchBounds), value(switchValue), onColor(switchOnColor),
		  offColor(switchOffColor), knobColor(switchKnobColor),
		  outlineColor(outlineColor), outlineThickness(outlineThickness), renderLayer(renderLayer),
		  offTrackTexture(offTrackTexture), onTrackTexture(onTrackTexture), knobTexture(knobTexture),
		  animationDuration(animationDuration), knobScale(knobScale)
	{
	}

	sf::FloatRect bounds;
	bool& value;
	sf::Color onColor;
	sf::Color offColor;
	sf::Color knobColor;
	sf::Color outlineColor;
	float outlineThickness;
	std::uint8_t renderLayer;
	const sf::Texture* offTrackTexture;
	const sf::Texture* onTrackTexture;
	const sf::Texture* knobTexture;
	sf::Time animationDuration;
	float knobScale;

	void applyStyleTo(ToggleSwitch& toggle) const
	{
		toggle.setOnColor(onColor);
		toggle.setOffColor(offColor);
		toggle.setKnobColor(knobColor);
		toggle.setOutline(outlineColor, outlineThickness);
		toggle.setCustomTextures(offTrackTexture, onTrackTexture, knobTexture);
		toggle.setAnimationDuration(animationDuration);
		toggle.setKnobScale(knobScale);
		toggle.setRenderLayer(renderLayer);
	}
};

class ToggleSwitchFactory : public WidgetFactory<ToggleSwitch, ToggleSwitchData>
{
public:
	[[nodiscard]] std::unique_ptr<ToggleSwitch> create(const ToggleSwitchData& data) const override
	{
		auto toggle = std::make_unique<ToggleSwitch>(data.bounds, data.value);
		data.applyStyleTo(*toggle);
		return toggle;
	}
};
