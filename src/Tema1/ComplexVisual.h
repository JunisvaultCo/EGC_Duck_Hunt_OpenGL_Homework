#pragma once

#include "components/simple_scene.h"
#include "Tema1/transform2D.h"
#include <vector>
#include <cstdarg>
using namespace std;

namespace complexVisual {
    struct MeshWithState {
        Mesh *mesh;
        glm::mat3 transform;
        // consider a circle for collision purposes
        bool isCircle = false;
        // only gets used if it is a circle
        double radius;
        // slight optimisation
        bool collidable = true;
    };
    // consider this a template for more complex
    // visual objects
    class ComplexVisual {
     public:
        vector<MeshWithState> meshes;
        vector<int> meshDrawOrder;
        ComplexVisual();
        ~ComplexVisual();
        virtual void update(int count, ...);
        virtual bool collide(glm::vec3 point, glm::mat3 transform);
    };
}