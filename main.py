import imgui

from imgui.integrations.sdl2 import SDL2Renderer
from editor import *
from sdl2 import *
import OpenGL.GL as gl
import ctypes
import imgui
import sys

sdl_window = None

OCICAT_WINDOW_TITLE = "Ocicat Recoil Editor"

def main():
	window, gl_context = impl_pysdl2_init()
	imgui.create_context()
	impl = SDL2Renderer(window)

	running = True
	event = SDL_Event()
	while running:
		while SDL_PollEvent(ctypes.byref(event)) != 0:
			if event.type == SDL_QUIT:
				running = False
				break
			impl.process_event(event)
		impl.process_inputs()

		imgui.new_frame()

		w = ctypes.c_int()
		h = ctypes.c_int()
		SDL_GetWindowSize(window, w, h)

		show_editor(w.value, h.value)
		# imgui.show_test_window()

		gl.glClearColor(1.0, 1.0, 1.0, 1)
		gl.glClear(gl.GL_COLOR_BUFFER_BIT)

		imgui.render()
		impl.render(imgui.get_draw_data())
		SDL_GL_SwapWindow(window)

	impl.shutdown()
	SDL_GL_DeleteContext(gl_context)
	SDL_DestroyWindow(window)
	SDL_Quit()

def impl_pysdl2_init():
	global sdl_window

	width, height = 1280, 720

	if SDL_Init(SDL_INIT_EVERYTHING) < 0:
		print(
			"Error: SDL could not initialize! SDL Error: "
			+ SDL_GetError().decode("utf-8")
		)
		sys.exit(1)

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)

	SDL_SetHint(SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK, b"1")
	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, b"1")

	sdl_window = SDL_CreateWindow(
		OCICAT_WINDOW_TITLE.encode("utf-8"),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE,
	)

	if sdl_window is None:
		print(
			"Error: Window could not be created! SDL Error: "
			+ SDL_GetError().decode("utf-8")
		)
		sys.exit(1)

	gl_context = SDL_GL_CreateContext(sdl_window)
	if gl_context is None:
		print(
			"Error: Cannot create OpenGL Context! SDL Error: "
			+ SDL_GetError().decode("utf-8")
		)
		sys.exit(1)

	SDL_GL_MakeCurrent(sdl_window, gl_context)
	if SDL_GL_SetSwapInterval(1) < 0:
		print(
			"Warning: Unable to set VSync! SDL Error: " + SDL_GetError().decode("utf-8")
		)
		sys.exit(1)

	return sdl_window, gl_context

if __name__ == "__main__":
	main()