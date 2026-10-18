#pragma once

#include <SFML/Graphics.hpp>

// Fonts and textures are kept together because the widgets borrow references to them.
struct ExampleAssets
{
	ExampleAssets();

	bool loadFont();

	sf::Font font;
	sf::Texture iconTexture;
	sf::Texture sliderTrackTexture;
	sf::Texture triangleIndicatorTexture;
	sf::Texture customCheckMarkTexture;
	sf::Texture toggleOffTexture;
	sf::Texture toggleOnTexture;
	sf::Texture toggleKnobTexture;
};
