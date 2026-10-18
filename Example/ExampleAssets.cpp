#include "ExampleAssets.hpp"

#include <algorithm>
#include <cmath>

namespace
{
	float distanceToSegment(float x, float y, float startX, float startY, float endX, float endY)
	{
		const float dx = endX - startX;
		const float dy = endY - startY;
		const float lengthSquared = dx * dx + dy * dy;
		const float projection = std::clamp(
			((x - startX) * dx + (y - startY) * dy) / lengthSquared, 0.f, 1.f);
		const float nearestX = startX + projection * dx;
		const float nearestY = startY + projection * dy;
		return std::sqrt((x - nearestX) * (x - nearestX) + (y - nearestY) * (y - nearestY));
	}

	sf::Image makeIconImage()
	{
		sf::Image image({ 64u, 64u }, sf::Color::Transparent);
		for (unsigned int y = 0; y < 64; ++y)
		{
			for (unsigned int x = 0; x < 64; ++x)
			{
				const int distance = std::abs(static_cast<int>(x) - 32)
					+ std::abs(static_cast<int>(y) - 32);
				if (distance <= 27)
					image.setPixel({ x, y }, distance < 16
						? sf::Color(255, 235, 100) : sf::Color(220, 125, 35));
			}
		}
		return image;
	}

	sf::Image makeTriangleOutlineImage()
	{
		sf::Image image({ 64u, 64u }, sf::Color::Transparent);
		for (unsigned int y = 0; y < 64; ++y)
		{
			for (unsigned int x = 0; x < 64; ++x)
			{
				const float px = static_cast<float>(x);
				const float py = static_cast<float>(y);
				const bool onEdge =
					distanceToSegment(px, py, 32.f, 5.f, 7.f, 56.f) <= 2.f ||
					distanceToSegment(px, py, 7.f, 56.f, 57.f, 56.f) <= 2.f ||
					distanceToSegment(px, py, 57.f, 56.f, 32.f, 5.f) <= 2.f;
				if (onEdge)
					image.setPixel({ x, y }, sf::Color::White);
			}
		}
		return image;
	}

	sf::Image makeCheckMarkImage()
	{
		sf::Image image({ 64u, 64u }, sf::Color::Transparent);
		for (unsigned int y = 0; y < 64; ++y)
		{
			for (unsigned int x = 0; x < 64; ++x)
			{
				const float px = static_cast<float>(x);
				const float py = static_cast<float>(y);
				const bool onMark =
					distanceToSegment(px, py, 13.f, 33.f, 27.f, 48.f) <= 3.f ||
					distanceToSegment(px, py, 27.f, 48.f, 53.f, 16.f) <= 3.f;
				if (onMark)
					image.setPixel({ x, y }, sf::Color::White);
			}
		}
		return image;
	}

	sf::Image makePillImage(sf::Vector2u size, const sf::Color& color)
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

	sf::Image makeCircleImage(unsigned int diameter, const sf::Color& color)
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
}

ExampleAssets::ExampleAssets()
	: iconTexture(makeIconImage()),
	  sliderTrackTexture(sf::Image({ 1u, 1u }, sf::Color::White)),
	  triangleIndicatorTexture(makeTriangleOutlineImage()),
	  customCheckMarkTexture(makeCheckMarkImage()),
	  toggleOffTexture(makePillImage({ 96u, 48u }, sf::Color(80, 80, 80))),
	  toggleOnTexture(makePillImage({ 96u, 48u }, sf::Color(65, 170, 110))),
	  toggleKnobTexture(makeCircleImage(48u, sf::Color::White))
{
}

bool ExampleAssets::loadFont()
{
	return font.openFromFile("C:/Windows/Fonts/arial.ttf");
}
