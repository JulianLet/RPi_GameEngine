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
    DebugManager::GetInstance().ClearLogs();
    ResourceManager& resources = ResourceManager::GetInstance();
    uint8_t buffer[128];

        // Test 1: init correctly
    memset(buffer, 0, sizeof(buffer));
    std::string initText = "init progressed"; 
    if (resources.Initialize()) 
    {
        initText = "init worked";
    }
    else
    {
        initText = "init failed";
    }

    auto initTextObj = std::make_unique<UITextObject>(Vector2(2, 37), initText.c_str(), Color::WHITE, -1);
    myUIElements.emplace_back(std::move(initTextObj));

    // Test 1: /config.txt
    memset(buffer, 0, sizeof(buffer));
    File configFile = resources.sdManager.Open("config.txt");
    std::string configText = "nothing worked";  // will hold file contents
    if (configFile.IsValid()) 
    {
        size_t bytesRead = resources.sdManager.Read(configFile, buffer, sizeof(buffer) - 1);
        buffer[bytesRead] = '\0';
        configText = reinterpret_cast<char*>(buffer); // convert buffer to string
        resources.sdManager.Close(configFile);
    }
    else
    {
        configText = "could not load file";
    }

    auto pongButtonText = std::make_unique<UITextObject>(Vector2(2, 67), configText.c_str(), Color::WHITE, -1);
    myUIElements.emplace_back(std::move(pongButtonText));

    // Test 2: /myfolder/long_filename.txt
    memset(buffer, 0, sizeof(buffer));
    File longnameFile = resources.sdManager.Open("myfolder/long_filename.txt");
    std::string longtext = "nothing worked";  // will hold file contents
    if (longnameFile.IsValid()) 
    {
        size_t bytesRead = resources.sdManager.Read(longnameFile, buffer, sizeof(buffer) - 1);
        buffer[bytesRead] = '\0';
        longtext = reinterpret_cast<char*>(buffer); // convert buffer to string
        resources.sdManager.Close(longnameFile);
    }
    else
    {
        longtext = "could not load file";
    }

    auto jnrButtonText = std::make_unique<UITextObject>(Vector2(2, 97),longtext.c_str(), Color::WHITE, -1);
    myUIElements.emplace_back(std::move(jnrButtonText));

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