#pragma once

#include <utility>
#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glube/Shader.hpp"

namespace glube
{
    template <typename... T>
    concept shader_stand_in = (std::is_same_v<Shader, T> && ...);

    class [[nodiscard]] Program final
    {
        GLuint inner;

        void attachRecursive() {}
        void detachRecursive() {}

        void attachRecursive(shader_stand_in auto &shader, shader_stand_in auto &...shaders)
        {
            glAttachShader(inner, *shader);
            attachRecursive(shaders...);
        }

        void detachRecursive(shader_stand_in auto &shader, shader_stand_in auto &...shaders)
        {
            glDetachShader(inner, *shader);
            detachRecursive(shaders...);
        }

    public:
        [[nodiscard]] Program() { inner = glCreateProgram(); }
        [[nodiscard]] Program(const std::string_view vertexSource, const std::string_view fragmentSource) : Program()
        {
            Shader vert(ShaderType::Vertex, vertexSource);
            Shader frag(ShaderType::Fragment, fragmentSource);
            AddShaders(vert, frag);
        }
        ~Program() { glDeleteProgram(inner); }
        [[nodiscard]] Program(Program &&other) noexcept : inner{std::exchange(other.inner, 0)} {}
        Program &operator=(Program &&other) noexcept
        {
            inner = std::exchange(other.inner, 0);
            return *this;
        }
        Program(const Program &) = delete;
        Program &operator=(const Program &) = delete;
        [[nodiscard]] GLuint operator*() const { return inner; }
        void Activate() const { glUseProgram(inner); }

        void AddShaders(shader_stand_in auto &...shaders)
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
                throw std::runtime_error{infoLog};
            }
            detachRecursive(shaders...);
        }

        void SetUniform(const std::string_view name, const glm::mat4 &matrix)
        {
            Activate();
            const auto location = glGetUniformLocation(inner, name.data());
            glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
        }
    };

} // End namespace glube.