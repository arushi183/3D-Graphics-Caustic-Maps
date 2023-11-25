#include <Renderer/Renderer.h>
#include <Renderer/RenderPipeline/RenderPipeline.h>
#include <Renderer/RenderPipeline/ColorPass.h>
#include <Renderer/RenderPipeline/DirectionalShadowPass.h>
#include <Renderer/RenderPipeline/ScreenPass.h>

#include <Renderer/Graphics/Material.h>
#include <Renderer/Graphics/TextureManager.h>

#include <Renderer/Graphics/Camera.h>
#include <Renderer/Graphics/Model.h>

#include <Renderer/Graphics/DirectionalLight.h>

#include <iostream>
#include <vector>

std::vector<float> Plane_verts = {
        0.5f, 0, 0.5f,  1,0,  0,-1,0,// top right
        0.5f, 0,-0.5f,  0,0,  0,-1,0,// bottom right
       -0.5f, 0,-0.5f,  0,0,  0,-1,0,// bottom left
       -0.5f, 0, 0.5f,  1,0,  0,-1,0// top left 
};

std::vector<unsigned int> Plane_ind = {
    0,1,3,
    1,2,3
};

std::vector<unsigned int> Plane_vertAttrib = {
    3, 2, 3
};

int main()
{
    window::Window *mainWindow = new window::Window(800, 450, "Demo");
    renderer::Renderer *mainRenderer = new renderer::Renderer(mainWindow);
    graphics::Scene mainScene;

    graphics::Shader* screenShader = graphics::ShaderManager::getInstance()->loadShader("res/shaders/screen.vert", "res/shaders/screen.frag");

    renderer::RenderPipeline pipeline;
    pipeline.addPass(new renderer::DirectionalShadowPass(&mainScene, mainRenderer));
    pipeline.addPass(new renderer::ColorPass(&mainScene, mainRenderer));
    pipeline.addPass(new renderer::ScreenPass(mainRenderer, screenShader));

    mainScene.setCamera(new graphics::Camera(mainWindow));
    
    graphics::DirectionalLight* sun = new graphics::DirectionalLight(true);
    sun->color = { 1.0f, 0.96f, 0.83f };
    sun->transform.rotation = { 145.0f, 0.0f, 0.0f };
    sun->transform.position = { 0.0f, 15.0f, 15.0f };
    sun->ambientIntensity = 0.1f;
    sun->diffuseIntensity = 1.0f;

    graphics::Model* cycle = new graphics::Model("res/models/cycle.glb");
    cycle->transform.position = { 5,0,5 };

    float basecolor[] = { 1,1,1,1 };
    graphics::Material unTexturedMaterial(graphics::ShaderManager::getInstance()->loadShader("res/shaders/default.vert", "res/shaders/default_color.frag"));
    unTexturedMaterial.setParamVec4("u_diffuseColor", &basecolor[0]);

    graphics::Model* plane = new graphics::Model(Plane_verts.data(), Plane_verts.size(), Plane_ind.data(), Plane_ind.size(), Plane_vertAttrib.data(), Plane_vertAttrib.size(), &unTexturedMaterial);
    plane->transform.position = { 0,0,0 };
    plane->transform.scale = { 50,50,50 };

    graphics::Model* sphere = new graphics::Model("res/models/sphere.fbx", &unTexturedMaterial);
    sphere->transform.position = { 0,10,0 };

    mainScene.addGraphics(cycle);
    mainScene.addGraphics(plane);
    mainScene.addGraphics(sphere);

    mainScene.addLight(sun);

    float rot = 0;
    float angle = 0;
    while (!mainWindow->isClosed())
    {
        pipeline.renderPipeline();

        float deltaTime = mainWindow->getInputs()->getDeltaTime();
        rot += deltaTime * 50.0f;
        angle += deltaTime * 50.0f;
        if (rot >= 360)
            rot -= 360;
        sun->transform.rotation = { 145, rot, 0 };
       
        mainScene.handleInput(mainWindow->getInputs());
    }

    graphics::ShaderManager::terminate();
    graphics::TextureManager::terminate();

    delete(mainRenderer);
    delete(mainWindow);
    return 0;
}