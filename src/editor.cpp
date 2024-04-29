#include "editor.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <imconfig-SFML.h>
#include <imgui-SFML.h>

#include <imgui.h>

static Pattern kPattern;

static bool bDialogueReset = false;
static size_t selectedPointIndex = 0;

static sf::RectangleShape bgRect;
static sf::RectangleShape bgLineHorizontal;
static sf::RectangleShape bgLineVertical;

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

static PatternPoint* get_selected_point (size_t index) {
	if (index >= kPattern.points.size())
		return nullptr;

	return &kPattern.points[index];
}

static void add_point () {
	if (PatternPoint* p = get_selected_point(selectedPointIndex))
		p->set_selected(false);

	PatternPoint newpoint;
	newpoint.set_selected(true);

	kPattern.points.push_back(newpoint);
	selectedPointIndex = kPattern.points.size() - 1;
}

static void remove_point () {
	if (kPattern.points.size() == 0)
		return;

	if (selectedPointIndex >= kPattern.points.size())
		return;

	kPattern.points.erase(kPattern.points.begin() + selectedPointIndex);
}

static void draw_imgui () {
	ImGui::SetNextWindowPos({ 400, 40 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 600 }, ImGuiCond_Once);

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

			p.set_position({ x * 10, y * 10 });
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

void editor_init () {
	bgRect.setPosition({ 10, 10 });
	bgRect.setSize({ 400, 600 });
	bgRect.setFillColor(sf::Color::White);

	bgLineHorizontal.setPosition({ 10, 10 + 600 / 2 });
	bgLineHorizontal.setSize({ 400, 2 });
	bgLineHorizontal.setFillColor(sf::Color::Black);

	bgLineVertical.setPosition({ 10 + 400 / 2, 10 });
	bgLineVertical.setSize({ 2, 600 });
	bgLineVertical.setFillColor(sf::Color::Black);
}

void editor_draw (sf::RenderWindow& wnd) {
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

	editor_draw_pattern(wnd);
}