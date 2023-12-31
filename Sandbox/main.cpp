#include <Renderer/Renderer.h>
#include <Renderer/RenderPipeline/RenderPipeline.h>

#include <Renderer/RenderPipeline/ColorPass.h>
#include <Renderer/RenderPipeline/DirectionalShadowPass.h>
#include <Renderer/RenderPipeline/CubeMapPass.h>
#include <Renderer/RenderPipeline/ScreenPass.h>
#include <Renderer/RenderPipeline/CausticMapPass.h>

#include <Renderer/Graphics/Material.h>
#include <Renderer/Graphics/TextureManager.h>

#include <Renderer/Graphics/Camera.h>
#include <Renderer/Graphics/Model.h>

#include <Renderer/Graphics/DirectionalLight.h>

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

graphics::Model* createWaterPlane(graphics::Material* Watermaterial)
{
    int resolution = 500;

    std::vector<float> verts;
    std::vector<unsigned int> ind;
    std::vector<unsigned int> attribs = { 3 };

    for (int y = 0; y < resolution; y++)
    {
        for (int x = 0; x < resolution; x++)
        {
            float tx = (float)x / (float)(resolution - 1);
            float ty = (float)y / (float)(resolution - 1);
            verts.insert(verts.end(), { -0.5f + tx, 0, -0.5f + ty });
        }
    }

    for (int y = 0; y < resolution-1; y++)
    {
        for (int x = 0; x < resolution-1; x++)
        {
            unsigned int quad = y * resolution + x;
            ind.insert(ind.end(), { quad, quad + resolution, quad + resolution + 1 });
            ind.insert(ind.end(), { quad, quad + resolution + 1, quad + 1 });
            ind.insert(ind.end(), { quad + resolution + 1, quad + resolution, quad });
            ind.insert(ind.end(), { quad + 1, quad + resolution + 1, quad });
        }
    }

    return new graphics::Model(verts.data(), verts.size(), ind.data(), ind.size(), attribs.data(), attribs.size(), Watermaterial);
}

int main()
{
    window::Window *mainWindow = new window::Window("Demo");
    renderer::Renderer *mainRenderer = new renderer::Renderer(mainWindow);
    graphics::Scene mainScene;

    graphics::Shader* screenShader = graphics::ShaderManager::getInstance()->loadShader("res/shaders/screen.vert", "res/shaders/screen.frag");
    graphics::Material causticGenMaterial(graphics::ShaderManager::getInstance()->loadShader("res/shaders/causticGen.vert", "res/shaders/causticGen.frag"), 0);

    renderer::RenderPipeline pipeline;
    pipeline.addPass(new renderer::CausticMapPass(&mainScene, mainRenderer, &causticGenMaterial));
    pipeline.addPass(new renderer::CubeMapPass(&mainScene, mainRenderer));
    pipeline.addPass(new renderer::DirectionalShadowPass(&mainScene, mainRenderer));
    pipeline.addPass(new renderer::ColorPass(&mainScene, mainRenderer));
    pipeline.addPass(new renderer::ScreenPass(&mainScene, mainRenderer, screenShader));
    

    mainScene.setCamera(new graphics::Camera(mainWindow));
    
    //-------------------- Sun Light --------------------
    graphics::DirectionalLight* sun = new graphics::DirectionalLight(true);
    sun->color = { 1.0f, 0.96f, 0.83f };
    sun->transform.rotation = { 90.0f, 0.0f, 0.0f };
    sun->transform.position = { 0.0f, 10.0f, 10.0f };
    sun->ambientIntensity = 0.5f;
    sun->diffuseIntensity = 1.0f;

    //-------------------- Boat --------------------
    graphics::Material boatMaterial(graphics::ShaderManager::getInstance()->loadShader("res/shaders/boat.vert", "res/shaders/default.frag"));
    graphics::Model* boat = new graphics::Model("res/models/boat.glb", &boatMaterial);
    boat->transform.position = { 5,0,5 };

    //-------------------- Water --------------------
    graphics::Material waterMaterial(graphics::ShaderManager::getInstance()->loadShader("res/shaders/water.vert", "res/shaders/water.frag"), COLOR_PASS | CUBEMAP_PASS);
    float waterColor[] = { 0.16,0.83,1,0.5 };
    float time = 0.0f;
    std::vector<float> WaveDir(96, 0);
    {
        srand(0);
        for (int i = 0; i < 32; i++)
        {
            int number = rand() % 100;
            WaveDir[3 * i + 0] = (float)number / (float)100;
            number = rand() % 100;
            WaveDir[3 * i + 1] = (float)number / (float)100;
            number = rand() % 100;
            WaveDir[3 * i + 2] = ((float)number / (float)100) * 3;
        }
    }

    waterMaterial.setParamVec4("u_diffuseColor", &waterColor[0]);
    waterMaterial.setParamFloat("u_time", time);
    std::vector<std::string> uniforms(32, "");
    for (int i = 0; i < 96; i += 3)
    {
        int loc = i / 3;
        uniforms[loc] = "u_wave[" + std::to_string(loc) + "]";
        waterMaterial.setParamVec3(uniforms[loc].c_str(), &WaveDir[i]);
    }

    graphics::Model* water = createWaterPlane(&waterMaterial);
    water->transform.position = { 0,0,0 };
    water->transform.scale = { 50,50,50 };

    // Caustic Shader params
    causticGenMaterial.setParamFloat("u_time", time);
    for (int i = 0; i < 32; i ++)
    {
        causticGenMaterial.setParamVec3(uniforms[i].c_str(), &WaveDir[i]);
    }

    // Boat Shader params
    boatMaterial.setParamFloat("u_time", time);
    for (int i = 0; i < 32; i++)
    {
        boatMaterial.setParamVec3(uniforms[i].c_str(), &WaveDir[i]);
    }

    //-------------------- Sphere --------------------
    graphics::Model* sphere = new graphics::Model("res/models/sphere.fbx");
    sphere->transform.position = { 0,5,0 };

    //-------------------- Underwater Corals --------------------
    graphics::Material underwaterMaterial(graphics::ShaderManager::getInstance()->loadShader("res/shaders/underwater.vert", "res/shaders/underwater.frag"), COLOR_PASS | CAUSTIC_PASS | SHADOW_PASS);
    graphics::Model* underwater = new graphics::Model("res/models/underwater.glb", &underwaterMaterial);
    underwater->transform.rotation = { 90,0,0 };
    underwater->transform.position = { 0,-20,0 };
    underwater->transform.scale = { 2,2,2 };

    mainScene.addGraphics(boat);
    mainScene.addGraphics(water);
    mainScene.addGraphics(sphere);
    mainScene.addGraphics(underwater);

    mainScene.addLight(sun);

    float rot = 0;
    while (!mainWindow->isClosed())
    {
        pipeline.renderPipeline();

        //Update stuffs
        float deltaTime = mainWindow->getInputs()->getDeltaTime();
        rot += deltaTime * 2.0f;
        time += deltaTime * 2.0f;
        if (rot >= 360)
            rot -= 360;
        //sun->transform.rotation = { 160.0f, rot, 0 };
        waterMaterial.setParamFloat("u_time", time);
        causticGenMaterial.setParamFloat("u_time", time);
        boatMaterial.setParamFloat("u_time", time);

        mainScene.handleInput(mainWindow->getInputs());
    }

    graphics::ShaderManager::terminate();
    graphics::TextureManager::terminate();

    delete(mainRenderer);
    delete(mainWindow);
    return 0;
}