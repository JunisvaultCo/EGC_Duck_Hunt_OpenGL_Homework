#include "Tema1/Background.h"

using namespace object2D;
using namespace transform2D;
using namespace tema1;

Background::Background(double scale) {
    backgroundScale = scale;
    currentScale = 0;
    rotation = 0;
    showSun = false;

    glm::vec3 yellow = glm::vec3(1, 1, 0);
    glm::vec3 blue = glm::vec3(0, 0.3f, 0.8f);
    glm::vec3 black = glm::vec3(0, 0, 0);
    glm::vec3 red = glm::vec3(1, 0, 0);

    Mesh* sky = CreatePolygon("sky",
                                {
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(1, 0, 0),
                                    glm::vec3(1, 9.0 / 16, 0),
                                    glm::vec3(0, 9.0 / 16, 0),
                                },
                                vector<glm::vec3> {black, black, blue, blue},
                                true);
    meshes.push_back({sky, glm::mat3(1)});

    Mesh* octagram = CreateOctagram("octagram",
                                        vector<glm::vec3> {yellow},
                                        1.0,
                                        true);
    meshes.push_back({octagram, glm::mat3(1)});
    
    Mesh* sky2 = CreatePolygon("sky2",
                                {
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(1, 0, 0),
                                    glm::vec3(1, 9.0 / 16, 0),
                                    glm::vec3(0, 9.0 / 16, 0),
                                },
                                vector<glm::vec3> {yellow, yellow, red, red},
                                true);
    meshes.push_back({sky2, glm::mat3(1)});

    meshDrawOrder = {1, 0};
}

Background::~Background() {
    
}

void Background::update(int count, ...) {
    va_list valist;
    if (count < 1)
        return;
    va_start(valist, count);
    double deltaTime = va_arg(valist, double);
    if (count >= 2) {
        showSun = va_arg(valist, bool);
        currentScale = 0;
        if (showSun) {
            meshDrawOrder[1] = 2;
        } else {
            meshDrawOrder[1] = 0;
        }
    }
    va_end(valist);
    if (showSun)
        currentScale += deltaTime * (backgroundScale / 20.0);
    rotation += deltaTime;
    if (currentScale > backgroundScale / 4.0)
        currentScale = backgroundScale / 4.0;
    meshes[meshDrawOrder[1]].transform = Scale(backgroundScale, backgroundScale);
    meshes[1].transform = Translate(backgroundScale * 0.5, backgroundScale * 0.5 * 9 / 16) *
                            Scale(currentScale, currentScale) * Rotate(rotation);
}
