#pragma once

#include "components/simple_scene.h"
#include "Tema1/ComplexVisual.h"
#include <vector>

using namespace std;
using namespace complexVisual;

namespace tema1
{
    class Duck : public complexVisual::ComplexVisual
    {
     public:
        Duck(double scale, double speed, int generation, int hp = 1,
               bool isBoss = false, double despawnTime = 5.0);
        ~Duck();
        // uses only first given parameter as deltaFrameTime
        void update(int count, ...) override;
        glm::vec3 getDuckHeadCoords(glm::mat3 transform);
        glm::vec3 getBottomCoords(glm::mat3 transform);
        double getDuckHeadRadius();
        double getSpeed();
        bool isBoss();
        // return whether the duck died
        bool getHit();
        double getDespawnTime();

     private:
        double duckScale;
        int maxFlap;
        double rotation;
        bool dir;
        int health;
        bool isSpecial;
        double speed;
        double despawnTime;
        int generation;

     protected:
     

    };
}   // namespace m1
