#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Button.hpp"

class DropdownOptionButton : public Button
{
public:
	DropdownOptionButton(const sf::FloatRect& bounds, const sf::Font& font,
		const sf::String& label, unsigned int characterSize)
		: Button(bounds), text(font, label, characterSize)
	{
		setName(label.toAnsiString());
		text.setPosition({ bounds.position.x + 8.f, bounds.position.y + 4.f });
	}

	void setOptionColors(const sf::Color& normal, const sf::Color& hover,
		const sf::Color& selected, const sf::Color& textColor)
	{
		normalColor = normal;
		hoverColor = hover;
		selectedColor = selected;
		text.setFillColor(textColor);
		setFillColor(isSelected ? selectedColor : normalColor);
		setHoverFillColor(hoverColor);
	}

	void setSelected(bool selected)
	{
		isSelected = selected;
		setFillColor(isSelected ? selectedColor : normalColor);
	}

	void render(sf::RenderWindow& window) override
	{
		Button::render(window);
		window.draw(text);
	}

	void update(const sf::Event& event, const sf::RenderWindow& window,
		sf::Time deltaTime) override
	{
		Button::update(event, window, deltaTime);
	}

private:
	sf::Text text;
	sf::Color normalColor{ 65, 65, 65 };
	sf::Color hoverColor{ 120, 160, 240 };
	sf::Color selectedColor{ 80, 100, 160 };
	bool isSelected = false;
};

class DropdownList : public Widget
{
public:
	// Font and selectedIndex must outlive this dropdown.
	DropdownList(const sf::FloatRect& bounds, const sf::Font& font,
		std::vector<sf::String> options, std::size_t& selectedIndex,
		unsigned int characterSize = 20)
		: bounds(bounds), font(font), options(std::move(options)), selectedIndex(selectedIndex),
		  characterSize(characterSize), box(bounds.size), text(font, "", characterSize)
	{
		box.setPosition(bounds.position);
		for (std::size_t index = 0; index < this->options.size(); ++index)
		{
			const sf::FloatRect optionBounds(
				{ bounds.position.x, bounds.position.y + bounds.size.y * static_cast<float>(index + 1) },
				bounds.size);
			auto option = std::make_unique<DropdownOptionButton>(
				optionBounds, font, this->options[index], characterSize);
			option->setOptionColors(optionColor, hoverOptionColor, selectedOptionColor, textColor);
			option->setOutline(outlineColor, outlineThickness);
			option->setSelected(index == selectedIndex);
			optionButtons.push_back(std::move(option));
		}
	}

	void setBackgroundColor(const sf::Color& color) { backgroundColor = color; }
	void setOptionColor(const sf::Color& color)
	{
		optionColor = color;
		updateOptionStyles();
	}
	void setHoverOptionColor(const sf::Color& color)
	{
		hoverOptionColor = color;
		updateOptionStyles();
	}
	void setSelectedOptionColor(const sf::Color& color)
	{
		selectedOptionColor = color;
		updateOptionStyles();
	}
	void setTextColor(const sf::Color& color)
	{
		textColor = color;
		updateOptionStyles();
	}
	void setOutline(const sf::Color& color, float thickness)
	{
		outlineColor = color;
		outlineThickness = thickness;
		box.setOutlineColor(color);
		box.setOutlineThickness(thickness);
		for (auto& option : optionButtons)
			option->setOutline(color, thickness);
	}

	[[nodiscard]] DropdownOptionButton* getOptionButton(std::size_t index)
	{
		return index < optionButtons.size() ? optionButtons[index].get() : nullptr;
	}

	void render(sf::RenderWindow& window) override
	{
		box.setFillColor(isHeaderHovered ? headerHoverColor : backgroundColor);
		window.draw(box);
		text.setString(selectedIndex < options.size() ? options[selectedIndex] : sf::String{});
		text.setFillColor(textColor);
		text.setPosition({ bounds.position.x + 8.f, bounds.position.y + 4.f });
		window.draw(text);

		if (isExpanded)
			for (auto& option : optionButtons)
				option->render(window);
	}

