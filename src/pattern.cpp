#include "pattern.h"

PatternPoint::PatternPoint () {
	x = 0;
	y = 0;
	time = 0;

	float size = 10.f;
	circle.setOrigin(sf::Vector2f(size, size) / 2.f);
	circle.setRadius(size / 2.f);

	set_position({ x, y });
	set_selected(false);
}

void PatternPoint::set_position (sf::Vector2f pos) {
	circle.setPosition(pos + get_cross_center());
}

void PatternPoint::set_selected (bool bSelected) {
	static sf::Color cSelected = sf::Color(0x2dc44eff);
	static sf::Color cNot = sf::Color(0xb3507dff); 

	if (bSelected)
		circle.setFillColor(cSelected);
	else
		circle.setFillColor(cNot);
}

bool PatternPoint::is_inside (sf::Vector2f point) {
	const sf::Vector2f distance = sf::Vector2f(x, y) - point;
	const float radius = circle.getRadius();
	
	float distance_squared = distance.x * distance.x + distance.y * distance.y;

	bool result = distance_squared <= (radius * radius);

	return result;
}