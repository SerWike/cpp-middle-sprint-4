

#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>
#include <unordered_map>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

TEST(CategoricalAccumulatorTest, ValidTest) {
    std::string metric_name = "Naming style";
    std::vector<std::string> values = {"Snake Case", "Unknown", "Snake Case", "Camel Case", "Unknown"};
    CategoricalAccumulator accumulator;

    std::ranges::for_each(values, [&accumulator, &metric_name](std::string &val) {
        accumulator.Accumulate(metric::MetricResult{metric_name, val});
    });

    accumulator.Finalize();

    std::unordered_map<std::string, int> expected_result = {{"Snake Case", 2}, {"Unknown", 2}, {"Camel Case", 1}};

    ASSERT_EQ(expected_result, accumulator.Get());
}

TEST(CategoricalAccumulatorTest, EmptyTest) {
    std::string metric_name = "Naming style";
    std::vector<std::string> values = {};
    CategoricalAccumulator accumulator;

    std::ranges::for_each(values, [&accumulator, &metric_name](std::string &val) {
        accumulator.Accumulate(metric::MetricResult{metric_name, val});
    });

    accumulator.Finalize();

    std::unordered_map<std::string, int> expected_result;
    ASSERT_EQ(expected_result, accumulator.Get());
}

TEST(CategoricalAccumulatorTest, InvalidGetTest) {
    std::string metric_name = "Naming style";
    CategoricalAccumulator accumulator;

    accumulator.Accumulate(metric::MetricResult{metric_name, "Unknown"});
    ASSERT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(CategoricalAccumulatorTest, ResetTest) {
    std::string metric_name = "Naming style";
    std::vector<std::string> values = {"Snake Case", "Unknown", "Snake Case", "Camel Case", "Unknown"};
    CategoricalAccumulator accumulator;

    std::ranges::for_each(values, [&accumulator, &metric_name](std::string &val) {
        accumulator.Accumulate(metric::MetricResult{metric_name, val});
    });

    accumulator.Reset();

    ASSERT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(CategoricalAccumulatorTest, ResetAfterFinalizeTest) {
    std::string metric_name = "Naming style";
    CategoricalAccumulator accumulator;

    accumulator.Accumulate(metric::MetricResult{metric_name, "Unknown"});
    accumulator.Finalize();
    std::unordered_map<std::string, int> expected_result = {{"Unknown", 1}};
    ASSERT_EQ(expected_result, accumulator.Get());

    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{metric_name, "Camel Case"});
    accumulator.Finalize();
    expected_result = {{"Camel Case", 1}};
    ASSERT_EQ(expected_result, accumulator.Get());
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
