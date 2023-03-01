#include "Tema1/ComplexVisual.h"

using namespace complexVisual;

ComplexVisual::ComplexVisual() {}
ComplexVisual::~ComplexVisual() {}
void ComplexVisual::update(int count, ...) {}

// SAT implementation oh yeah
bool ComplexVisual::collide(glm::vec3 point, glm::mat3 transform)
{
    for (auto j: meshes) {
        if (!j.collidable)
            continue;
        glm::vec3 pointInMeshSpace = (glm::inverse(transform * j.transform)) * point;
        if (j.isCircle) {
            double newX = pointInMeshSpace.x;
            double newY = pointInMeshSpace.y;
            double dist = sqrt(newX * newX + newY * newY);
            if (dist < j.radius) {
                return true;
            }
        } else {
            unsigned int length = j.mesh->vertices.size();
            // loop over the sides
            double found = false;
            for (unsigned int k = 0; k < length; k++) {
                glm::vec3 current = j.mesh->vertices[k].position;
                glm::vec3 next = j.mesh->vertices[(k + 1) % length].position;
                glm::vec3 dPoint = pointInMeshSpace;
                double projectionPoint = transform2D::projection(current, next, dPoint);
                double smallest = INFINITY;
                double biggest = -INFINITY;

                for (auto o : j.mesh->vertices) {
                    double currentProjection = transform2D::projection(current, next, o.position);
                    smallest = min(currentProjection, smallest);
                    biggest = max(currentProjection, biggest);
                }
                if (projectionPoint < smallest || projectionPoint > biggest) {
                    found = true;
                    break;
                }
            }
            if (!found)
                return true;
        }
    }
    return false;
}