#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>

#include "Widget.hpp"

class TextInput : public Widget
{
public:
	// Font and value must outlive this text input.
	TextInput(const sf::FloatRect& bounds, const sf::Font& font,
		sf::String& value, const sf::String& placeholder = "", std::size_t maxLength = 256,
		unsigned int characterSize = 22)
		: bounds(bounds), value(value), placeholder(placeholder), maxLength(maxLength),
	  box(bounds.size), text(font, "", characterSize)
	{
		box.setPosition(bounds.position);
		cursor.setFillColor(textColor);
		cursorIndex = value.getSize();
	}

	void setBackgroundColor(const sf::Color& color) { backgroundColor = color; }
	void setFocusedBackgroundColor(const sf::Color& color) { focusedBackgroundColor = color; }
	void setTextColor(const sf::Color& color) { textColor = color; }
	void setPlaceholderColor(const sf::Color& color) { placeholderColor = color; }
	void setOutline(const sf::Color& color, float thickness)
	{
		box.setOutlineColor(color);
		box.setOutlineThickness(thickness);
	}
	[[nodiscard]] bool isFocused() const { return focused; }

	void render(sf::RenderWindow& window) override
	{
		box.setFillColor(focused ? focusedBackgroundColor : backgroundColor);
		window.draw(box);
		cursorIndex = std::min(cursorIndex, value.getSize());
		const sf::String& displayedText = value.isEmpty() && !focused ? placeholder : value;
		text.setString(displayedText);
		text.setFillColor(value.isEmpty() && !focused ? placeholderColor : textColor);
		const float availableWidth = std::max(1.f, bounds.size.x - horizontalPadding * 2.f);
		const float textWidth = text.getLocalBounds().size.x;
		const float cursorWidth = measureWidth(value.substring(0, cursorIndex));
		text.setString(displayedText);
		if (focused)
		{
			if (cursorWidth < horizontalScrollOffset)
				horizontalScrollOffset = cursorWidth;
			else if (cursorWidth > horizontalScrollOffset + availableWidth)
				horizontalScrollOffset = cursorWidth - availableWidth;
		}
		else
			horizontalScrollOffset = std::max(0.f, textWidth - availableWidth);
		horizontalScrollOffset = std::clamp(horizontalScrollOffset, 0.f,
			std::max(0.f, textWidth - availableWidth));
		text.setPosition({ bounds.position.x + horizontalPadding - horizontalScrollOffset,
			bounds.position.y + verticalPadding });

		const sf::View originalView = window.getView();
		const auto clipTopLeft = window.mapCoordsToPixel({
			bounds.position.x + horizontalPadding, bounds.position.y + verticalPadding });
		const auto clipBottomRight = window.mapCoordsToPixel({
			bounds.position.x + bounds.size.x - horizontalPadding,
			bounds.position.y + bounds.size.y - verticalPadding });
		const auto targetSize = window.getSize();
		if (targetSize.x > 0 && targetSize.y > 0)
		{
			const float left = std::clamp(static_cast<float>(clipTopLeft.x), 0.f,
				static_cast<float>(targetSize.x));
			const float top = std::clamp(static_cast<float>(clipTopLeft.y), 0.f,
				static_cast<float>(targetSize.y));
			const float right = std::clamp(static_cast<float>(clipBottomRight.x), left,
				static_cast<float>(targetSize.x));
			const float bottom = std::clamp(static_cast<float>(clipBottomRight.y), top,
				static_cast<float>(targetSize.y));
			sf::View clippedView = originalView;
			clippedView.setScissor(sf::FloatRect(
				{ left / targetSize.x, top / targetSize.y },
				{ (right - left) / targetSize.x, (bottom - top) / targetSize.y }));
			window.setView(clippedView);
			window.draw(text);
			if (focused && caretVisible)
			{
				const float cursorHeight = std::min(bounds.size.y * 0.45f,
					bounds.size.y - verticalPadding * 2.f);
				cursor.setSize({ 1.5f, cursorHeight });
				cursor.setPosition({ bounds.position.x + horizontalPadding + cursorWidth - horizontalScrollOffset,
					bounds.position.y + (bounds.size.y - cursorHeight) / 2.f });
				cursor.setFillColor(textColor);
				window.draw(cursor);
			}
			window.setView(originalView);
		}
	}

