#include "Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "Tema1/transform2D.h"
#include "Tema1/object2D.h"

using namespace std;
using namespace m1;
using namespace object2D;
using namespace transform2D;
using namespace tema1;
using namespace std::chrono;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1(): duck(Duck(DUCK_SIZE, DUCK_BASE_SPEED, 0)), heart(Heart(HEART_SIZE)),
                bullet(Bullet(BULLET_SIZE)), score(Meter(SCORE_SIZE, MAX_SCORE)),
                background(Background(1)), evadeMeter(EVADE_SIZE, MAX_SCORE, WHITE, RED, GREEN, GRAY),
                healthMeter(EVADE_SIZE, MAX_SCORE, WHITE, GREEN, YELLOW, GRAY)
{
    missedBulletMesh = object2D::CreateCircle("lastBullet",
                                        glm::vec3(0, 0, 1),
                                        LAST_BULLET_SIZE,
                                        {glm::vec3(1, 0, 0)},
                                        true,
                                        20);
    hitBulletMesh = object2D::CreateCircle("lastBullet",
                                        glm::vec3(0, 0, 1),
                                        LAST_BULLET_SIZE,
                                        {glm::vec3(0, 1, 0)},
                                        true,
                                        20);
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    initDimension = resolution;
    background = Background(initDimension.x);
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    heartCount = MAX_BASE_HEARTS;
    bulletCount = MAX_BASE_BULLETS;
    generatedDucks = 0;

    system_clock::time_point current = std::chrono::system_clock::now();
    double currentMilli = current.time_since_epoch().count() / 10000;
    srand((int)(currentMilli));
    generateDuck();
    sentBullets = std::deque<sentBullet>();
}

glm::i64vec4 Tema1::calculateLogicSpace()
{
    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw

    long minn = std::min(resolution.x / 16, resolution.y / 9);
    long dy = resolution.y - minn * 9;
    long dx = resolution.x - minn * 16;
    return glm::i64vec4(dx / 2, dy / 2, minn * 16, minn * 9);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::i64vec4 logicSpace = calculateLogicSpace();
    glViewport(logicSpace.x, logicSpace.y, logicSpace.z, logicSpace.a);
}


