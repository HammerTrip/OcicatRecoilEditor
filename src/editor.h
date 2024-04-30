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

sf::Vector2f mouse_to_canvas ();
sf::Vector2f vec_to_canvas (sf::Vector2f vec);

void editor_clear_pattern ();
void editor_sort_points ();

void editor_save ();
void editor_load ();