#pragma once

#include <numbers>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <imconfig-SFML.h>
#include <imgui-SFML.h>

#include <imgui.h>

sf::Vector2f get_cross_center ();

inline float deg_to_rad (float deg) {
	const float pi = std::numbers::pi_v<float>;
	return deg * (pi / 180.f);
}