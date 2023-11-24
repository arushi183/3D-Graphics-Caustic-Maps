#include <Renderer/Renderer.h>
#include <Renderer/RenderPipeline/RenderPipeline.h>
#include <Renderer/RenderPipeline/ColorPass.h>

#include <Renderer/Graphics/ShaderManager.h>
#include <Renderer/Graphics/TextureManager.h>

#include <Renderer/Graphics/Camera.h>
#include <Renderer/Graphics/Model.h>

#include <iostream>
#include <vector>

float Plane_verts[] = {
       0, 0.5f,    0.5f,  1,0, // top right
       0, 0.5f,   -0.5f,  0,0, // bottom right
       0,-0.5f,   -0.5f,  0,0, // bottom left
       0,-0.5f,    0.5f,  1,0  // top left 
};

unsigned int Plane_ind[] = {
    0,1,3,
    1,2,3
};

unsigned int Plane_vertAttrib[] = {
    3, 2
};

int main()
{
    window::Window *mainWindow = new window::Window(800, 450, "Demo");
    renderer::Renderer *mainRenderer = new renderer::Renderer(mainWindow);
    graphics::Scene mainScene;

    renderer::RenderPipeline pipeline;
    //pipeline.addPass(new renderer::DirectionalShadowPass(&mainScene, mainRenderer));
    pipeline.addPass(new renderer::ColorPass(&mainScene, mainRenderer));

    mainScene.setCamera(new graphics::Camera(mainWindow));
    mainScene.addGraphics(new graphics::Model(Plane_verts, 20, Plane_ind, 6, Plane_vertAttrib, 2));
    
    while (!mainWindow->isClosed())
    {
        pipeline.renderPipeline();

        mainScene.handleInput(mainWindow->getInputs());
    }

    graphics::ShaderManager::terminate();
    graphics::TextureManager::terminate();

    delete(mainRenderer);
    delete(mainWindow);
    return 0;
}