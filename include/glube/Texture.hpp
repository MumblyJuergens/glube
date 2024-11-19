#pragma once

#include <utility>
#include <glad/gl.h>
#include <glm/vec2.hpp>
#include <gsl/span>

namespace glube
{
    struct TextureConfig
    {
        GLenum target{};
        GLenum internal_format{};
        GLenum format{};
        GLenum type{};

    };
    namespace textureconfigs
    {
        static constexpr TextureConfig D2R8{ .target = GL_TEXTURE_2D, .internal_format = GL_R8, .format = GL_RED, .type = GL_UNSIGNED_BYTE };
        static constexpr TextureConfig D2RGB8{ .target = GL_TEXTURE_2D, .internal_format = GL_RGB8, .format = GL_RGB, .type = GL_UNSIGNED_BYTE };
        static constexpr TextureConfig D2RGBA8{ .target = GL_TEXTURE_2D, .internal_format = GL_RGBA8, .format = GL_RGBA, .type = GL_UNSIGNED_BYTE };
    } // End namespace TextureConfigs.

    class [[nodiscard]] Texture final
    {
        GLuint inner{};

        public:
        [[nodiscard]] Texture() noexcept = default;
        template<glm::length_t N>
        [[nodiscard]] Texture(const TextureConfig &config, const glm::vec<N, int> size, const gsl::span<std::uint8_t> pixels, const int levels = 1) noexcept
        {
            init(config, size, pixels, levels);
        }
        ~Texture() noexcept { glDeleteTextures(1, &inner); }
        [[nodiscard]] Texture(Texture &&that) noexcept : inner{ std::exchange(that.inner, 0) } {}
        Texture &operator=(Texture &&that) noexcept
        {
            inner = std::exchange(that.inner, 0);
            return *this;
        }
        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;
        [[nodiscard]] GLuint operator*() const noexcept { return inner; }

        template<glm::length_t N>
        void init(const TextureConfig &config, const glm::vec<N, int> size, const gsl::span<std::uint8_t> pixels, const int levels = 1) noexcept
        {
            glCreateTextures(config.target, 1, &inner);

            if constexpr (N == 1) glTextureStorage1D(inner, levels, config.internal_format, size.x);
            else if constexpr (N == 2) glTextureStorage2D(inner, levels, config.internal_format, size.x, size.y);
            else if constexpr (N == 3) glTextureStorage2D(inner, levels, config.internal_format, size.x, size.y, size.z);

            if constexpr (N == 1) glTextureSubImage1D(inner, 0, 0, size.x, config.format, config.type, pixels.data());
            else if constexpr (N == 2) glTextureSubImage2D(inner, 0, 0, 0, size.x, size.y, config.format, config.type, pixels.data());
            else if constexpr (N == 3) glTextureSubImage3D(inner, 0, 0, 0, 0, size.x, size.y, size.z, config.format, config.type, pixels.data());
        }
    };

    static_assert(!std::is_copy_assignable_v<Texture>);
    static_assert(!std::is_copy_constructible_v<Texture>);
    static_assert(std::is_move_assignable_v<Texture>);
    static_assert(std::is_move_constructible_v<Texture>);

} /// End namespace glube.