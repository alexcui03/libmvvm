/**
 * This file is part of libmvvm.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <meta>

namespace mvvm {

template <class Object> class ObservableObject;

template <std::meta::info PropertyMeta>
class PropertyProxy {
    using T = typename [:std::meta::type_of(PropertyMeta):];
    using Object = typename [:std::meta::parent_of(PropertyMeta):];

    friend class ObservableObject<Object>;

    /// Pointer to the object that contains the property.
    Object *object; // @todo add const to object

public:
    PropertyProxy(Object *object = nullptr): object(object) {}

    inline T &operator=(const T &value) noexcept {
        object->OnPropertyChanging(value);
        object->[:PropertyMeta:] = value;
        object->OnPropertyChanged(value);
        return object->[:PropertyMeta:];
    }

    inline operator T&() noexcept { return object->[:PropertyMeta:]; }
    inline operator T() const noexcept { return object->[:PropertyMeta:]; }
};

}
