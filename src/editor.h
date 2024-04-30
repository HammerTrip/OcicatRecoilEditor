#pragma once

#include "inc.h"

#include <vector>

namespace ocicat {
	void editor_init ();
	void editor_draw (sf::RenderWindow& wnd);

	void evnt_left_pressed ();
	void evnt_left_released ();
	void evnt_mouse_position (sf::Vector2i pos);
};

void evnt_right_pressed ();

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

sf::Vector2f mouse_to_canvas ();
sf::Vector2f vec_to_canvas (sf::Vector2f vec);