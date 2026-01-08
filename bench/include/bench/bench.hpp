#pragma once

#include <string_view>

namespace bench {

    struct Result {
    };

    template<typename Fn>
    Result run(const std::string& name, size_t iterations, Fn&& fn);

}