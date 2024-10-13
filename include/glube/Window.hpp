#pragma once

#include <string_view>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "glube/Context.hpp"
#include "glube/keys.hpp"
#include "glube/glube_export.h"

namespace glube
{
    class [[nodiscard]] Window
    {
        Context Context;
        GLFWwindow *window{};
        KeyEventHandler *keyEventHandler{};
        void *userPointer{};

        GLUBE_EXPORT static void error_callback(int error, const char *description);
        GLUBE_EXPORT static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    public:
        [[nodiscard]] Window() noexcept = default;
        [[nodiscard]] Window(const int width, const int height, const std::string_view title) { init(width, height, title); }
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        [[nodiscard]] Window(Window &&other) noexcept
        {
            window = std::exchange(other.window, nullptr);
            keyEventHandler = std::exchange(other.keyEventHandler, nullptr);
            userPointer = std::exchange(other.userPointer, nullptr);
        }
        Window &operator=(Window &&other) noexcept
        {
            window = std::exchange(other.window, nullptr);
            keyEventHandler = std::exchange(other.keyEventHandler, nullptr);
            userPointer = std::exchange(other.userPointer, nullptr);
            return *this;
        }
        ~Window() { glfwDestroyWindow(window); }
        [[nodiscard]] GLUBE_EXPORT bool try_init(const int width, const int height, const std::string_view title);
        void init(const int width, const int height, const std::string_view title)
        {
            if (!try_init(width, height, title))
            {
                throw std::runtime_error{"Could not initialise window"};
            }
        }

        [[nodiscard]] GLFWwindow *get() const noexcept { return window; }
        [[nodiscard]] bool good() const noexcept { return window; }

        void swap_interval(const bool minimum) { glfwSwapInterval(minimum); }

        void set_key_event_handler(KeyEventHandler *handler) noexcept { keyEventHandler = handler; }

        [[nodiscard]] bool should_close() const { return glfwWindowShouldClose(window); }
        void set_should_close(const bool value) { glfwSetWindowShouldClose(window, value); }

        void swap_buffers() { glfwSwapBuffers(window); }
        void poll_events() { glfwPollEvents(); }

        [[nodiscard]] void *user_pointer() const noexcept { return userPointer; }
        void set_user_pointer(void *value) noexcept { userPointer = value; }
    };

} // End namespace glube.