	void update(const sf::Event& event, const sf::RenderWindow& window, sf::Time deltaTime) override
	{
		if (const auto* move = event.getIf<sf::Event::MouseMoved>())
			isHeaderHovered = bounds.contains(window.mapPixelToCoords(move->position));
		else if (event.is<sf::Event::MouseLeft>())
			isHeaderHovered = false;

		const auto* press = event.getIf<sf::Event::MouseButtonPressed>();
		const bool leftClick = press && press->button == sf::Mouse::Button::Left;
		if (leftClick && bounds.contains(window.mapPixelToCoords(press->position)))
		{
			isExpanded = !isExpanded;
			return;
		}

		if (!isExpanded)
			return;

		if (leftClick)
		{
			const sf::Vector2f mouse = window.mapPixelToCoords(press->position);
			for (std::size_t index = 0; index < optionButtons.size(); ++index)
			{
				const sf::Vector2f position(bounds.position.x,
					bounds.position.y + bounds.size.y * static_cast<float>(index + 1));
				if (sf::FloatRect(position, bounds.size).contains(mouse))
				{
					selectedIndex = index;
					updateOptionSelection();
					break;
				}
			}
		}

		for (auto& option : optionButtons)
			option->update(event, window, deltaTime);

		if (leftClick)
			isExpanded = false;
	}

private:
	void updateOptionStyles()
	{
		for (std::size_t index = 0; index < optionButtons.size(); ++index)
		{
			optionButtons[index]->setOptionColors(
				optionColor, hoverOptionColor, selectedOptionColor, textColor);
			optionButtons[index]->setSelected(index == selectedIndex);
		}
	}

	void updateOptionSelection()
	{
		for (std::size_t index = 0; index < optionButtons.size(); ++index)
			optionButtons[index]->setSelected(index == selectedIndex);
	}

	sf::FloatRect bounds;
	const sf::Font& font;
	std::vector<sf::String> options;
	std::size_t& selectedIndex;
	unsigned int characterSize;
	sf::RectangleShape box;
	sf::Text text;
	std::vector<std::unique_ptr<DropdownOptionButton>> optionButtons;
	sf::Color backgroundColor{ 55, 55, 55 };
	sf::Color optionColor{ 65, 65, 65 };
	sf::Color hoverOptionColor{ 120, 160, 240 };
	sf::Color selectedOptionColor{ 80, 100, 160 };
	sf::Color headerHoverColor{ 85, 105, 150 };
	sf::Color textColor{ sf::Color::White };
	sf::Color outlineColor{ sf::Color::White };
	float outlineThickness = 1.f;
	bool isExpanded = false;
	bool isHeaderHovered = false;
};

struct DropdownListData
{
	// Font and selectedIndex must outlive the created dropdown.
	DropdownListData(const sf::FloatRect& listBounds, const sf::Font& listFont,
		std::vector<sf::String> listOptions, std::size_t& currentIndex,
		unsigned int listCharacterSize = 20,
		const sf::Color& listBackgroundColor = sf::Color(55, 55, 55),
		const sf::Color& listOptionColor = sf::Color(65, 65, 65),
		const sf::Color& listSelectedOptionColor = sf::Color(80, 100, 160),
		const sf::Color& listTextColor = sf::Color::White,
		const sf::Color& listOutlineColor = sf::Color::White,
		float listOutlineThickness = 1.f, std::uint8_t listRenderLayer = 0,
		const sf::Color& listHoverOptionColor = sf::Color(120, 160, 240))
		: bounds(listBounds), font(listFont), options(std::move(listOptions)),
		  selectedIndex(currentIndex), characterSize(listCharacterSize),
		  backgroundColor(listBackgroundColor), optionColor(listOptionColor),
		  selectedOptionColor(listSelectedOptionColor), textColor(listTextColor),
		  outlineColor(listOutlineColor), outlineThickness(listOutlineThickness),
		  renderLayer(listRenderLayer), hoverOptionColor(listHoverOptionColor)
	{
	}

	sf::FloatRect bounds;
	const sf::Font& font;
	std::vector<sf::String> options;
	std::size_t& selectedIndex;
	unsigned int characterSize;
	sf::Color backgroundColor;
	sf::Color optionColor;
	sf::Color selectedOptionColor;
	sf::Color textColor;
	sf::Color outlineColor;
	float outlineThickness;
	std::uint8_t renderLayer;
	sf::Color hoverOptionColor;

	void applyStyleTo(DropdownList& dropdown) const
	{
		dropdown.setBackgroundColor(backgroundColor);
		dropdown.setOptionColor(optionColor);
		dropdown.setSelectedOptionColor(selectedOptionColor);
		dropdown.setHoverOptionColor(hoverOptionColor);
		dropdown.setTextColor(textColor);
		dropdown.setOutline(outlineColor, outlineThickness);
		dropdown.setRenderLayer(renderLayer);
	}
};

class DropdownListFactory : public WidgetFactory<DropdownList, DropdownListData>
{
public:
	[[nodiscard]] std::unique_ptr<DropdownList> create(const DropdownListData& data) const override
	{
		auto dropdown = std::make_unique<DropdownList>(
			data.bounds, data.font, data.options, data.selectedIndex, data.characterSize);
		data.applyStyleTo(*dropdown);
		return dropdown;
	}
};
