/**
 * This file is part of libmvvm example.
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include <mvvm/mvvm.hpp>

class ViewModel {
    friend class mvvm::ObservableObject<ViewModel>;

    [[=mvvm::ObservableProperty()]]
    int property = -1;

    [[=mvvm::ObservableProperty()]]
    int another_property = -1;

public:
    [[=mvvm::RelayCommand()]]
    void TestCommand() {
        std::cout << "Test " << property << " " << another_property << std::endl;
    }

    bool CanExecuteAnotherCommand() {
        return another_property == 42;
    }

    [[=mvvm::RelayCommand(^^CanExecuteAnotherCommand)]]
    void AnotherCommand() {
        std::cout << "Another " << property << " " << another_property << std::endl;
    }

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
    view_model->another_property = 20;
    view_model->TestCommand();
    view_model->AnotherCommand();
    view_model->another_property = 42;
    view_model->AnotherCommand();
}
