#pragma once

#include <string_view>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "glube/Context.hpp"
#include "glube/keys.hpp"

namespace glube
{
    class [[nodiscard]] Window
    {
        Context Context;
        GLFWwindow *window{};
        KeyEventHandler *keyEventHandler{};
        void *userPointer{};

        static void error_callback(int error, const char *description);
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    public:
        [[nodiscard]] Window() noexcept = default;
        [[nodiscard]] Window(const int width, const int height, const std::string_view title) { init(width, height, title); }
        [[nodiscard]] Window(Window &&other) noexcept;
        Window &operator=(Window &&other) noexcept;
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        [[nodiscard]] bool try_init(const int width, const int height, const std::string_view title);
        void init(const int width, const int height, const std::string_view title);
        ~Window();

        [[nodiscard]] GLFWwindow *get() const noexcept { return window; }
        [[nodiscard]] bool good() const noexcept { return window; }

        void SwapInterval(const bool minimum) { glfwSwapInterval(minimum); }

        void SetKeyEventHandler(KeyEventHandler *handler) noexcept { keyEventHandler = handler; }

        [[nodiscard]] bool ShouldClose() const { return glfwWindowShouldClose(window); }
        void SetShouldClose(const bool value) { glfwSetWindowShouldClose(window, value); }

        void SwapBuffers() { glfwSwapBuffers(window); }
        void PollEvents() { glfwPollEvents(); }

        [[nodiscard]] void *UserPointer() const noexcept { return userPointer; }
        void SetUserPointer(void *value) noexcept { userPointer = value; }
    };
} // End namespace glube.