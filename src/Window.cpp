#include "glube/Window.hpp"
#include <stdexcept>
#include <print>
#include "glube/keys.hpp"

namespace glube
{

    namespace internal
    {
        void opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
    }

    void Window::error_callback(int error, const char *description)
    {
        std::println(stderr, "GLFW error {0}: {1}", error, description);
    }

    Window::Window(Window &&other) noexcept
    {
        window = std::exchange(other.window, nullptr);
        keyEventHandler = std::exchange(other.keyEventHandler, nullptr);
        userPointer = std::exchange(other.userPointer, nullptr);
    }

    Window &Window::operator=(Window &&other) noexcept
    {
        window = std::exchange(other.window, nullptr);
        keyEventHandler = std::exchange(other.keyEventHandler, nullptr);
        userPointer = std::exchange(other.userPointer, nullptr);
        return *this;
    }

    void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Window *me = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        if (me->keyEventHandler)
        {
            me->keyEventHandler({
                .window = me,
                .key = static_cast<Key>(key),
                .mod = static_cast<KeyMod>(mods),
                .action = static_cast<KeyAction>(action),
                .scancode = scancode,
            });
        }
    }

    bool Window::try_init(const int width, const int height, const std::string_view title)
    {
        glfwSetErrorCallback(error_callback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
        glfwSetKeyCallback(window, key_callback);
        glfwSetWindowUserPointer(window, this);
        glfwMakeContextCurrent(window);
        gladLoadGL(glfwGetProcAddress);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(internal::opengl_debug_callback, nullptr);
        return window;
    }

    void Window::init(const int width, const int height, const std::string_view title)
    {
        if (!try_init(width, height, title))
        {
            throw std::runtime_error{"Could not initialise window"};
        }
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
    }

} // End namespace glube.