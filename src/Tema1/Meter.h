#pragma once

#include "components/simple_scene.h"
#include "Tema1/ComplexVisual.h"
#include <vector>

using namespace std;
using namespace complexVisual;
using namespace glm;

namespace tema1
{
    class Meter : public complexVisual::ComplexVisual
    {
     public:
        // uses default colors
        Meter(double scale, int maxValue);
        Meter(double scale, int maxValue, vec3 wireColor, vec3 gradient1, vec3 gradient2, vec3 empty);
        ~Meter();
        vec3 getDimensions();
        // Really requires only one parameter
        // which is by how much to increment
        void update(int count, ...) override;
        void reset();

     private:
        double meterScale;
        double currentValue;
        int maxValue;
     protected:
     

    };
}   // namespace m1
