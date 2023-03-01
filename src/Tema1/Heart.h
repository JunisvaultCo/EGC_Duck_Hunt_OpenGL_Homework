#pragma once

#include "components/simple_scene.h"
#include "Tema1/ComplexVisual.h"
#include <vector>

using namespace std;
using namespace complexVisual;

namespace tema1
{
    class Heart : public complexVisual::ComplexVisual
    {
     public:
        Heart(double scale);
        ~Heart();
        glm::vec3 getDimensions();

     private:
        double heartScale;
     protected:
     

    };
}   // namespace m1
