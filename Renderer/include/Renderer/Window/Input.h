#pragma once

namespace window
{
#define KEY_ARR 7
#define KEY_BITS 64 * KEY_ARR

	using key = const int;

	class Window;

	class Input
	{
	public:
		Input();

		bool getKeyDown(key keycode) const;
		bool getKeyUp(key keycode) const;
		bool getKeyPressed(key keycode) const;

		float getDeltaTime() const;

		void getMousePos(double& x, double& y) const;
		void getMouseDelta(double& x, double& y) const;
		void getScroll(double* y, double* x = nullptr) const;
	private:
		unsigned long long m_currentKeys[KEY_ARR];
		unsigned long long m_prevKeys[KEY_ARR];

		double m_currentMouse[2];
		double m_prevMouse[2];

		float m_prevFrameTime;
		float m_currFrameTime;

		double m_scroll[2];

		void swapInputs(float newFrameTime);
		void setMousePos(double x, double y);
		void setScroll(double x, double y);
		void updateKey(key keycode, bool state);

		friend class Window;
	};
}