	void update(const sf::Event& event, const sf::RenderWindow& window, sf::Time deltaTime) override
	{
		caretBlinkElapsed += deltaTime;
		if (caretBlinkElapsed >= caretBlinkInterval)
		{
			caretBlinkElapsed %= caretBlinkInterval;
			caretVisible = !caretVisible;
		}

		if (const auto* press = event.getIf<sf::Event::MouseButtonPressed>())
		{
			const bool clickedInside = press->button == sf::Mouse::Button::Left &&
				bounds.contains(window.mapPixelToCoords(press->position));
			focused = clickedInside;
			if (clickedInside)
			{
				text.setString(value);
				const auto mousePosition = window.mapPixelToCoords(press->position);
				setCursorFromMouseX(mousePosition.x - (bounds.position.x + horizontalPadding)
					+ horizontalScrollOffset);
				caretVisible = true;
				caretBlinkElapsed = sf::Time::Zero;
			}
			return;
		}

		if (!focused)
			return;

		if (const auto* key = event.getIf<sf::Event::KeyPressed>())
		{
			switch (key->code)
			{
			case sf::Keyboard::Key::Backspace:
				if (cursorIndex > 0)
					value.erase(--cursorIndex);
				break;
			case sf::Keyboard::Key::Delete:
				if (cursorIndex < value.getSize())
					value.erase(cursorIndex);
				break;
			case sf::Keyboard::Key::Left:
				if (cursorIndex > 0) --cursorIndex;
				break;
			case sf::Keyboard::Key::Right:
				if (cursorIndex < value.getSize()) ++cursorIndex;
				break;
			case sf::Keyboard::Key::Home:
				cursorIndex = 0;
				break;
			case sf::Keyboard::Key::End:
				cursorIndex = value.getSize();
				break;
			default:
				break;
			}
			caretVisible = true;
			caretBlinkElapsed = sf::Time::Zero;
		}
		else if (const auto* entered = event.getIf<sf::Event::TextEntered>())
		{
			if (entered->unicode >= U' ' && entered->unicode != U'\x7f' &&
				value.getSize() < maxLength)
			{
				value.insert(cursorIndex, sf::String(entered->unicode));
				++cursorIndex;
				caretVisible = true;
				caretBlinkElapsed = sf::Time::Zero;
			}
		}
		else if (event.is<sf::Event::FocusLost>())
		{
			focused = false;
		}
	}

private:
	float measureWidth(const sf::String& string)
	{
		text.setString(string);
		return text.getLocalBounds().size.x;
	}

	void setCursorFromMouseX(float mouseX)
	{
		cursorIndex = 0;
		float nearestDistance = std::numeric_limits<float>::max();
		for (std::size_t index = 0; index <= value.getSize(); ++index)
		{
			const float distance = std::abs(measureWidth(value.substring(0, index)) - mouseX);
			if (distance >= nearestDistance)
				break;
			nearestDistance = distance;
			cursorIndex = index;
		}
		text.setString(value);
	}

	static constexpr float horizontalPadding = 8.f;
	static constexpr float verticalPadding = 4.f;
	static constexpr sf::Time caretBlinkInterval{ sf::milliseconds(500) };

	sf::FloatRect bounds;
	sf::String& value;
	sf::String placeholder;
	std::size_t maxLength;
	sf::RectangleShape box;
	sf::Text text;
	sf::RectangleShape cursor;
	sf::Color backgroundColor{ 45, 45, 45 };
	sf::Color focusedBackgroundColor{ 55, 55, 65 };
	sf::Color textColor{ sf::Color::White };
	sf::Color placeholderColor{ 160, 160, 160 };
	bool focused = false;
	std::size_t cursorIndex = 0;
	float horizontalScrollOffset = 0.f;
	sf::Time caretBlinkElapsed = sf::Time::Zero;
	bool caretVisible = true;
};

struct TextInputData
{
	// Font and value must outlive the created text input.
	TextInputData(const sf::FloatRect& inputBounds, const sf::Font& inputFont,
		sf::String& inputValue, const sf::String& inputPlaceholder = "",
		std::size_t maxLength = 256, unsigned int characterSize = 22,
		const sf::Color& backgroundColor = sf::Color(45, 45, 45),
		const sf::Color& focusedBackgroundColor = sf::Color(55, 55, 65),
		const sf::Color& textColor = sf::Color::White,
		const sf::Color& placeholderColor = sf::Color(160, 160, 160),
		const sf::Color& outlineColor = sf::Color::White,
		float outlineThickness = 1.f, std::uint8_t renderLayer = 0)
		: bounds(inputBounds), font(inputFont), value(inputValue), placeholder(inputPlaceholder),
		  maxLength(maxLength), characterSize(characterSize), backgroundColor(backgroundColor),
		  focusedBackgroundColor(focusedBackgroundColor), textColor(textColor),
		  placeholderColor(placeholderColor), outlineColor(outlineColor),
		  outlineThickness(outlineThickness), renderLayer(renderLayer)
	{
	}

	sf::FloatRect bounds;
	const sf::Font& font;
	sf::String& value;
	sf::String placeholder;
	std::size_t maxLength;
	unsigned int characterSize;
	sf::Color backgroundColor;
	sf::Color focusedBackgroundColor;
	sf::Color textColor;
	sf::Color placeholderColor;
	sf::Color outlineColor;
	float outlineThickness;
	std::uint8_t renderLayer;

	void applyStyleTo(TextInput& input) const
	{
		input.setBackgroundColor(backgroundColor);
		input.setFocusedBackgroundColor(focusedBackgroundColor);
		input.setTextColor(textColor);
		input.setPlaceholderColor(placeholderColor);
		input.setOutline(outlineColor, outlineThickness);
		input.setRenderLayer(renderLayer);
	}
};

class TextInputFactory : public WidgetFactory<TextInput, TextInputData>
{
public:
	[[nodiscard]] std::unique_ptr<TextInput> create(const TextInputData& data) const override
	{
		auto input = std::make_unique<TextInput>(data.bounds, data.font, data.value,
			data.placeholder, data.maxLength, data.characterSize);
		data.applyStyleTo(*input);
		return input;
	}
};
