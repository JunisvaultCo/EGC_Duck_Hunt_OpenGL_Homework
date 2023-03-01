#pragma once

#include "components/simple_scene.h"
#include "Tema1/ComplexVisual.h"
#include "Tema1/object2D.h"
#include "Tema1/transform2D.h"
#include <vector>

using namespace std;
using namespace complexVisual;

namespace tema1
{
    class Background : public complexVisual::ComplexVisual
    {
     public:
        Background(double scale);
        ~Background();
        glm::vec3 getDimensions();
        // can be used with several parameters:
        // first is deltaTime, used to increase current scale
        // second is whether to start showing inner symbol 
        void update(int count, ...) override;

     private:
        double backgroundScale;
        double currentScale;
        double rotation;
        bool showSun;
     protected:
     

    };
}   // namespace m1
