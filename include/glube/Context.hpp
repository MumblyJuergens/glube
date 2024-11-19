#pragma once

#include <stdexcept>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace glube
{
    class [[nodiscard]] Context final
    {
        public:
        [[nodiscard]] Context()
        {
            if (!glfwInit())
            {
                throw std::runtime_error{ "Could not initialise context" };
            }
        };
        [[nodiscard]] Context(Context &&other) noexcept = default;
        Context &operator=(Context &&other) noexcept = default;
        Context(const Context &) = delete;
        Context &operator=(const Context &) = delete;
        ~Context() { glfwTerminate(); }
    };

    static_assert(!std::is_copy_assignable_v<Context>);
    static_assert(!std::is_copy_constructible_v<Context>);
    static_assert(std::is_move_assignable_v<Context>);
    static_assert(std::is_move_constructible_v<Context>);

} // End namespace glube.