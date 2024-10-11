#pragma once

#include <utility>
#include <glad/gl.h>

namespace glube
{
    enum class BufferStorageFlags : GLbitfield
    {
        DynamicStorage = GL_DYNAMIC_STORAGE_BIT,
    };

    constexpr BufferStorageFlags operator&(BufferStorageFlags a, BufferStorageFlags b) { return BufferStorageFlags(std::to_underlying(a) & std::to_underlying(b)); }
    constexpr BufferStorageFlags operator|(BufferStorageFlags a, BufferStorageFlags b) { return BufferStorageFlags(std::to_underlying(a) | std::to_underlying(b)); }
    constexpr BufferStorageFlags operator^(BufferStorageFlags a, BufferStorageFlags b) { return BufferStorageFlags(std::to_underlying(a) ^ std::to_underlying(b)); }
    constexpr BufferStorageFlags operator~(BufferStorageFlags a) { return BufferStorageFlags(~std::to_underlying(a)); }

    class [[nodiscard]] Buffer final
    {
        GLuint inner;

    public:
        [[nodiscard]] Buffer() { glCreateBuffers(1, &inner); }
        [[nodiscard]] Buffer(std::size_t size, const void *const data = nullptr, BufferStorageFlags flags = BufferStorageFlags::DynamicStorage)
            : Buffer()
        {
            InitStorage(size, data, flags);
        }
        ~Buffer() { glDeleteBuffers(1, &inner); }
        [[nodiscard]] Buffer(Buffer &&other) noexcept : inner{std::exchange(other.inner, 0)} {}
        Buffer &operator=(Buffer &&other) noexcept
        {
            inner = std::exchange(other.inner, 0);
            return *this;
        }
        Buffer(const Buffer &) = delete;
        Buffer &operator=(const Buffer &) = delete;
        [[nodiscard]] GLuint operator*() const { return inner; }

        void InitStorage(std::size_t size, const void *const data = nullptr, BufferStorageFlags flags = BufferStorageFlags::DynamicStorage)
        {
            glNamedBufferStorage(inner, size, data, std::to_underlying(flags));
        }

        void Overwrite(const std::size_t size, const void *const data, const int offset = 0)
        {
            glNamedBufferSubData(inner, offset, size, data);
        }
    };

} // End namespace glube.