#pragma once

#include <vector>
#include "glube/AutoBuffer.hpp"

namespace glube
{

    template<typename T>
    class [[nodiscard]] BuildingBuffer final
    {
        AutoBuffer m_buffer;
        std::vector<T> m_data;
        std::size_t m_size_set{};

        public:

        void reserve(const std::size_t capacity) { m_data.reserve(capacity); }

        void push_back(const T &value) { m_data.push_back(value); }
        void push_back(T &&value) { m_data.push_back(std::move(value)); }

        template<typename... Args>
        auto emplace_back(Args&&... args) { return m_data.emplace_back(std::forward<Args>(args)...); }

        void clear() { m_data.clear(); }

        auto size() const noexcept { return m_data.size(); }
        auto size_set() const noexcept { return m_size_set; }

        void set() { m_size_set = size(); m_buffer.set(size() * sizeof(T), m_data.data()); }
        void set_and_clear() { set(); clear(); }

        template<typename Self>
        auto &buffer(this Self &&self) noexcept { return self.m_buffer; }

    };

    // We should inherit all these from Buffer by encapsulating one.
    static_assert(!std::is_copy_assignable_v<AutoBuffer>);
    static_assert(!std::is_copy_constructible_v<AutoBuffer>);
    static_assert(std::is_move_assignable_v<AutoBuffer>);
    static_assert(std::is_move_constructible_v<AutoBuffer>);
} // End namesapce glube.