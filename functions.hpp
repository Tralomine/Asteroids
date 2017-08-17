#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

void drawBounds(const sf::Text& drawable, sf::RenderTarget& target);

float dist(sf::Vector2f pos1, sf::Vector2f pos2);

sf::Vector2f normalize(sf::Vector2f v);

void resetView(sf::RenderTarget& target, sf::Sprite& backgroundTexture);
