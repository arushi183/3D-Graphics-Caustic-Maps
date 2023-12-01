#pragma once

#include <vector>
#include <functional>

#include "Input.h"
#include "KeyBindings.h"

struct GLFWwindow;

using FuncPtr = std::function<void(unsigned int, unsigned int)>;

namespace window
{
	class Window
	{
	public:
		Window(unsigned int width, unsigned int height, const char* title);
		Window(const char* title);
		~Window();

		bool isClosed();
		void registerInputs();

		void addSizeCallback(FuncPtr function);

		void getResolution(float& width, float& height);
		void updateResolution(int width, int height);

		const Input* getInputs() const;
	protected:
		GLFWwindow* m_window;
		unsigned int m_width, m_height;

		Input* m_inputs;

		std::vector<FuncPtr> m_size_callbacks;

		void updateCursorPos(double x, double y);
		void updateKey(int keycode, bool action);
		void updateScroll(double x, double y);

		friend void window_cursor_pos_callback(GLFWwindow*, double, double);
		friend void window_key_callback(GLFWwindow*, int, int, int, int);
		friend void window_mouse_button_callback(GLFWwindow*, int, int, int);
		friend void window_scroll_callback(GLFWwindow*, double, double);
	};

}