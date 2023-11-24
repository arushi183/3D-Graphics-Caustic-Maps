#include "Renderer/Window/Input.h"

namespace window
{
	Input::Input()
	{
		for (int i = 0; i < KEY_ARR; i++)
			m_currentKeys[i] = 0;
		for (int i = 0; i < KEY_ARR; i++)
			m_prevKeys[i] = 0;

		m_currFrameTime = 0.0f;
		m_prevFrameTime = 0.0f;

		m_scroll[0] = 0;
		m_scroll[1] = 0;

		m_currentMouse[0] = -1;
		m_currentMouse[1] = -1;

		m_prevMouse[0] = -1;
		m_prevMouse[1] = -1;
	}

	bool Input::getKeyDown(key keycode) const
	{
		if (keycode >= KEY_BITS)
			return false;

		int index = keycode / 64;
		int shift = keycode % 64;
		unsigned long long mask = (unsigned long long)1 << (63 - shift);

		return (bool)(mask & m_currentKeys[index]);
	}

	bool Input::getKeyUp(key keycode) const
	{
		if (keycode >= KEY_BITS)
			return false;

		int index = keycode / 64;
		int shift = keycode % 64;
		unsigned long long mask = (unsigned long long)1 << (63 - shift);

		bool prev = (bool)(mask & m_prevKeys[index]);
		bool current = (bool)(mask & m_currentKeys[index]);

		return (prev != current && !current);
	}

	bool Input::getKeyPressed(key keycode) const
	{
		if (keycode >= KEY_BITS)
			return false;

		int index = keycode / 64;
		int shift = keycode % 64;
		unsigned long long mask = (unsigned long long)1 << (63 - shift);

		bool prev = (bool)(mask & m_prevKeys[index]);
		bool current = (bool)(mask & m_currentKeys[index]);

		return (prev != current && current);
	}

	float Input::getDeltaTime() const
	{
		return m_currFrameTime - m_prevFrameTime;
	}

	void Input::getMousePos(double& x, double& y) const
	{
		x = m_currentMouse[0];
		y = m_currentMouse[1];
	}

	void Input::getMouseDelta(double& x, double& y) const
	{
		if (m_prevMouse[0] == -1 || m_prevMouse[1] == -1)
		{
			x = 0;
			y = 0;
		}
		else
		{
			x = m_currentMouse[0] - m_prevMouse[0];
			y = m_currentMouse[1] - m_prevMouse[1];
		}
	}

	void Input::getScroll(double* y, double* x) const
	{
		*y = m_scroll[1];
		if (x != nullptr)
			*x = m_scroll[0];
	}

	void Input::swapInputs(float newFrameTime)
	{
		m_prevFrameTime = m_currFrameTime;
		m_currFrameTime = newFrameTime;

		m_prevMouse[0] = m_currentMouse[0];
		m_prevMouse[1] = m_currentMouse[1];

		for (int i = 0; i < KEY_ARR; i++)
			m_prevKeys[i] = m_currentKeys[i];
	}

	void Input::setMousePos(double x, double y)
	{
		m_currentMouse[0] = x;
		m_currentMouse[1] = y;
	}

	void Input::setScroll(double x, double y)
	{
		m_scroll[0] = x;
		m_scroll[1] = y;
	}

	void Input::updateKey(key keycode, bool state)
	{
		if (keycode >= KEY_BITS)
			return;

		int index = keycode / 64;
		int shift = keycode % 64;
		unsigned long long mask = (unsigned long long)1 << (63 - shift);

		if (state == true)
			m_currentKeys[index] = m_currentKeys[index] | mask;
		else
			m_currentKeys[index] = m_currentKeys[index] & (~mask);
	}
}