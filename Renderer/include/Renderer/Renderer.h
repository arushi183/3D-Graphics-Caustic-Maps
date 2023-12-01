#pragma once

#include "Window/Window.h"

namespace renderer
{
    class Renderer
    {
    public:
        Renderer(window::Window* window);
        ~Renderer();

        void render(unsigned int VAO, int elementCount);
        void renderLine(unsigned int VAO, int elementCount);

        void getUpdatedWindow(int& width, int& height);
    private:
        int m_framebuffer_width, m_framebuffer_height;

        void updateFrameBufferResolution(unsigned int width, unsigned int height);
    };

}