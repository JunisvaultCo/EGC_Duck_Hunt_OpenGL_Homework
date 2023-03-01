#pragma once

#include "components/simple_scene.h"
#include "Tema1/Duck.h"
#include "Tema1/Heart.h"
#include "Tema1/Bullet.h"
#include "Tema1/Meter.h"
#include "Tema1/Background.h"
#include <time.h>
#include <ctime>
#include <chrono>
#include <deque>

#define DUCK_SIZE 25
#define DUCK_BASE_SPEED 150
#define DUCK_SPEED_BOOST 50
#define DUCK_BOOST_ONCE 5
#define INITIAL_SCORE_PER_DUCK 1
#define HEART_SIZE 25
#define BULLET_SIZE 25
#define SCORE_SIZE 500
#define EVADE_SIZE 200
#define MAX_SCORE 1000
#define MAX_BASE_HEARTS 3
#define MAX_BASE_BULLETS 3
#define BULLET_DESPAWN 0.5
#define LAST_BULLET_SIZE 10.0

// this is for the lastHitWall
#define WALL_NONE 0
#define WALL_RIGHT 1 
#define WALL_DOWN 2
#define WALL_UP 3
#define WALL_LEFT 4

// this is for duck state
#define DUCK_DEFAULT 0
#define DUCK_EVADING 1
#define DUCK_DYING 2

#define WHITE glm::vec3(1, 1, 1)
#define GREEN glm::vec3(0, 0.5f, 0)
#define RED glm::vec3(0.5f, 0, 0)
#define GRAY glm::vec3(0.3f, 0.3f, 0.3f)
#define YELLOW glm::vec3(1, 1, 0)

using namespace tema1;

namespace m1
{
    struct sentBullet {
        glm::vec3 coords;
        double time;
        bool hit;
    };
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::ivec2 initDimension;
        Duck duck;
        Heart heart;
        Bullet bullet;
        Meter score;
        Meter evadeMeter;
        Meter healthMeter;
        Background background;
        Mesh *missedBulletMesh;
        Mesh *hitBulletMesh;
        std::deque<sentBullet> sentBullets;
        int duckDir;
        double duckWalked;
        double timeSinceGeneration;
        glm::vec3 lastDuckDirChange;
        int lastHitWallX;
        int lastHitWallY;
        int duckState;
        int heartCount;
        int bulletCount;
        int generatedDucks;

        void generateDuck();
        void mirror(glm::vec3 oldPlace, glm::vec3 head);
        bool hitUpWall(glm::vec3 coords, double r);
        bool hitDownWall(glm::vec3 coords, double r);
        bool hitRightWall(glm::vec3 coords, double r);
        bool hitLeftWall(glm::vec3 coords, double r);
        void startEvading();
        void killDuck();
        glm::i64vec4 calculateLogicSpace();
    };
}   // namespace m1
