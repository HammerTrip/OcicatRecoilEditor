#pragma once

#include "inc.h"

struct PatternPoint {
	float time;
	float x;
	float y;

	sf::CircleShape circle;

	PatternPoint ();

	void set_position (sf::Vector2f pos);
	void set_selected (bool bSelected);

	bool is_inside (sf::Vector2f point);
};

struct Pattern {
	std::vector<PatternPoint> points;
};

PatternPoint* get_selected_point (size_t index);