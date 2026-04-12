/**
 * This file is part of libmvvm.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <meta>
#include <ranges>
#include <string_view>

#include "utils.hpp"

namespace mvvm {

template <class Object> class ObservableObject;

template <std::meta::info PropertyMeta>
class PropertyProxy {
    using T = typename [:std::meta::type_of(PropertyMeta):];
    using Object = typename [:std::meta::parent_of(PropertyMeta):];

    friend class ObservableObject<Object>;

    /// Pointer to the object that contains the property.
    Object *object; // @todo add const to object

    template <const char *Name, typename ...Args>
    inline constexpr void call_method(Args &&...args) noexcept {
        using std::operator""sv;

        constexpr auto method = utils::get_member_function(^^Object, Name);

        if constexpr (method != ^^Object) {
            object->[:method:](std::forward<Args>(args)...);
        }
    }

public:
    PropertyProxy(Object *object = nullptr): object(object) {}

    inline T &operator=(const T &value) noexcept {
        using std::operator""sv;

        // @todo use constexpr std::format (P3391R2) when it is avaliable.
        call_method<
            std::define_static_string(std::views::concat(
                "On"sv,
                std::string_view(utils::snake_to_pascal_case(std::meta::identifier_of(PropertyMeta))),
                "Changing"sv
            ))
        >(value);

        object->[:PropertyMeta:] = value;

        call_method<
            std::define_static_string(std::views::concat(
                "On"sv,
                std::string_view(utils::snake_to_pascal_case(std::meta::identifier_of(PropertyMeta))),
                "Changed"sv
            ))
        >(value);

        return object->[:PropertyMeta:];
    }

    inline operator T&() noexcept { return object->[:PropertyMeta:]; }
    inline operator T() const noexcept { return object->[:PropertyMeta:]; }
};

}
