/**
 * This file is part of libmvvm.
 * SPDX-License-Identifier: MIT
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <mvvm/mvvm.hpp>

using namespace ::testing;

struct MockModel {
    int property = 0;
    int another = 10;
    int no_hook = 20;

    MOCK_METHOD(void, OnPropertyChanging, (int));
    MOCK_METHOD(void, OnPropertyChanged, (int));

    MOCK_METHOD(void, OnAnotherChanged, (int));
};

TEST(PropertyProxy, Simple) {
    MockModel model;
    mvvm::PropertyProxy<^^MockModel::property> proxy(&model);

    Expectation on_changing = EXPECT_CALL(model, OnPropertyChanging(10))
        .Description("OnProperyChanging(10) should be called once.")
        .Times(Exactly(1));

    EXPECT_CALL(model, OnPropertyChanged(10))
        .Description("OnPropertyChanged(10) should be called once after OnPropertyChanging.")
        .Times(Exactly(1))
        .After(on_changing);

    EXPECT_EQ(proxy, 0) << "Initial value should be 0.";
    proxy = 10;
    EXPECT_EQ(proxy, 10) << "Value should be 10 after assignment.";
}

TEST(PropertyProxy, NoChangingHook) {
    MockModel model;
    mvvm::PropertyProxy<^^MockModel::another> proxy(&model);

    EXPECT_CALL(model, OnAnotherChanged(0))
        .Times(Exactly(1));

    EXPECT_EQ(proxy, 10) << "Initial value should be 20.";
    proxy = 0;
    EXPECT_EQ(proxy, 0) << "Value should be 0 after assignment.";
}

TEST(PropertyProxy, NoHook) {
    MockModel model;
    mvvm::PropertyProxy<^^MockModel::no_hook> proxy(&model);

    EXPECT_EQ(proxy, 20) << "Initial value should be 20.";
    proxy = 10;
    EXPECT_EQ(proxy, 10) << "Value should be 10 after assignment.";
}