void Tema1::Update(float deltaTimeSeconds)
{
    // one second lag isn't fair for player
    if (deltaTimeSeconds > 1) {
        for (auto i: background.meshDrawOrder) {
            auto tri = background.meshes[i];
            RenderMesh2D(tri.mesh, shaders["VertexColor"], tri.transform);
        }
        return;
    }

    glm::vec3 bulletBounds = bullet.getDimensions();
    glm::vec3 heartBounds = heart.getDimensions();
    glm::mat3 modelMatrix = Translate(5, initDimension.y - 5 - heartBounds.y);
    double maxWidth = (heartBounds.x > bulletBounds.x) ? heartBounds.x : bulletBounds.x;
    for (int i = 0; i < heartCount; i++) {
        for (auto j: heart.meshDrawOrder) {
            auto tri = heart.meshes[j];
            glm::mat3 newMat = modelMatrix * tri.transform;
            RenderMesh2D(tri.mesh, shaders["VertexColor"], newMat);
        }
        modelMatrix *= Translate(5 + maxWidth, 0);
    }
    modelMatrix = Translate(5, initDimension.y - 5 - heartBounds.y);
    modelMatrix *= Translate((-bulletBounds.x + heartBounds.x) / 2.0, -bulletBounds.y - 5);
    for (int i = 0; i < bulletCount; i++) {
        for (auto j: bullet.meshDrawOrder) {
            auto tri = bullet.meshes[j];
            glm::mat3 newMat = modelMatrix * tri.transform;
            RenderMesh2D(tri.mesh, shaders["VertexColor"], newMat);
        }
        modelMatrix *= Translate(5 + maxWidth, 0);
    }
    modelMatrix = Translate(initDimension.x - 5 - SCORE_SIZE, initDimension.y - 5 - SCORE_SIZE / 10.0);
    for (auto j: score.meshDrawOrder) {
        auto tri = score.meshes[j];
        glm::mat3 newMat = modelMatrix * tri.transform;
        RenderMesh2D(tri.mesh, shaders["VertexColor"], newMat);
    }
    system_clock::time_point current = std::chrono::system_clock::now();
    double currentPeriods = current.time_since_epoch().count() / 10000;
    double evadeSeconds = (currentPeriods - timeSinceGeneration) / 1000;
    evadeMeter.reset();
    evadeMeter.update(1, (int) ((duck.getDespawnTime() - evadeSeconds) * 1000));
    
    modelMatrix = Translate(initDimension.x - 5 - EVADE_SIZE, initDimension.y - 5 - (SCORE_SIZE + EVADE_SIZE) / 10.0);
    for (auto j: healthMeter.meshDrawOrder) {
        auto tri = healthMeter.meshes[j];
        glm::mat3 newMat = modelMatrix * tri.transform;
        RenderMesh2D(tri.mesh, shaders["VertexColor"], newMat);
    }
    modelMatrix *= Translate(0, - EVADE_SIZE / 10.0);
    for (auto j: evadeMeter.meshDrawOrder) {
        auto tri = evadeMeter.meshes[j];
        glm::mat3 newMat = modelMatrix * tri.transform;
        RenderMesh2D(tri.mesh, shaders["VertexColor"], newMat);
    }

    for (auto j: sentBullets) {
        double bulletElapse = std::min(BULLET_DESPAWN, (currentPeriods - j.time) / 1000);
        double newRad = (BULLET_DESPAWN - bulletElapse) / BULLET_DESPAWN;
        glm::mat3 bulletMat = Translate(j.coords.x, j.coords.y) * Scale(newRad, newRad);
        if (j.hit)
            RenderMesh(hitBulletMesh, shaders["VertexColor"], bulletMat);
        else
            RenderMesh(missedBulletMesh, shaders["VertexColor"], bulletMat);
    }
    while (!sentBullets.empty() && (currentPeriods - sentBullets[0].time) / 1000 > BULLET_DESPAWN) {
        sentBullets.pop_front();
    }
    // deal with the duck
    if (evadeSeconds > duck.getDespawnTime() && duckState == DUCK_DEFAULT) {
        startEvading();
    }
    duckWalked += deltaTimeSeconds * duck.getSpeed();
    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(lastDuckDirChange.x, lastDuckDirChange.y);
    modelMatrix *= Rotate(duckDir / 180.0 * M_PI);
    modelMatrix *= Translate(duckWalked, 0);
    modelMatrix *= Rotate(-M_PI / 2);

    duck.update(1, deltaTimeSeconds);
    glm::vec3 coords = duck.getDuckHeadCoords(modelMatrix);
    glm::vec3 bottomCoords = duck.getBottomCoords(modelMatrix);
    double r = duck.getDuckHeadRadius();
    if (duckState == DUCK_EVADING) {
        if (hitUpWall(bottomCoords, 0)) {
            heartCount--;
            if (generatedDucks % 10 == 0)
                heartCount = 0;
            if (heartCount == 0) {
                score.reset();
                generatedDucks = 0;
                heartCount = MAX_BASE_HEARTS;
            }
            generateDuck();
        }
    } else if (duckState == DUCK_DYING) {
        if (hitDownWall(bottomCoords, 0)) {
            score.update(1, (int) (INITIAL_SCORE_PER_DUCK * (std::log2(generatedDucks / DUCK_BOOST_ONCE + 1) + 1)));
            generateDuck();
        }
    } else {
        if ((!hitUpWall(coords, r) && lastHitWallY == WALL_UP)
         || (!hitDownWall(coords, r) && lastHitWallY == WALL_DOWN))
            lastHitWallY = WALL_NONE;
        if ((!hitLeftWall(coords, r) && lastHitWallX == WALL_LEFT)
         || (!hitRightWall(coords, r) && lastHitWallX == WALL_RIGHT))
            lastHitWallX = WALL_NONE;
        if ((hitUpWall(coords, r) && lastHitWallY != WALL_UP)
         || (hitLeftWall(coords, r) && lastHitWallX != WALL_LEFT)
         || (hitRightWall(coords, r) && lastHitWallX != WALL_RIGHT)
         || (hitDownWall(coords, r) && lastHitWallY != WALL_DOWN)) {
            mirror(modelMatrix * glm::vec3(0, 0, 1), coords);
        }
    }
    for (auto i: duck.meshDrawOrder) {
        auto tri = duck.meshes[i];
        glm::mat3 newMat = modelMatrix * tri.transform;
        RenderMesh2D(tri.mesh, shaders["VertexColor"], newMat);
    }
    background.update(1, deltaTimeSeconds);
    for (auto i: background.meshDrawOrder) {
        auto tri = background.meshes[i];
        RenderMesh2D(tri.mesh, shaders["VertexColor"], tri.transform);
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (bulletCount <= 0)
        return;
    bulletCount--;
    glm::i64vec4 logicBounds = calculateLogicSpace();
    double transformedX = ((mouseX - logicBounds.x) * initDimension.x) / logicBounds.z;
    double transformedY = initDimension.y - ((mouseY - logicBounds.y) * initDimension.y) / logicBounds.a;
    system_clock::time_point current = std::chrono::system_clock::now();
    double time = current.time_since_epoch().count() / 10000;
    sentBullets.push_back({glm::vec3(transformedX, transformedY, 1), time, false});
    if (duckState == DUCK_DYING)
        return;
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(lastDuckDirChange.x, lastDuckDirChange.y);
    modelMatrix *= Rotate(duckDir / 180.0 * M_PI);
    modelMatrix *= Translate(duckWalked, 0);
    modelMatrix *= Rotate(-M_PI / 2);

    glm::vec3 coords = glm::vec3(transformedX, transformedY, 1);
    if (duck.collide(coords, modelMatrix)) {
        sentBullets[sentBullets.size() - 1].hit = true;
        healthMeter.update(1, -1);
        if (!duck.getHit())
            return;
        killDuck();
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}


void Tema1::generateDuck()
{
    generatedDucks++;
    system_clock::time_point current = std::chrono::system_clock::now();
    timeSinceGeneration = current.time_since_epoch().count() / 10000;
    duckState = DUCK_DEFAULT;
    double duckSpeed = DUCK_BASE_SPEED + DUCK_SPEED_BOOST * (generatedDucks / DUCK_BOOST_ONCE);
    int health = 1;
    if (generatedDucks % 10 != 0) {
        duck = Duck(DUCK_SIZE, duckSpeed, generatedDucks);
        bulletCount = MAX_BASE_BULLETS;
        background.update(2, 0, false);
    } else {
        duck = Duck(DUCK_SIZE, duckSpeed / 2, generatedDucks,
                generatedDucks / 5, true, std::log2(generatedDucks / 5) * 5);
        bulletCount = (generatedDucks / 5) * 1.5;
        background.update(2, 0, true);
        health = generatedDucks / 5;
    }
    evadeMeter = Meter(EVADE_SIZE, duck.getDespawnTime() * 1000, WHITE, RED, GREEN, GRAY);
    evadeMeter.update(1, (int) (duck.getDespawnTime() * 1000));
    healthMeter = Meter(EVADE_SIZE, health, WHITE, GREEN, YELLOW, GRAY);
    healthMeter.update(1, health);
    duckWalked = 0;
    lastHitWallY = WALL_DOWN;
    lastHitWallX = WALL_NONE;
    // We will not generate 80 degrees
    // out of the 180 that would start the duck
    // with the upwards direction. Those are those
    // in the middle of the interval (70 to 110),
    // and those between (0 to 20) and (160 to 180).
    // They are too close to OX and OY.
    duckDir = rand() % 100 + 20;
    if (duckDir > 70)
        duckDir += 40;

    // This part should make the duck always spawns with the head
    // slightly off screen (but so that it doesn't bump right away)
    glm::mat3 transform = Rotate(duckDir / 180.0 * M_PI - M_PI / 2);
    double r = duck.getDuckHeadRadius();
    int deltaX = r + duck.getDuckHeadCoords(transform).x;
    int deltaY = r + duck.getDuckHeadCoords(transform).y;
    int newRange = initDimension.x - 2 * (r + duck.getDuckHeadCoords(transform).x);
    lastDuckDirChange = glm::vec3(rand() % newRange + deltaX, -deltaY, 0);
}

void Tema1::mirror(glm::vec3 oldPlace, glm::vec3 head)
{
    double r = duck.getDuckHeadRadius();
    if (hitLeftWall(head, r) && lastHitWallX != WALL_LEFT) {
        lastHitWallX = WALL_LEFT;
        duckDir = 180 - duckDir;
    } else if (hitRightWall(head, r) && lastHitWallX != WALL_RIGHT) {
        lastHitWallX = WALL_RIGHT;
        duckDir = 180 - duckDir;
    }
    
    if (hitDownWall(head, r) && lastHitWallY != WALL_DOWN) {
        lastHitWallY = WALL_DOWN;
        duckDir = - duckDir;
    } else if (hitUpWall(head, r) && lastHitWallY != WALL_UP) {
        lastHitWallY = WALL_UP;
        duckDir = - duckDir;
    }
    lastDuckDirChange = head;
    duckWalked = 0;
}

bool Tema1::hitUpWall(glm::vec3 coords, double r) {
    return coords.y + r > initDimension.y;
}

bool Tema1::hitDownWall(glm::vec3 coords, double r) {
    return coords.y - r < 0;
}

bool Tema1::hitRightWall(glm::vec3 coords, double r) {
    return coords.x + r > initDimension.x;
}

bool Tema1::hitLeftWall(glm::vec3 coords, double r) {
    return coords.x - r < 0;
}

void Tema1::startEvading() {
    duckState = DUCK_EVADING;
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(lastDuckDirChange.x, lastDuckDirChange.y);
    modelMatrix *= Rotate(duckDir / 180.0 * M_PI);
    modelMatrix *= Translate(duckWalked, 0);
    modelMatrix *= Rotate(-M_PI / 2);
    duckWalked = 0;
    lastDuckDirChange = modelMatrix * glm::vec3(0, 0, 1);
    duckDir = 90;
}

void Tema1::killDuck() {
    duckState = DUCK_DYING;
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(lastDuckDirChange.x, lastDuckDirChange.y);
    modelMatrix *= Rotate(duckDir / 180.0 * M_PI);
    modelMatrix *= Translate(duckWalked, 0);
    modelMatrix *= Rotate(-M_PI / 2);
    duckWalked = 0;
    lastDuckDirChange = modelMatrix * glm::vec3(0, 0, 1);
    duckDir = -90;
}