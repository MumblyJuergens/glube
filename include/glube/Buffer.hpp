#pragma once

#include <utility>
#include <glad/gl.h>

namespace glube
{
    // enum class BufferStorageFlags : GLbitfield
    // {
    //     dynamic_storage = GL_DYNAMIC_STORAGE_BIT,
    // };

    // constexpr BufferStorageFlags operator&(BufferStorageFlags a, BufferStorageFlags b) { return BufferStorageFlags(std::to_underlying(a) & std::to_underlying(b)); }
    // constexpr BufferStorageFlags operator|(BufferStorageFlags a, BufferStorageFlags b) { return BufferStorageFlags(std::to_underlying(a) | std::to_underlying(b)); }
    // constexpr BufferStorageFlags operator^(BufferStorageFlags a, BufferStorageFlags b) { return BufferStorageFlags(std::to_underlying(a) ^ std::to_underlying(b)); }
    // constexpr BufferStorageFlags operator~(BufferStorageFlags a) { return BufferStorageFlags(~std::to_underlying(a)); }

    enum class BufferDataUsage : GLenum
    {
        none,
        dynamic_draw = GL_DYNAMIC_DRAW,
    };

    class [[nodiscard]] Buffer final
    {
        GLuint inner;

        public:
        [[nodiscard]] Buffer() { glCreateBuffers(1, &inner); }
        [[nodiscard]] Buffer(const std::size_t size, const void *const data = nullptr, const BufferDataUsage usage = BufferDataUsage::dynamic_draw)
            : Buffer()
        {
            init(size, data, usage);
        }
        ~Buffer() { glDeleteBuffers(1, &inner); }
        [[nodiscard]] Buffer(Buffer &&other) noexcept : inner{ std::exchange(other.inner, 0) } {}
        Buffer &operator=(Buffer &&other) noexcept
        {
            inner = std::exchange(other.inner, 0);
            return *this;
        }
        Buffer(const Buffer &) = delete;
        Buffer &operator=(const Buffer &) = delete;
        [[nodiscard]] GLuint operator*() const noexcept { return inner; }

        void init(const std::size_t size, const void *const data = nullptr, const BufferDataUsage usage = BufferDataUsage::dynamic_draw)
        {
            glNamedBufferData(inner, size, data, std::to_underlying(usage));
        }

        void overwrite(const std::size_t size, const void *const data, const int offset = 0)
        {
            glNamedBufferSubData(inner, offset, size, data);
        }
    };

    static_assert(!std::is_copy_assignable_v<Buffer>);
    static_assert(!std::is_copy_constructible_v<Buffer>);
    static_assert(std::is_move_assignable_v<Buffer>);
    static_assert(std::is_move_constructible_v<Buffer>);

} // End namespace glube.