#include <print>
#include "glube/glube.hpp"
#include "glube/Attributes.hpp"
#include "glube/Buffer.hpp"
#include "glube/Context.hpp"
#include "glube/keys.hpp"
#include "glube/Pool.hpp"
#include "glube/Program.hpp"
#include "glube/Shader.hpp"
#include "glube/Window.hpp"

namespace glube::internal
{
    void opengl_debug_callback([[maybe_unused]] GLenum source, GLenum type, [[maybe_unused]] GLuint id, GLenum severity, [[maybe_unused]] GLsizei length, const GLchar *message, [[maybe_unused]] const void *userParam)
    {
        std::println(stderr, "GL CALLBACK: {} type = {}, severity = {}, message = {}",
                     (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
    }
}