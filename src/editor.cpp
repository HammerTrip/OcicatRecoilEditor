#include "editor.h"
#include "pattern.h"

static Pattern kPattern;

static bool bMoveSelectedPoint = false;

static bool bRequestedOpenPopup = false;
static sf::Vector2f popupMousePosition { 0, 0 };

static bool bRequestPatternClean = false;

static size_t selectedPointIndex = 0;

static sf::RectangleShape bgRect;
static sf::RectangleShape bgLineHorizontal;
static sf::RectangleShape bgLineVertical;

static sf::Vector2f mousePosition;

const sf::Vector2f crossCenter { 200, 300 };
const sf::Vector2f crossSize { 400, 600 };
const float crossThickness = 2;

const float COORD_TO_SCR_MLP = 10.f;

const char* CONTEXT_MENU_ID = "ContextMenu";
const char* CLEAR_MENU_ID = "ClearPattern";

const char* IMGUI_TITLE = "The great Editor selfness.";



sf::Vector2f get_cross_center () {
	return crossCenter;
}

sf::Vector2f vec_to_canvas (sf::Vector2f vec) {
	sf::Vector2f v = vec - crossCenter;
	v /= COORD_TO_SCR_MLP;

	return v;
}

sf::Vector2f mouse_to_canvas () {
	return vec_to_canvas(mousePosition);
}

void editor_clear_pattern () {
	kPattern = Pattern();
	selectedPointIndex = 0;
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

static void add_point_at_position (sf::Vector2f pos) {
	add_point();

	if (PatternPoint* p = get_selected_point(selectedPointIndex)) {
		auto v = pos;

		p->x = v.x;
		p->y = v.y;

		p->set_position(v * COORD_TO_SCR_MLP);
	}
}

static void remove_point () {
	if (kPattern.points.size() == 0)
		return;

	if (selectedPointIndex >= kPattern.points.size())
		return;

	kPattern.points.erase(kPattern.points.begin() + selectedPointIndex);
}

static void draw_imgui () {
	ImGui::SetNextWindowPos({ crossSize.x, 0 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ crossSize.x, crossSize.y }, ImGuiCond_FirstUseEver);

	uint32_t flags = 0;

	if (ImGui::Begin(IMGUI_TITLE, nullptr, flags)) {
		if (ImGui::Button("Clear pattern"))
			ImGui::OpenPopup(CLEAR_MENU_ID);
		
		if (ImGui::BeginPopup(CLEAR_MENU_ID)) {
			ImGui::Text("Are you sure?");

			ImGui::NewLine();

			if (ImGui::Selectable("Yes"))
				bRequestPatternClean = true;

			ImGui::NewLine();
			
			ImGui::Selectable("No");

			ImGui::EndPopup();
		}
		
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

		if (bRequestedOpenPopup) {
			ImGui::OpenPopup(CONTEXT_MENU_ID);
			bRequestedOpenPopup = false;
		}

		if (ImGui::BeginPopup(CONTEXT_MENU_ID)) {
			if (ImGui::Selectable("Make point here"))
				add_point_at_position(vec_to_canvas(popupMousePosition));
			
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}

static void draw_arrow (PatternPoint& a, PatternPoint& b, sf::RenderWindow& wnd) {
	const auto va = a.circle.getPosition();
	const auto vb = b.circle.getPosition();

	const size_t VTX_COUNT = 2;

	sf::VertexArray lines = sf::VertexArray(sf::LinesStrip, VTX_COUNT);
	lines[0].position = va;
	lines[1].position = vb;

	for (size_t i = 0; i < VTX_COUNT; i++)
		lines[i].color = sf::Color::Black;

	wnd.draw(lines);
}

static void editor_draw_pattern (sf::RenderWindow& wnd) {
	wnd.draw(bgRect);
	wnd.draw(bgLineHorizontal);
	wnd.draw(bgLineVertical);

	for (int i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];

		wnd.draw(p.circle);
	}

	for (int i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];

		wnd.draw(p.circle);

		if (i > 0) {
			draw_arrow(kPattern.points[i - 1], p, wnd);
		}
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

	if (bRequestPatternClean) {
		bRequestPatternClean = false;
		editor_clear_pattern();
	}

	if (bMoveSelectedPoint) {
		if (PatternPoint* p = get_selected_point(selectedPointIndex)) {
			sf::Vector2f pos = mouse_to_canvas();

			p->x = pos.x;
			p->y = pos.y;
			p->set_position(pos * COORD_TO_SCR_MLP);
		}
	}

	editor_draw_pattern(wnd);
}

void ocicat::evnt_left_pressed () {
	auto mouseVec = mouse_to_canvas();

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

void evnt_right_pressed () {
	bRequestedOpenPopup = true;
	popupMousePosition = mousePosition;
}