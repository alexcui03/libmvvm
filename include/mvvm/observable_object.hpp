/**
 * This file is part of libmvvm.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <meta>
#include <utility>
#include <vector>

#include "observable_property.hpp"
#include "property_proxy.hpp"
#include "utils.hpp"

namespace mvvm {

template <class Object>
class ObservableObject: public Object {
    struct Impl; // Proxy object for getter and setter, implemented by std::meta::define_aggregate().

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    // Generate data members of Impl struct. All non-static data members in Object with annotation
    // ObservableObject will be wrapped by PropertyProxy.
    consteval {
        std::vector<std::meta::info> proxy_properties;
        auto context = std::meta::access_context::current();

        for (auto property : std::meta::nonstatic_data_members_of(^^Object, context)) {
            if (!utils::has_annotation<ObservableProperty>(property))
                continue;

            proxy_properties.push_back(
                std::meta::data_member_spec(
                    std::meta::substitute(
                        ^^PropertyProxy, {
                            std::meta::reflect_constant(property)
                        }
                    ),
                    {
                        .name = std::meta::identifier_of(property)
                    }
                )
            );
        }

        std::meta::define_aggregate(^^Impl, proxy_properties);
    }
#pragma GCC diagnostic pop

    Impl impl;

public:
    template <class ...Args>
    ObservableObject(Args &&...args): Object(std::forward<Args>(args)...) {
        // @todo should be replaced by member initialization list.
        template for (
            constexpr auto property :
            std::define_static_array((
                std::meta::nonstatic_data_members_of(^^Impl, std::meta::access_context::current())
            ))
        ) {
            impl.[:property:].object = static_cast<Object *>(this);
        }
    }

    inline Impl *operator->() { return &impl; }
    inline const Impl *operator->() const { return &impl; }
};

}
