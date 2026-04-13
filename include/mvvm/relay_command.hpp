/**
 * This file is part of libmvvm.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <meta>

namespace mvvm {

struct RelayCommand {
    std::meta::info CanExecuteMeta = std::meta::reflect_constant(false);

    consteval RelayCommand() {}
    consteval RelayCommand(std::meta::info CanExecuteMeta): CanExecuteMeta(CanExecuteMeta) {}
};

namespace impl {

template <std::meta::info ExecuteMeta, std::meta::info CanExecuteMeta = std::meta::reflect_constant(false)>
class RelayCommand {
    using Parent = typename [:std::meta::parent_of(ExecuteMeta):];
    using Execute = typename [:std::meta::type_of(ExecuteMeta):];
    using CanExecute = typename [:std::meta::type_of(CanExecuteMeta):];

    friend class ObservableObject<Parent>;

    Parent *object;

public:
    inline void execute() const noexcept(std::meta::is_noexcept(ExecuteMeta)) {
        object->[:ExecuteMeta:]();
    }

    inline bool can_execute() const
        noexcept(CanExecuteMeta == std::meta::reflect_constant(false) || std::meta::is_noexcept(CanExecuteMeta))
    {
        if constexpr (CanExecuteMeta == std::meta::reflect_constant(false)) {
            return true;
        } else {
            return object->[:CanExecuteMeta:]();
        }
    }

    inline void operator()() const noexcept(noexcept(execute()) && noexcept(can_execute())) {
        if (can_execute())
            execute();
    }
};

}

}
