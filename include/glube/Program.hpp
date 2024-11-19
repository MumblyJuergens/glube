#pragma once

#include <utility>
#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glube/Shader.hpp"

namespace glube
{
    template <typename... T>
    concept ShaderClass = (std::is_same_v<Shader, T> && ...);

    class [[nodiscard]] Program final
    {
        GLuint inner;

        void attachRecursive() {}
        void detachRecursive() {}

        void attachRecursive(const ShaderClass auto &shader, const ShaderClass auto &...shaders)
        {
            glAttachShader(inner, *shader);
            attachRecursive(shaders...);
        }

        void detachRecursive(const ShaderClass auto &shader, const ShaderClass auto &...shaders)
        {
            glDetachShader(inner, *shader);
            detachRecursive(shaders...);
        }

        public:
        [[nodiscard]] Program() { inner = glCreateProgram(); }
        [[nodiscard]] Program(const std::string_view vertexSource, const std::string_view fragmentSource) : Program()
        {
            Shader vert(ShaderType::vertex, vertexSource);
            Shader frag(ShaderType::fragment, fragmentSource);
            add_shaders(vert, frag);
        }
        ~Program() { glDeleteProgram(inner); }
        [[nodiscard]] Program(Program &&other) noexcept : inner{ std::exchange(other.inner, 0) } {}
        Program &operator=(Program &&other) noexcept
        {
            inner = std::exchange(other.inner, 0);
            return *this;
        }
        Program(const Program &) = delete;
        Program &operator=(const Program &) = delete;
        [[nodiscard]] GLuint operator*() const noexcept { return inner; }
        void activate() const { glUseProgram(inner); }

        void add_shaders(const ShaderClass auto &...shaders)
        {
            attachRecursive(shaders...);
            glLinkProgram(inner);
            GLint isLinked = 0;
            glGetProgramiv(inner, GL_LINK_STATUS, &isLinked);
            if (!isLinked)
            {
                GLint maxLength = 0;
                glGetProgramiv(inner, GL_INFO_LOG_LENGTH, &maxLength);
                std::string infoLog(static_cast<std::size_t>(maxLength), '\0');
                glGetProgramInfoLog(inner, maxLength, &maxLength, infoLog.data());
                throw std::runtime_error{ infoLog };
            }
            detachRecursive(shaders...);
        }

        void set_uniform(const std::string_view name, const glm::mat4 &matrix)
        {
            const auto location = glGetUniformLocation(inner, name.data());
            glProgramUniformMatrix4fv(inner, location, 1, false, glm::value_ptr(matrix));
        }
        void set_uniform(const std::string_view name, const int value)
        {
            const auto location = glGetUniformLocation(inner, name.data());
            glProgramUniform1i(inner, location, value);
        }
        void set_uniform(const std::string_view name, const glm::vec3 value)
        {
            const auto location = glGetUniformLocation(inner, name.data());
            glProgramUniform3fv(inner, location, 1, glm::value_ptr(value));
        }
    };

    static_assert(!std::is_copy_assignable_v<Program>);
    static_assert(!std::is_copy_constructible_v<Program>);
    static_assert(std::is_move_assignable_v<Program>);
    static_assert(std::is_move_constructible_v<Program>);

} // End namespace glube.