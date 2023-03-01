#include "Tema1/Duck.h"
#include "Tema1/object2D.h"
#include "Tema1/transform2D.h"

using namespace object2D;
using namespace transform2D;
using namespace tema1;

Duck::Duck(double scale, double speed, int generation, int hp, bool isBoss, 
        double despawnTime) {
    glm::vec3 corner = glm::vec3(0, 0, 0);

    duckScale = scale * (isBoss ? 1.5 : 1);
    dir = 1;
    rotation = 0;
    maxFlap = 25;
    health = hp;
    isSpecial = isBoss;
    this->speed = speed;
    this->despawnTime = despawnTime;
    this->generation = generation;

    glm::vec3 brown = glm::vec3(0.25f, 0.15f, 0);
    glm::vec3 black = glm::vec3(0, 0, 0);
    glm::vec3 gray = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 green = glm::vec3(0, 0.6f, 0);
    glm::vec3 yellow = glm::vec3(0.8f, 0.8f, 0);
    glm::vec3 eyeBlue = glm::vec3(73.0 / 255, 13.0 / 255, 1);
    glm::vec3 orange = glm::vec3(1, 0.5f, 0);
    glm::vec3 red = glm::vec3(1, 0, 0);
    glm::vec3 darkRed = glm::vec3(0.7f, 0, 0);
    glm::vec3 white = glm::vec3(1, 1, 1);

    glm::vec3 bodyColour = (isSpecial) ? yellow : brown;
    glm::vec3 headColour = (isSpecial) ? white : green;

    // duck torso
    Mesh* equilateral1 = object2D::CreatePolygon("equilateral1",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(4.0, 0, 0),
                                                glm::vec3(2, 2 * sqrt(3), 0)
                                            },
                                            vector<glm::vec3> {bodyColour, bodyColour, black},
                                            true);
    // literally just changing the colours

    Mesh* equilateral2 = object2D::CreatePolygon("equilateral2",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(4, 0, 0),
                                                glm::vec3(2, 2 * sqrt(3), 0)
                                            },
                                            vector<glm::vec3> {black, bodyColour, bodyColour},
                                            true);
    Mesh* equilateral3 = object2D::CreatePolygon("equilateral3",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(4, 0, 0),
                                                glm::vec3(2, 2 * sqrt(3), 0)
                                            },
                                            vector<glm::vec3> {black, black, bodyColour},
                                            true);
    meshes.push_back({equilateral1, glm::mat3(1)});
    
    //first triangle out of left wing
    meshes.push_back({equilateral2, glm::mat3(1)});
    
    //second triangle of left wing
    meshes.push_back({equilateral3, glm::mat3(1)});
    
    //third triangle of left wing

    Mesh* right = object2D::CreatePolygon("right",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(-2, 0, 0),
                                                glm::vec3(-2, 2.5, 0)
                                            },
                                            vector<glm::vec3> {black, bodyColour, bodyColour},
                                            true);

    meshes.push_back({right, glm::mat3(1)});

    // fourth and last triangle of wing
    double sharedSide = sqrt((2.5 * 2.5) + 4);
    double angleFromLastTriangle = atan(2.5 / 2);
    
    double angle = M_PI / 6; // 30 degrees
    double thirdSideX = 4 * (cos(angle) + sin(angle));
    double thirdSideY = 4 * (cos(angle) - sin(angle));
    
    Mesh* uglyLastTriangle = object2D::CreatePolygon("ugly",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(-sharedSide, 0, 0),
                                                glm::vec3(-thirdSideX, thirdSideY, 0)
                                            },
                                            vector<glm::vec3> {black, bodyColour, bodyColour},
                                            true);

    meshes.push_back({uglyLastTriangle, glm::mat3(1)});

    // bottom

    vector<glm::vec3> bottomColors;
    if (!isSpecial) {
        bottomColors.push_back(brown);
        for (int i = 1; i <= 100; i++) {
            if (i >= 8 && i <= 42)
                bottomColors.push_back(brown * glm::vec3(0.8, 0.8, 0.8));
            else
                bottomColors.push_back(gray);
        }
    } else {
        bottomColors.push_back(white);
        for (int i = 1; i <= 100; i++) {
            if (i % 2)
                bottomColors.push_back(black);
            else
                bottomColors.push_back(gray);
        }
    }
    Mesh* bottom = object2D::CreateCircle("bottom",
                                        glm::vec3(0,0,0),
                                        2.25,
                                        bottomColors,
                                        true,
                                        100);

    meshes.push_back({bottom, glm::mat3(1), true, 2.25});

    // head

    Mesh* head = object2D::CreateCircle("head",
                                        glm::vec3(0,0,0),
                                        1.25,
                                        headColour,
                                        true,
                                        100);

    meshes.push_back({head, glm::mat3(1), true, 1.25});

    // beak
    
    Mesh* beak = object2D::CreatePolygon("beak",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(4, 0, 0),
                                                glm::vec3(2, 2 * sqrt(3), 0)
                                            },
                                            vector<glm::vec3> {yellow, yellow, yellow},
                                            true);
    meshes.push_back({beak, glm::mat3(1)});

    // eyes

    vector<glm::vec3> eyeColors {black};
    for (int i = 0; i < 100; i++)
        eyeColors.push_back((isSpecial) ? red : eyeBlue);
    Mesh* eye = object2D::CreateCircle("eye",
                                        glm::vec3(0, 0, 0),
                                        0.25,
                                        eyeColors,
                                        true,
                                        100);
    meshes.push_back({eye, glm::mat3(1), true, 0.25, false});
    meshes.push_back({eye, glm::mat3(1), true, 0.25, false});

    // legs
    Mesh* leg = object2D::CreatePolygon("leg",
                                            {
                                                glm::vec3(0, 0, 0),
                                                glm::vec3(-0.5, -0.5 * sqrt(3), 0),
                                                glm::vec3(0.5, -0.5 * sqrt(3), 0)
                                            },
                                            vector<glm::vec3> {orange, orange, orange},
                                            true);
    meshes.push_back({leg, glm::mat3(1)});
    meshes.push_back({leg, glm::mat3(1)});

    // mirrored
    meshes.push_back({equilateral2, glm::mat3(1)});
    meshes.push_back({equilateral3, glm::mat3(1)});
    meshes.push_back({right, glm::mat3(1)});
    meshes.push_back({uglyLastTriangle, glm::mat3(1)});

    // eyebrows (only visible on bosses)

    Mesh* eyebrow = object2D::CreateSquare("eyebrow",
                                            glm::vec3(0, 0, 0),
                                            1.0f,
                                            darkRed,
                                            true);
    meshes.push_back({eyebrow, glm::mat3(1), 1, false});
    meshes.push_back({eyebrow, glm::mat3(1), 1, false});

    meshDrawOrder = {16, 17, 11, 10, 8, 9, 6, 7, 5, 0, 15, 13, 12, 14, 4, 2, 1, 3};
    Duck::update(1, 0);
}

