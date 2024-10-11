#include <utility>
#include <catch2/catch_test_macros.hpp>
#include <glube/Pool.hpp>

struct [[nodiscard]] MoveOnly
{
    int moved{};

    [[nodiscard]] MoveOnly() = default;
    [[nodiscard]] MoveOnly(MoveOnly &&o) noexcept : moved{std::exchange(o.moved, 0) + 1} {}
    MoveOnly &operator=(MoveOnly &&o) noexcept
    {
        moved = std::exchange(o.moved, 0) + 1;
        return *this;
    }
    MoveOnly(const MoveOnly &) = delete;
    MoveOnly &operator=(const MoveOnly &) = delete;
};

TEST_CASE("Pool return value optimisation", "[pool]")
{
    glube::Pool<MoveOnly> pool;

    auto [mo, status] = pool.Get();

    REQUIRE(status == glube::PoolReturnType::New);
    REQUIRE(mo->moved == 0);
}

TEST_CASE("Pool moves", "[pool]")
{
    glube::Pool<MoveOnly> pool;
    glube::PoolReturnType originalReturnType = glube::PoolReturnType::Error;
    {
        auto [mo, status] = pool.Get();
        originalReturnType = status;

        // Returned to pool by dtor here...
    }
    auto [mo, status] = pool.Get();

    REQUIRE(originalReturnType == glube::PoolReturnType::New);
    REQUIRE(status == glube::PoolReturnType::Used);
    REQUIRE(mo->moved == 0);
}
