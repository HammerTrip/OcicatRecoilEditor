#pragma once

#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace ocicat {
	void editor_init ();
	void editor_draw (sf::RenderWindow& wnd);

	void evnt_left_pressed ();
	void evnt_left_released ();
	void evnt_mouse_position (sf::Vector2i pos);
};

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