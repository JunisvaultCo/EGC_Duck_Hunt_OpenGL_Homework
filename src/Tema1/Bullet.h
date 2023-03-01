#pragma once

#include "components/simple_scene.h"
#include "Tema1/ComplexVisual.h"
#include <vector>

using namespace std;
using namespace complexVisual;

namespace tema1
{
    class Bullet : public complexVisual::ComplexVisual
    {
     public:
        Bullet(double scale);
        ~Bullet();
        glm::vec3 getDimensions();

     private:
        double bulletScale;
     protected:
     

    };
}   // namespace m1
