#pragma once

#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

void editor_init ();
void editor_draw (sf::RenderWindow& wnd);

struct PatternPoint {
	float time;
	float x;
	float y;

	sf::CircleShape circle;

	PatternPoint ();

	void set_position (sf::Vector2f pos);
	void set_selected (bool bSelected);
};

struct Pattern {
	std::vector<PatternPoint> points;
};