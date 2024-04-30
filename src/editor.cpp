#include "editor.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <imconfig-SFML.h>
#include <imgui-SFML.h>

#include <imgui.h>

static Pattern kPattern;

static bool bDialogueReset = false;
static bool bMoveSelectedPoint = false;
static size_t selectedPointIndex = 0;

static sf::RectangleShape bgRect;
static sf::RectangleShape bgLineHorizontal;
static sf::RectangleShape bgLineVertical;

static sf::Vector2f mousePosition;

const sf::Vector2f crossCenter { 200, 300 };
const sf::Vector2f crossSize { 400, 600 };
const float crossThickness = 2;

const float COORD_TO_SCR_MLP = 10.f;



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
	circle.setPosition(pos + bgRect.getPosition() + bgRect.getSize() / 2.f);
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



static void select_point (size_t index) {
	if (PatternPoint* p = get_selected_point(selectedPointIndex))
		p->set_selected(false);
	
	selectedPointIndex = index;

	if (PatternPoint* p = get_selected_point(selectedPointIndex))
		p->set_selected(true);
}

PatternPoint* get_selected_point (size_t index) {
	if (index >= kPattern.points.size())
		return nullptr;

	return &kPattern.points[index];
}

static void add_point () {
	PatternPoint newpoint;
	kPattern.points.push_back(newpoint);
	
	select_point(kPattern.points.size() - 1);
}

static void remove_point () {
	if (kPattern.points.size() == 0)
		return;

	if (selectedPointIndex >= kPattern.points.size())
		return;

	kPattern.points.erase(kPattern.points.begin() + selectedPointIndex);
}

static void draw_imgui () {
	ImGui::SetNextWindowPos({ crossSize.x, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ crossSize.x, crossSize.y }, ImGuiCond_Once);

	uint32_t flags = 0;

	if (ImGui::Begin("щсшсфе кусщшд увшещк", nullptr, flags)) {
		if (ImGui::Button("Clear pattern"))
			bDialogueReset = true;
		
		ImGui::NewLine();

		if (ImGui::Button("Add point"))
			add_point();

		ImGui::NewLine();

		ImGui::Text("Selected point: %zu", selectedPointIndex);

		if (selectedPointIndex < kPattern.points.size()) {
			PatternPoint& p = kPattern.points[selectedPointIndex];

			float x = p.x;
			float y = p.y;

			ImGui::SliderFloat("x", &x, -40.f, 40.f, "%.2f", 0);
			ImGui::SliderFloat("y", &y, -60.f, 60.f, "%.2f", 0);
			ImGui::SliderFloat("time", &p.time, 0, 15.f, "%.3f");

			p.x = x;
			p.y = y;

			p.set_position({ x * COORD_TO_SCR_MLP, y * COORD_TO_SCR_MLP });
		}
		
		ImGui::End();
	}
}

static void editor_draw_pattern (sf::RenderWindow& wnd) {
	wnd.draw(bgRect);
	wnd.draw(bgLineHorizontal);
	wnd.draw(bgLineVertical);

	for (int i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];

		wnd.draw(p.circle);
	}
}

void ocicat::editor_init () {
	bgRect.setPosition({ 0, 0 });
	bgRect.setSize({ crossSize.x, crossSize.y });
	bgRect.setFillColor(sf::Color::White);

	bgLineHorizontal.setPosition({ 0, crossSize.y / 2.f });
	bgLineHorizontal.setSize({ crossSize.x, crossThickness });
	bgLineHorizontal.setFillColor(sf::Color::Black);

	bgLineVertical.setPosition({ crossSize.x / 2, 0 });
	bgLineVertical.setSize({ crossThickness, crossSize.y });
	bgLineVertical.setFillColor(sf::Color::Black);
}

void ocicat::editor_draw (sf::RenderWindow& wnd) {
	draw_imgui();

	if (bDialogueReset) {
		if (ImGui::Begin("Are you sure?##1")) {
			if (ImGui::Button("Yes")) {
				bDialogueReset = false;
				kPattern = Pattern();
			}

			if (ImGui::Button("No"))
				bDialogueReset = false;
		}
	}

	if (bMoveSelectedPoint) {
		if (PatternPoint* p = get_selected_point(selectedPointIndex)) {
			sf::Vector2f pos = mousePosition - crossCenter;
			pos /= COORD_TO_SCR_MLP;

			p->x = pos.x;
			p->y = pos.y;
			p->set_position(pos * COORD_TO_SCR_MLP);
		}
	}

	editor_draw_pattern(wnd);
}

void ocicat::evnt_left_pressed () {
	sf::Vector2f mouseVec = mousePosition - crossCenter;
	mouseVec /= COORD_TO_SCR_MLP;

	for (size_t i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];

		if (p.is_inside(mouseVec) && i != selectedPointIndex) {
			select_point(i);
			return;
		}
	}

	for (size_t i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];

		if (p.is_inside(mouseVec) && i == selectedPointIndex) {
			bMoveSelectedPoint = true;
			return;
		}
	}
}

void ocicat::evnt_left_released () {
	bMoveSelectedPoint = false;
}

void ocicat::evnt_mouse_position (sf::Vector2i pos) {
	mousePosition.x = pos.x;
	mousePosition.y = pos.y;
}