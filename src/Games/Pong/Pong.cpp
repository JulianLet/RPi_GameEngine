#include "Pong.h"

#include "pico/stdlib.h"
#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Games/Pong/Entities/PongManager.h"
#include "Games/Pong/Entities/Ball.h"
#include "Games/Pong/Entities/Wall.h"
#include "Games/Pong/Entities/Paddle.h"
#include "Games/Pong/Entities/TextObject.h"
#include "Games/Pong/Entities/ScoreText.h"
#include "Games/Pong/Entities/CameraObject.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/CameraComponent.h"

#include <algorithm>

Pong::Pong(GameManager &manager) : Game("PONG", manager), actionSystem(this), entityManager(myEntities, this), eventComponentSystem(myEntities, this), eventComponentSystemUI(myUIElements, this)
{
    auto& es = EventSystem::GetInstance();

    // --- World entities ---

    // Pong manager
    es.DispatchEvent(EventSpawnEntity(new PongManager()));

    // Paddles
    es.DispatchEvent(EventSpawnEntity(new Paddle(this, Vector2(5, 65), KEYCODE::UP, KEYCODE::DOWN, true)));
    es.DispatchEvent(EventSpawnEntity(new Paddle(this, Vector2(120, 65), KEYCODE::LEFT, KEYCODE::RIGHT, false)));

    // Walls
    es.DispatchEvent(EventSpawnEntity(new Wall(Vector2(0, 0), Vector2(playingFieldSize.x, 30))));
    es.DispatchEvent(EventSpawnEntity(new Wall(Vector2(0, playingFieldSize.y + 30), Vector2(playingFieldSize.x, 35))));

    // Ball
    auto* ball = new Ball();
    ballTransform = ball->GetComponent<TransformComponent>();
    es.DispatchEvent(EventSpawnEntity(ball));

    // Camera
    auto* camera = new CameraObject(Vector2(65, 81), 1.3f);
    camRef = camera->GetComponent<CameraComponent>();
    es.DispatchEvent(EventSpawnEntity(camera));

    // --- UI elements ---
    myUIElements.push_back(std::make_unique<TextObject>(Vector2(57, 8), "PONG", Color::WHITE));
    myUIElements.push_back(std::make_unique<ScoreText>(Vector2(65, 18), Color::WHITE));
    myUIElements.push_back(std::make_unique<TextObject>(Vector2(80, 150), "by Julez", Color::WHITE));

    runGame = true;
}

void Pong::Update(Input &input, float deltaTime)
{
    inputSystem.Update(myEntities, input);
    actionSystem.Update(myEntities, *this);
    entityManager.Update();
    uiUpdateSystem.Update(myUIElements, input, myGameManager);

    if (runGame)
    {
        aiSystem.Update(myEntities, deltaTime);
        movementSystem.Update(myEntities, deltaTime);
        physicsSystem.Update(myEntities, deltaTime);
        collisionSystem.Update(myEntities);
        physicsSystem.ResolveCollisions(myEntities);
        cameraSystem.Update(myEntities, deltaTime);

        if (ballTransform->currentPosition.x < -5 || ballTransform->currentPosition.x > playingFieldSize.x + 5)
        {
            EventBallOOB e(ballTransform->currentPosition.x);
            EventSystem::GetInstance().DispatchEvent(e);
            ResetGame();
        }
    }
}

void Pong::Render(Renderer &renderer)
{
    renderer.Clear(playingFieldColor);

    renderSystem.Render(myEntities, renderer, camRef);
    uiRenderSystem.Render(myUIElements, renderer); //ui on top of world
}

void Pong::ResetGame()
{
    runGame = false;

    for (auto& e : myEntities)
    {
        e->Reset();
    }
}
 
Entity *Pong::GetBall()
{
    for (auto& e : myEntities)
    {
        if (e->tag == "Ball")
        {
            return e.get();
        }
    }
    return nullptr;
}
