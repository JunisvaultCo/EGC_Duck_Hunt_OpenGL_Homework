#include "Tema1/Bullet.h"
#include "Tema1/object2D.h"
#include "Tema1/transform2D.h"

using namespace object2D;
using namespace transform2D;
using namespace tema1;

Bullet::Bullet(double scale) {
    bulletScale = scale;

    glm::vec3 brown = glm::vec3(0.3f, 0.3f, 0);
    glm::vec3 lighterBrown = glm::vec3(0.5f, 0.5f, 0);
    glm::vec3 yellow = glm::vec3(0.8f, 0.8f, 0);

    // bullet cylinder
    Mesh* cylinder = object2D::CreatePolygon("heartBody",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(1.0 / 3, 0, 0),
                                                glm::vec3(1.0 / 3, 1, 0),
                                                glm::vec3(0, 1, 0),
                                            },
                                            vector<glm::vec3> {
                                                brown,
                                                lighterBrown,
                                                lighterBrown,
                                                brown
                                            },
                                            true);
    meshes.push_back({cylinder, Scale(bulletScale, bulletScale)});

    
    Mesh* circle = object2D::CreateCircle("circle",
                                        glm::vec3(0, 0, 0),
                                        0.5 / 3,
                                        yellow,
                                        true);
    meshes.push_back({circle, Scale(bulletScale, bulletScale) * Translate(0.5 / 3, 1)});

    meshDrawOrder = {0, 1};
}

Bullet::~Bullet() {
    
}

glm::vec3 Bullet::getDimensions() {
    double height = (1 + 1.0 / 3) * bulletScale;
    double width = bulletScale / 3.0;
    return glm::vec3(width, height, 0);
}