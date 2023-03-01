#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(double translateX, double translateY)
    {
        return glm::transpose(glm::mat3(
            1, 0, translateX,
            0, 1, translateY,
            0, 0, 1 
        ));

    }

    // Scale matrix
    inline glm::mat3 Scale(double scaleX, double scaleY)
    {
        return glm::transpose(glm::mat3(
            scaleX, 0, 0,
            0, scaleY, 0,
            0, 0, 1 
        ));

    }

    // Rotate matrix
    inline glm::mat3 Rotate(double radians)
    {
        return glm::transpose(glm::mat3(
            cos(radians), -sin(radians), 0,
            sin(radians), cos(radians), 0,
            0, 0, 1 
        ));

    }
    
    inline double projection(glm::vec3 v0, glm::vec3 v1, glm::vec3 p)
    {
        glm::vec3 dv = Rotate(M_PI / 2) * (v1 - v0 + glm::vec3(0, 0, 1));
        glm::vec3 dp = p - v0;
        double distV = sqrt(dv.x * dv.x + dv.y * dv.y);
        double distP = sqrt(dp.x * dp.x + dp.y * dp.y);
        double scalar = dp.x * dv.x + dp.y * dv.y;
        return scalar / distV;
    }
}   // namespace transform2D
