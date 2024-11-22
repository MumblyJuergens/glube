#include "glube/utilities.hpp"

namespace glube
{

    GLUBE_EXPORT std::vector<glm::vec2> triangle_fan_circle(const float radius, const std::size_t sides)
    {
        std::vector<glm::vec2> vertices;
        vertices.reserve(sides + 2);
        vertices.push_back({ 0, 0 });

        static constexpr float twopi = std::numbers::pi_v<float> *2;
        const float sidesf = static_cast<float>(sides);
        for (std::size_t i{}; i < sides + 2; ++i)
        {
            const float val = static_cast<float>(i) * twopi / sidesf;
            vertices.push_back({
                    radius * std::cos(val),
                    radius * std::sin(val),
                });
        }
        return vertices;
    }

} // End namespace glube.