Duck::~Duck() {
    
}

void Duck::update(int count, ...) {
    va_list valist;
    if (count < 1)
        return;
    va_start(valist, count);
    double deltaTime = va_arg(valist, double);
    va_end(valist);
    if (deltaTime < 1000) {
        if (dir) {
            rotation += deltaTime * 30;
            if (rotation > maxFlap) {
                rotation = maxFlap; // don't go overboard if laggy
                dir = false;
            }
        } else {
            rotation -= deltaTime * 30;
            if (rotation < 0) {
                rotation = 0; // don't go overboard if laggy
                dir = true;
            }
        }
    }

    double rad = rotation / 180 * M_PI;

    // duck torso
    meshes[0].transform = Scale(duckScale, duckScale);

    // first triangle out of left wing
    glm::mat3 transform1 = Rotate(M_PI / 3); // 60 degrees
    transform1 *= Translate(duckScale / 2 + duckScale * 2, 0);
    transform1 *= Rotate(M_PI / 3 * 2); // 120 degrees
    transform1 *= Rotate(rad * 2); // extra rotation
    meshes[1].transform = transform1 * Scale(duckScale / 2, duckScale / 2);
    
    //second triangle of left wing
    glm::mat3 transform2 = Rotate(M_PI / 3); // 60 degrees
    transform2 *= Translate(duckScale * 2.5, 0);
    transform2 *= Rotate(M_PI / 3); // 60 degrees
    transform2 *= Rotate(rad * 2); // extra rotation

    meshes[2].transform = transform2 * Scale(duckScale / 2, duckScale / 2);

    //third triangle of left wing
    
    // what was necessary from the last triangle
    glm::mat3 transform3 = transform2;

    // delta from last triangle
    transform3 *= Translate(duckScale * 2, 0);
    transform3 *= Rotate(-M_PI / 3); // -60 degrees
    transform3 *= Rotate(rad); // extra rotation

    meshes[3].transform = transform3 * Scale(duckScale, duckScale);

    // fourth and last triangle of wing
    double angleFromLastTriangle = atan(2.5 / 2);
    
    glm::mat3 transform4 = transform3;
    transform4 *= Rotate(-angleFromLastTriangle);
    transform4 *= Rotate(rad / 2); // extra rotation

    meshes[4].transform = transform4 * Scale(duckScale, duckScale);

    // bottom
    glm::mat3 transform = Scale(1, 3.5 / 4.5);
    transform *= Translate(2 * duckScale, - 2.25 / 2 * duckScale);

    meshes[5].transform = transform * Scale(duckScale, duckScale);

    // head
    glm::mat3 headTransform = Translate(2 * duckScale, 2 * sqrt(3) * duckScale);

    meshes[6].transform = headTransform * Scale(duckScale, duckScale);

    // beak
    transform = Translate((2 - 0.25) * duckScale, (2 * sqrt(3) + 1.25 - 0.1) * duckScale);

    meshes[7].transform = transform * Scale(duckScale / 8, duckScale / 4);

    // eyes
    glm::mat3 eyeTransform1 = headTransform * Translate(-duckScale * 2.5 / 4, duckScale * 2.5 / 4);
    meshes[8].transform = eyeTransform1 * Scale(duckScale, duckScale / 2);

    glm::mat3 eyeTransform2 = headTransform * Translate(duckScale * 2.5 / 4, duckScale * 2.5 / 4);
    meshes[9].transform = eyeTransform2 * Scale(duckScale, duckScale / 2);

    // legs

    transform = Scale(1, 3.5 / 4.5);
    transform *= Translate(2 * duckScale, - 2.25 / 2 * duckScale);

    transform *= Rotate(M_PI + M_PI / 8);
    transform *= Translate((2.25 - 0.3) * duckScale, 0);
    // reverse the damage placing them on the ellipse
    // is going to do to their shape.
    transform *= Rotate(-M_PI - M_PI / 8);
    transform *= Scale(1, 4.5 / 3.5);
    transform *= Rotate(M_PI + M_PI / 8);
    transform *= Rotate(M_PI / 2);


    meshes[10].transform = transform * Scale(duckScale / 2, duckScale);

    transform = Scale(1, 3.5 / 4.5);
    transform *= Translate(2 * duckScale, - 2.25 / 2 * duckScale);

    transform *= Rotate(- M_PI / 8);
    transform *= Translate((2.25 - 0.3) * duckScale, 0);
    transform *= Rotate(M_PI / 8);
    transform *= Scale(1, 4.5 / 3.5);
    transform *= Rotate(- M_PI / 8);
    transform *= Rotate(M_PI / 2);

    meshes[11].transform = transform * Scale(duckScale / 2, duckScale);

    // mirror

    // mirror on OX and/or OY is basically scaling with -1 and/or 1 as parameters
    transform = Scale(-1, 1) * Translate(-duckScale * 4, 0);

    meshes[12].transform = transform * meshes[1].transform;
    meshes[13].transform = transform * meshes[2].transform;
    meshes[14].transform = transform * meshes[3].transform;
    meshes[15].transform = transform * meshes[4].transform;

    // eyebrows
    double eyebrowScale = (generation / 10.0) * 0.3 * duckScale + duckScale;
    double eyebrowAngle = min(M_PI / 3, generation / (10.0 * 18));
    if (!isSpecial)
        eyebrowScale = 0;
    // left
    transform = Translate(0.25f * duckScale, 0.25f * duckScale);
    transform *= Rotate(- eyebrowAngle) * Scale(-eyebrowScale, 0.2 * duckScale);
    meshes[16].transform = eyeTransform1 * transform;
    // right
    transform = Translate(-0.25f * duckScale, 0.25f * duckScale);
    transform *= Rotate(eyebrowAngle) * Scale(eyebrowScale, 0.2 * duckScale);
    meshes[17].transform = eyeTransform2 * transform;

    // center everything on the center of the bottom instead of "random"
    // point.
    glm::vec3 newCenter = meshes[5].transform * glm::vec3(0, 0, 1);
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].transform = Translate(-newCenter.x, -newCenter.y) * meshes[i].transform;
    }
}

glm::vec3 Duck::getDuckHeadCoords(glm::mat3 transform)
{
    glm::mat3 newTransform = transform * meshes[6].transform;
    return newTransform * glm::vec3(0, 0, 1);
}

double Duck::getDuckHeadRadius()
{
    return 1.25 * duckScale;
}

glm::vec3 Duck::getBottomCoords(glm::mat3 transform)
{
    glm::mat3 newTransform = transform * meshes[5].transform;
    return newTransform * glm::vec3(0, -2.25, 1);
}

double Duck::getSpeed() {
    return speed;
}
bool Duck::isBoss() {
    return isSpecial;
}
bool Duck::getHit() {
    health--;
    return (health <= 0);
}
double Duck::getDespawnTime() {
    return despawnTime;
}