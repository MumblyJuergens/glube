#pragma once

#include "glube/Buffer.hpp"

namespace glube
{

    class [[nodiscard]] AutoBuffer final
    {
        Buffer m_buffer;
        std::size_t m_size{};
        BufferDataUsage m_usage{ BufferDataUsage::none };

        public:

        void set(const std::size_t size, const void *const data = nullptr, const BufferDataUsage usage = BufferDataUsage::dynamic_draw)
        {
            if (size > m_size || usage != m_usage)
            {
                m_size = size;
                m_usage = usage;
                m_buffer.init(size, data, usage);
            }
            else
            {
                m_buffer.overwrite(size, data);
            }
        }

        template<typename Self>
        auto &buffer(this Self &&self) noexcept { return self.m_buffer; }

    };

    // We should inherit all these from Buffer by encapsulating one.
    static_assert(!std::is_copy_assignable_v<AutoBuffer>);
    static_assert(!std::is_copy_constructible_v<AutoBuffer>);
    static_assert(std::is_move_assignable_v<AutoBuffer>);
    static_assert(std::is_move_constructible_v<AutoBuffer>);
} // End namesapce glube.