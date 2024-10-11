#pragma once

#include <memory>
#include <limits>
#include <print>
#include <utility>
#include <vector>

namespace glube
{
    enum class PoolReturnType
    {
        New,
        Used,
        Error,
    };

    template <typename T>
    struct Pool
    {
        class Holder
        {
            friend Pool;
            static constexpr std::size_t bad = std::numeric_limits<std::size_t>::max();

            Pool *pool{};
            std::size_t index{bad};

            Holder(Pool *pool, std::size_t index) : pool{pool}, index{index} {}

        public:
            Holder() = default;
            Holder(Holder &&other) noexcept : pool{std::exchange(other.pool, nullptr)}, index{std::exchange(other.index, bad)} {}
            Holder &operator=(Holder &&other) noexcept
            {
                pool = std::exchange(other.pool, nullptr);
                if (index != bad && pool)
                {
                    pool->free.push_back(index);
                }
                index = std::exchange(other.index, bad);
                return *this;
            }
            ~Holder()
            {
                if (index != bad && pool)
                {
                    pool->free.push_back(index);
                }
            }
            T *operator->() const { return &pool->data[index]; }
            T &operator*() const { return pool->data[index]; }
            operator bool() const noexcept { return index != bad; }
        };

        template <typename... Args>
        [[nodiscard]] std::pair<Holder, PoolReturnType> Get(Args &&...args)
        {
            if (!free.empty())
            {
                Holder holder{this, free.back()};
                free.pop_back();
                return {std::move(holder), PoolReturnType::Used};
            }
            data.emplace_back(args...);
            return {Holder{this, data.size() - 1}, PoolReturnType::New};
        }

    private:
        std::vector<T> data;
        std::vector<std::size_t> free;
    };

} // End of namespace glube.