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
#include "relay_command.hpp"
#include "utils.hpp"

namespace mvvm {

template <class Object>
class ObservableObject {
    struct Impl; // Proxy object for getter and setter, implemented by std::meta::define_aggregate().

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    // Generate data members of Impl struct.
    //
    // All non-static data members in Object with annotation ObservableObject will be wrapped by PropertyProxy.
    // Example:
    //      [[=ObservableProperty()]] int property;
    // will generate following member in Impl:
    //      PropertyProxy<...> property;
    //
    // All non-static function members in Object with annotation RelayCommand will create a RelayCommandImpl in Impl.
    // Example:
    //      [[=RelayCommand()]] void Command();
    // will generate following member in Impl:
    //      impl::RelayCommand<...> Command;
    consteval {
        std::vector<std::meta::info> properties;
        auto context = std::meta::access_context::current();

        for (auto member : std::meta::members_of(^^Object, context)) {
            // ObservableObject
            if (
                std::meta::is_nonstatic_data_member(member) &&
                utils::has_annotation<ObservableProperty>(member)
            ) {
                properties.push_back(
                    std::meta::data_member_spec(
                        std::meta::substitute(
                            ^^PropertyProxy, {
                                std::meta::reflect_constant(member)
                            }
                        ),
                        { .name = std::meta::identifier_of(member) }
                    )
                );
            }

            // RelayCommand
            if (
                auto annotations = std::meta::annotations_of_with_type(member, ^^RelayCommand);
                annotations.size() > 0 &&
                std::meta::is_function(member) &&
                !std::meta::is_special_member_function(member)
            ) {
                properties.push_back(
                    std::meta::data_member_spec(
                        std::meta::substitute(
                            ^^impl::RelayCommand, {
                                std::meta::reflect_constant(member),
                                std::meta::reflect_constant(std::meta::extract<RelayCommand>(annotations[0]).CanExecuteMeta)
                            }
                        ),
                        { .name = std::meta::identifier_of(member) }
                    )
                );
            }
        }

        std::meta::define_aggregate(^^Impl, properties);
    }
#pragma GCC diagnostic pop

    Object object;
    Impl impl;

public:
    template <class ...Args>
    ObservableObject(Args &&...args): object(std::forward<Args>(args)...) {
        // @todo should be replaced by member initialization list.
        template for (
            constexpr auto property :
            std::define_static_array((
                std::meta::nonstatic_data_members_of(^^Impl, std::meta::access_context::current())
            ))
        ) {
            impl.[:property:].object = &object;
        }
    }

    inline Impl *operator->() { return &impl; }
    inline const Impl *operator->() const { return &impl; }
};

}
