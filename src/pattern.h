#pragma once

#include "inc.h"

struct PatternPoint {
	size_t id;

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
	static const size_t MAX_NAME = 64;
	char name[MAX_NAME];

	std::vector<PatternPoint> points;

	Pattern ();
};

PatternPoint* get_selected_point (size_t index);