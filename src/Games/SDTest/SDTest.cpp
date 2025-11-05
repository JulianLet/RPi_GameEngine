#include "SDTest.h"

#include "Entities/Entity.h"
#include "Systems/Core/GameManager.h"

#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Systems/Debug/DebugManager.h"
#include "Systems/Resource/ResourceManager.h"

#include "Entities/Common/Prototyping/StaticCamera.h"
#include "Entities/Common/UI/UITextObject.h"

#include "Entities/Components/Render/CameraComponent.h"
#include "pico/stdlib.h"
#include <algorithm>
#include <cstring>

SDTest::SDTest(GameManager &manager) : Game("SDTest", manager), entityManager(myEntities, this), eventComponentSystem(myEntities, this), eventComponentSystemUI(myUIElements, this)
{
    auto& es = EventSystem::GetInstance();

    // --- World entities ---
    auto* camera = new StaticCamera(Vector2(65, 81), 1.f, 1.f);
    camRef = camera->GetComponent<CameraComponent>();
    es.DispatchEvent(EventSpawnEntity(camera));

    // --- UI elements ---
    // DebugManager::GetInstance().ClearLogs();
    ResourceManager& resources = ResourceManager::GetInstance();

    // Test 1: init correctly
    std::string initText = "init progressed"; 
    if (resources.sdManager.IsMounted()) 
    {
        initText = "init worked";
    }
    else
    {
        initText = "init failed";
    }

    DebugManager::GetInstance().Log(initText);

    std::string result = "result : " + std::to_string(resources.sdManager.Result());
    DebugManager::GetInstance().Log(result);

    // Test 1: /config.txt
    File configFile = resources.sdManager.Open("config.txt", FA_READ);
    std::string configText = "nothing worked";  // will hold file contents
    if (configFile.IsValid()) 
    {
        configText = configFile.Read();
        configFile.Close();
    }
    else
    {
        configText = "could not load file";
    }

    DebugManager::GetInstance().Log(configText);

    // Test 2: /myfolder/long_filename.txt
    File longnameFile = resources.sdManager.Open("myfolder/long_filename.txt", FA_READ);
    std::string longtext = "nothing worked";  // will hold file contents
    if (longnameFile.IsValid()) 
    {
        longtext = longnameFile.Read();
        longnameFile.Close();
    }
    else
    {
        longtext = "could not load file";
    }

    DebugManager::GetInstance().Log(longtext);

    DebugManager::GetInstance().PrintLogsOnScreen(myUIElements, Color::RED);
}

void SDTest::Update(Input &input, float deltaTime)
{
    inputSystem.Update(myEntities, input);
    actionSystem.Update(myEntities);
    entityManager.Update();
    uiUpdateSystem.Update(myUIElements, input, myGameManager);

    if (runGame)
    {
        aiSystem.Update(myEntities, deltaTime);
        movementSystem.Update(myEntities, deltaTime);
        physicsSystem.Update(myEntities, deltaTime);
        collisionSystem.Update(myEntities);
        physicsSystem.ResolveCollisions(myEntities, deltaTime);
        cameraSystem.Update(myEntities, deltaTime);
    }
}

void SDTest::Render(Renderer &renderer)
{
    renderer.Clear(backgroundColor);

    renderSystem.Render(myEntities, renderer, camRef);
    uiRenderSystem.Render(myUIElements, renderer); //ui on top of world
}