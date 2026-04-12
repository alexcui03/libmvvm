/**
 * This file is part of libmvvm.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <meta>

namespace mvvm::utils {

/**
 * Check whether there contains a annotation with given type.
 */
template <class Annotation>
consteval bool has_annotation(std::meta::info info) {
    for (auto annotation : std::meta::annotations_of(info)) {
        if (
            std::meta::type_of(annotation) == ^^Annotation ||
            std::meta::type_of(annotation) == ^^const Annotation    // G++ will treat annotations as const type
        ) {
            return true;
        }
    }
    return false;
}

}
