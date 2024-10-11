#pragma once

#include <string_view>
#include <ctre.hpp>

#define nameof(x) glube::internal::_nameof(#x, sizeof(x))
#define nameofanything(x) glube::internal::_nameof(#x, 0)

namespace glube::internal
{
    consteval std::string_view _nameof(std::string_view x, std::size_t)
    {
        auto lastPointerDefreference = x.rfind("->");
        if (lastPointerDefreference != std::string_view::npos)
        {
            x = std::string_view(x.data() + lastPointerDefreference + 2);
        }
        if (auto m = x.find_last_of(".:"))
        {
            if (m != std::string_view::npos)
            {
                return x.data() + m + 1;
            }
        }
        return x;
    }

} // End namespace glube::internal.