# libmvvm

Model-View-ViewModel (MVVM) support library for modern C++.

This library is header-only, and it is recommended to use CMake.

## Requirements

- CMake 3.30 or newer
- g++ 16 (Not released now. You can acquire it from prebuilt package or build from source code by yourself.)

FTM (Feature-Test Macros) Requirements:
- `__cpp_impl_reflection`
- `__cpp_lib_define_static`

## Usage

Build examples with CMake:
```
mkdir build && cd build
cmake ..
cmake --build .
```

## Example

```cpp
#include <iostream>

#include <mvvm/mvvm.hpp>

class ViewModel {
    friend class mvvm::ObservableObject<ViewModel>;

    [[=mvvm::ObservableProperty()]]
    int property = -1;

    [[=mvvm::ObservableProperty()]]
    int another_property = -1;

public:
    void OnPropertyChanging(int value) {
        std::cout << "onChanging " << value << " " << property << std::endl;
    }

    void OnPropertyChanged(int value) {
        std::cout << "onChanged " << value << " " << property << std::endl;
    }

    void OnAnotherPropertyChanged(int value) {
        std::cout << "onChanged another " << value << " " << another_property << std::endl;
    }
};

int main() {
    mvvm::ObservableObject<ViewModel> view_model;

    view_model->property = 10;
    // onChanging 10 -1
    // onChanged 10 10

    view_model->another_property = 20;
    // onChanged another 20 20
}
```

## Reference

- [P2996R13](https://wg21.link/P2996R13) Reflection for C++26
- [P3394R4](https://wg21.link/P3394R4) Annotations for Reflection
- [P3491R3](https://wg21.link/P3491R3) define_static_{string,object,array}
- [P3391R2](https://wg21.link/P3391R2) constexpr std::format
