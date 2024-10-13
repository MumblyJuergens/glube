#pragma once

#include <utility>
#include <glad/gl.h>

namespace glube
{
    enum class ShaderType : GLenum
    {
        vertex = GL_VERTEX_SHADER,
        fragment = GL_FRAGMENT_SHADER,
    };

    class [[nodiscard]] Shader final
    {
        GLuint inner;

    public:
        [[nodiscard]] Shader(const ShaderType type) { inner = glCreateShader(std::to_underlying(type)); }
        [[nodiscard]] Shader(const ShaderType type, const std::string_view source) : Shader(type) { set_source(source); }
        ~Shader() { glDeleteShader(inner); }
        [[nodiscard]] Shader(Shader &&other) noexcept : inner{std::exchange(other.inner, 0)} {}
        Shader &operator=(Shader &&other) noexcept
        {
            inner = std::exchange(other.inner, 0);
            return *this;
        }
        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;
        [[nodiscard]] GLuint operator*() const noexcept { return inner; }

        void set_source(const std::string_view source)
        {
            const char *wrapper[] = {source.data()};
            glShaderSource(inner, 1, wrapper, nullptr);
            glCompileShader(inner);
            GLint isCompiled = 0;
            glGetShaderiv(inner, GL_COMPILE_STATUS, &isCompiled);
            if (!isCompiled)
            {
                GLint maxLength = 0;
                glGetShaderiv(inner, GL_INFO_LOG_LENGTH, &maxLength);
                std::string errorLog(static_cast<std::size_t>(maxLength), '\0');
                glGetShaderInfoLog(inner, maxLength, &maxLength, errorLog.data());
                glDeleteShader(inner);
                throw std::runtime_error{errorLog};
            }
        }
    };

} // End namespace glube.