#include "Tema1/Heart.h"
#include "Tema1/object2D.h"
#include "Tema1/transform2D.h"

using namespace object2D;
using namespace transform2D;
using namespace tema1;

Heart::Heart(double scale) {
    heartScale = scale;

    glm::vec3 pink = glm::vec3(0.7f, 0.2f, 0.2f);

    double height = sqrt(3) / 2 - 0.25;

    // heart body
    Mesh* body = object2D::CreatePolygon("heartBody",
                                        {
                                            glm::vec3(0.5, 0, 0),
                                            glm::vec3(0, height, 0),
                                            glm::vec3(1, height, 0)
                                        },
                                        vector<glm::vec3> {pink, pink, pink},
                                        true);
    meshes.push_back({body, Scale(heartScale, heartScale)});

    
    Mesh* circle = object2D::CreateCircle("circle",
                                        glm::vec3(0, 0, 0),
                                        0.25,
                                        pink,
                                        true,
                                        16,
                                        2);
    meshes.push_back({circle, Scale(heartScale, heartScale) * Translate(0.25, height)});
    meshes.push_back({circle, Scale(heartScale, heartScale) * Translate(0.75, height)});

    meshDrawOrder = {0, 1, 2};
}

Heart::~Heart() {
    
}


glm::vec3 Heart::getDimensions() {
    double height = (sqrt(3) / 2 + 0.25) * heartScale;
    double width = heartScale;
    return glm::vec3(width, height, 0);
}