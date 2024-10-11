#pragma once

#include <stdexcept>
#include <type_traits>
#include <string_view>
#include <print>
#include <glad/gl.h>
#include "Buffer.hpp"
#include "Program.hpp"

namespace glube
{

    enum class AttributeType : GLenum
    {
        AutoCalc = 0,
        Byte = GL_BYTE,
        UnsignedByte = GL_UNSIGNED_BYTE,
        Short = GL_SHORT,
        UnsignedShort = GL_UNSIGNED_SHORT,
        Int = GL_INT,
        UnsignedInt = GL_UNSIGNED_INT,
        Float = GL_FLOAT,
        Double = GL_DOUBLE,

        // Can't auto-detect... yet...
        Fixed = GL_FIXED,
        HalfFloat = GL_HALF_FLOAT,
        Int_2_10_10_10 = GL_INT_2_10_10_10_REV,
        UnsignedInt_2_10_10_10 = GL_UNSIGNED_INT_2_10_10_10_REV,
        UnsignedInt_10F_11F_11F = GL_UNSIGNED_INT_10F_11F_11F_REV,
    };

    struct AttributeConfig
    {
        AttributeType type = AttributeType::AutoCalc;
        bool normalize = false;
        int count = 0;
        int bindingIndex = 0;
        int bufferOffset = 0;
        int divisor = 0;
    };

    class [[nodiscard]] Attributes final
    {
        GLuint vao{};

        template <typename T>
        constexpr AttributeType GLTypeCalc()
        {
            using X = std::remove_all_extents_t<std::remove_cvref_t<T>>;
            if constexpr (sizeof(X) == 1)
                if constexpr (std::is_signed_v<T>)
                    return AttributeType::Byte;
                else
                    return AttributeType::UnsignedByte;
            else if constexpr (std::is_same_v<X, std::int16_t>)
                return AttributeType::Short;
            else if constexpr (std::is_same_v<X, std::uint16_t>)
                return AttributeType::UnsignedShort;
            else if constexpr (std::is_same_v<X, std::int32_t>)
                return AttributeType::Int;
            else if constexpr (std::is_same_v<X, std::uint32_t>)
                return AttributeType::UnsignedInt;
            else if constexpr (std::is_same_v<X, std::float_t>)
                return AttributeType::Float;
            else if constexpr (std::is_same_v<X, std::double_t>)
                return AttributeType::Double;
            else if constexpr (requires(T t) { t.x; })
                return GLTypeCalc<decltype(T::x)>();
            else
                throw std::invalid_argument{"Can't infer OpenGL type, please specify explicitly"};
        }

        template <typename T>
        constexpr int GLSizeCalc()
        {
            if constexpr (std::is_fundamental_v<T>)
                return 1;
            else if constexpr (requires(T t) { t.x; })
                return sizeof(T) / sizeof(T::x);
            else if constexpr (std::is_bounded_array_v<T>)
                return sizeof(T) / sizeof(*static_cast<T>(0));
            else
                throw std::invalid_argument{"Can't infer type size, please specify explicitly"};
        }

    public:
        [[nodiscard]] Attributes() { glCreateVertexArrays(1, &vao); }
        ~Attributes() { glDeleteVertexArrays(1, &vao); }
        [[nodiscard]] Attributes(Attributes &&other) noexcept : vao{std::exchange(other.vao, 0)} {}
        Attributes &operator=(Attributes &&other) noexcept
        {
            vao = std::exchange(other.vao, 0);
            return *this;
        }
        Attributes(const Attributes &) = delete;
        Attributes &operator=(const Attributes &) = delete;
        [[nodiscard]] GLuint operator*() const { return vao; }

        void Activate() const { glBindVertexArray(vao); }

        template <typename T, typename U>
        void Add(const Program &program, const Buffer &buffer, std::string_view name, U T::*u, const AttributeConfig &cfg = {})
        {
            T *t = nullptr;
            const auto offset = static_cast<GLsizei>(reinterpret_cast<std::size_t>(&(t->*u)));
            const auto location = glGetAttribLocation(*program, name.data());
            const auto atype = cfg.type == AttributeType::AutoCalc ? GLTypeCalc<U>() : cfg.type;
            const auto asize = cfg.count == 0 ? GLSizeCalc<U>() : cfg.count;
            glVertexArrayVertexBuffer(vao, cfg.bindingIndex, *buffer, cfg.bufferOffset, sizeof(T));
            glEnableVertexArrayAttrib(vao, location);
            glVertexArrayAttribFormat(vao, location, asize, std::to_underlying(atype), cfg.normalize, offset);
            glVertexArrayAttribBinding(vao, location, 0);
            glVertexArrayBindingDivisor(vao, cfg.bindingIndex, cfg.divisor);
        }
    };

} // End of namespace glube.