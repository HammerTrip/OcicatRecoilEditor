import colorsys
import imgui
import imgui.core
import itertools
import math
import os
import sys

class Vec2:
	x: float
	y: float

	def __init__ (self, x: float, y: float):
		self.x = x
		self.y = y

class EditorState:
	points = list[Vec2]

	def __init__ (self):
		pass

EDITOR_STATE = EditorState()

WIN_OFFSET_Y = 18
CANVAS_POS = Vec2(600, WIN_OFFSET_Y + 4)
CANVAS_SIZE = Vec2(CANVAS_POS.x + 300, CANVAS_POS.y + 500)

def show_editor (os_win_size_x: int, os_win_size_y: int):
	window_flags = 0
	window_flags |= imgui.WINDOW_NO_COLLAPSE
	window_flags |= imgui.WINDOW_NO_MOVE
	window_flags |= imgui.WINDOW_NO_RESIZE
	window_flags |= imgui.WINDOW_NO_TITLE_BAR
	window_flags |= imgui.WINDOW_NO_SCROLLBAR

	imgui.core.set_next_window_position(0, WIN_OFFSET_Y)
	imgui.core.set_next_window_size(os_win_size_x, os_win_size_y - WIN_OFFSET_Y)

	if imgui.begin(label="## Editor", closable=False, flags=window_flags):
		draw_list = imgui.get_window_draw_list()

		CANVAS_COLOR = imgui.get_color_u32_rgba(1,1,1,1)
		draw_list.add_rect_filled(CANVAS_POS.x, CANVAS_POS.y, CANVAS_SIZE.x, CANVAS_SIZE.y, CANVAS_COLOR, 0, 0)

		imgui.end()

	if imgui.begin_main_menu_bar():
		if imgui.begin_menu("File", True):
			clicked_quit, selected_quit = imgui.menu_item(
				"Quit", "Cmd+Q", False, True
			)

			if clicked_quit:
				sys.exit(0)

			imgui.end_menu()
		imgui.end_main_menu_bar()