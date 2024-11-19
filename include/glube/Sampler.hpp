#pragma once

#include <utility>
#include <glad/gl.h>

namespace glube
{
    class [[nodiscard]] Sampler final
    {
        GLuint inner;

        public:
        [[nodiscard]] Sampler() { glCreateSamplers(1, &inner); }
        ~Sampler() { glDeleteSamplers(1, &inner); }
        [[nodiscard]] Sampler(Sampler &&that) noexcept : inner{ std::exchange(that.inner, 0) } {}
        Sampler &operator=(Sampler &&that) noexcept
        {
            inner = std::exchange(that.inner, 0);
            return *this;
        }
        Sampler(const Sampler &) = delete;
        Sampler &operator=(const Sampler &) = delete;
        [[nodiscard]] GLuint operator*() const noexcept { return inner; }

        void bind(const GLuint textureUnit) { glBindSampler(textureUnit, inner); }

    };

    static_assert(!std::is_copy_assignable_v<Sampler>);
    static_assert(!std::is_copy_constructible_v<Sampler>);
    static_assert(std::is_move_assignable_v<Sampler>);
    static_assert(std::is_move_constructible_v<Sampler>);

} // End namespace glube.