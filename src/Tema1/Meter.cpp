#include "Tema1/Meter.h"
#include "Tema1/object2D.h"
#include "Tema1/transform2D.h"

using namespace object2D;
using namespace transform2D;
using namespace tema1;
using namespace glm;

Meter::Meter(double scale, int maxValue, vec3 wireColor, vec3 gradient1, vec3 gradient2, vec3 empty) {
    meterScale = scale;
    this->maxValue = maxValue;
    currentValue = 0;

    double width = scale * 10;

    // wireframe
    Mesh* wireframe = object2D::CreatePolygon("wireframe",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(1, 0, 0),
                                                glm::vec3(1, 0.1f, 0),
                                                glm::vec3(0, 0.1f, 0),
                                            },
                                            vector<glm::vec3> {wireColor, wireColor, wireColor, wireColor},
                                            false);
    meshes.push_back({wireframe, glm::mat3(1)});

    Mesh* full = object2D::CreatePolygon("filledScore",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(1, 0, 0),
                                                glm::vec3(1, 0.1f, 0),
                                                glm::vec3(0, 0.1f, 0),
                                            },
                                            vector<glm::vec3> {gradient1, gradient2, gradient2, gradient1},
                                            true);
    meshes.push_back({full, glm::mat3(1)});
                                            
    Mesh* lacking = object2D::CreatePolygon("emptyScore",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(1, 0, 0),
                                                glm::vec3(1, 0.1f, 0),
                                                glm::vec3(0, 0.1f, 0),
                                            },
                                            vector<glm::vec3> {empty, empty, empty, empty},
                                            true);
    meshes.push_back({lacking, glm::mat3(1)});
    
    meshDrawOrder = {0, 2, 1};
    
    Meter::update(1, 0);
}

Meter::Meter(double scale, int maxValue)
    : Meter(scale, maxValue, vec3(1, 1, 1), vec3(0, 0, 0.5f), vec3(1, 1, 1), vec3(0.3f, 0.3f, 0.3f)) {
}

Meter::~Meter() {
    
}

void Meter::update(int count, ...) {
    va_list valist;
    if (count < 1)
        return;
    va_start(valist, count);
    int addScore = va_arg(valist, int);
    va_end(valist);
    currentValue += addScore;
    if (currentValue > maxValue) {
        currentValue = maxValue;
    } else if (currentValue < 0)
        currentValue = 0;

    // wireframe
    meshes[0].transform = Scale(meterScale, meterScale);
    // filled
    meshes[1].transform = Scale(meterScale, meterScale);
    // empty
    meshes[2].transform = Translate(meterScale, 0) *
        Scale(meterScale * ((currentValue - maxValue) / maxValue), meterScale);
}

void Meter::reset() {
    currentValue = 0;
    update(1, 0);
}