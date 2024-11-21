#pragma once

#include <stdexcept>
#include <type_traits>
#include <string_view>
#include <glad/gl.h>
#include "Buffer.hpp"
#include "Program.hpp"

namespace glube
{
    enum class AttributeType : GLenum
    {
        auto_calc = 0,
        int8 = GL_BYTE,
        uint8 = GL_UNSIGNED_BYTE,
        int16 = GL_SHORT,
        uint16 = GL_UNSIGNED_SHORT,
        int32 = GL_INT,
        uint32 = GL_UNSIGNED_INT,
        float32 = GL_FLOAT,
        float64 = GL_DOUBLE,

        // Can't auto-detect... yet...
        fixed_point = GL_FIXED,
        float16 = GL_HALF_FLOAT,
        int32_2_10_10_10_rev = GL_INT_2_10_10_10_REV,
        uint32_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV,
        uint32_10F_11F_11F_rev = GL_UNSIGNED_INT_10F_11F_11F_REV,
    };

    struct [[nodiscard]] AttributeConfig final
    {
        AttributeType type = AttributeType::auto_calc;
        bool normalize = false;
        int count = 0;
        int binding_index = 0;
        int buffer_offset = 0;
        int divisor = 0;
    };

    class [[nodiscard]] Attributes final
    {
        GLuint vao{};

        template <typename T>
        static constexpr AttributeType infer_gl_type()
        {
            using X = std::remove_all_extents_t<std::remove_cvref_t<T>>;
            if constexpr (sizeof(X) == 1)
                if constexpr (std::is_signed_v<T>)
                    return AttributeType::int8;
                else
                    return AttributeType::uint8;
            else if constexpr (std::is_same_v<X, std::int16_t>)
                return AttributeType::int16;
            else if constexpr (std::is_same_v<X, std::uint16_t>)
                return AttributeType::uint16;
            else if constexpr (std::is_same_v<X, std::int32_t>)
                return AttributeType::int32;
            else if constexpr (std::is_same_v<X, std::uint32_t>)
                return AttributeType::uint32;
            else if constexpr (std::is_same_v<X, std::float_t>)
                return AttributeType::float32;
            else if constexpr (std::is_same_v<X, std::double_t>)
                return AttributeType::float64;
            else if constexpr (requires(T t) { t.x; })
                return infer_gl_type<decltype(T::x)>();
            else
                throw std::invalid_argument{ "Can't infer OpenGL type, please specify explicitly" };
        }

        template <typename T>
        constexpr int infer_gl_size()
        {
            if constexpr (std::is_fundamental_v<T>)
                return 1;
            else if constexpr (requires(T t) { t.x; })
                return sizeof(T) / sizeof(T::x);
            else if constexpr (std::is_bounded_array_v<T>)
                return sizeof(T) / sizeof(*static_cast<T>(0));
            else
                throw std::invalid_argument{ "Can't infer type size, please specify explicitly" };
        }

        public:
        [[nodiscard]] Attributes() { glCreateVertexArrays(1, &vao); }
        ~Attributes() { glDeleteVertexArrays(1, &vao); }
        [[nodiscard]] Attributes(Attributes &&other) noexcept : vao{ std::exchange(other.vao, 0) } {}
        Attributes &operator=(Attributes &&other) noexcept
        {
            vao = std::exchange(other.vao, 0);
            return *this;
        }
        Attributes(const Attributes &) = delete;
        Attributes &operator=(const Attributes &) = delete;
        [[nodiscard]] GLuint operator*() const noexcept { return vao; }

        void activate() const { glBindVertexArray(vao); }

        template <typename T, typename U>
        void add(const Program &program, const Buffer &buffer, std::string_view name, U T:: *u, const AttributeConfig &cfg = {})
        {
            const T *t = nullptr;
            const auto offset = static_cast<GLsizei>(reinterpret_cast<std::size_t>(&(t->*u)));
            const auto location = glGetAttribLocation(*program, name.data());
            const auto atype = cfg.type == AttributeType::auto_calc ? infer_gl_type<U>() : cfg.type;
            const auto asize = cfg.count == 0 ? infer_gl_size<U>() : cfg.count;
            glVertexArrayVertexBuffer(vao, cfg.binding_index, *buffer, cfg.buffer_offset, sizeof(T));
            glEnableVertexArrayAttrib(vao, location);
            glVertexArrayAttribFormat(vao, location, asize, std::to_underlying(atype), cfg.normalize, offset);
            glVertexArrayAttribBinding(vao, location, cfg.binding_index);
            glVertexArrayBindingDivisor(vao, cfg.binding_index, cfg.divisor);
        }

        void set_element_buffer(const Buffer &buffer)
        {
            glVertexArrayElementBuffer(vao, *buffer);
        }
    };

    static_assert(!std::is_copy_assignable_v<Attributes>);
    static_assert(!std::is_copy_constructible_v<Attributes>);
    static_assert(std::is_move_assignable_v<Attributes>);
    static_assert(std::is_move_constructible_v<Attributes>);

} // End of namespace glube.