#pragma once

#include <numbers>
#include <vector>
#include <cmath>
#include <glm/vec2.hpp>
#include "glube/glube_export.h"

namespace glube
{

    GLUBE_EXPORT std::vector<glm::vec2> triangle_fan_circle(const float radius, const std::size_t sides);

} // End namespace glube.