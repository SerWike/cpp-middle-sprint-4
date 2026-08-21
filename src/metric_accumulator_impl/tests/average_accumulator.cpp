#include "metric_accumulator_impl/average_accumulator.hpp"
#include "metric.hpp"

#include <algorithm>
#include <gtest/gtest.h>

#include <cmath>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

TEST(AverageAccumulatorTest, ValidTest) {
    std::string metric_name = "Code lines count";
    std::vector<int> values = {11, 20, 30, 40, 50};
    AverageAccumulator accumulator;

    std::ranges::for_each(values, [&accumulator, &metric_name](int &val) {
        accumulator.Accumulate(metric::MetricResult{metric_name, val});
    });

    accumulator.Finalize();

    ASSERT_DOUBLE_EQ(30.2, accumulator.Get());
}

TEST(AverageAccumulatorTest, EmptyTest) {
    std::string metric_name = "Code lines count";
    std::vector<int> values = {};
    AverageAccumulator accumulator;

    std::ranges::for_each(values, [&accumulator, &metric_name](int &val) {
        accumulator.Accumulate(metric::MetricResult{metric_name, val});
    });

    accumulator.Finalize();

    ASSERT_DOUBLE_EQ(0.0, accumulator.Get());
}

TEST(AverageAccumulatorTest, InvalidGetTest) {
    std::string metric_name = "Code lines count";
    AverageAccumulator accumulator;

    accumulator.Accumulate(metric::MetricResult{metric_name, 100});
    ASSERT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(AverageAccumulatorTest, ResetTest) {
    std::string metric_name = "Code lines count";
    std::vector<int> values = {11, 20, 30, 40, 50};
    AverageAccumulator accumulator;

    std::ranges::for_each(values, [&accumulator, &metric_name](int &val) {
        accumulator.Accumulate(metric::MetricResult{metric_name, val});
    });

    accumulator.Reset();

    ASSERT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(AverageAccumulatorTest, ResetAfterFinalizeTest) {
    std::string metric_name = "Code lines count";
    AverageAccumulator accumulator;

    accumulator.Accumulate(metric::MetricResult{metric_name, 100});
    accumulator.Finalize();
    ASSERT_DOUBLE_EQ(100.0, accumulator.Get());

    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{metric_name, 200});
    accumulator.Finalize();
    ASSERT_DOUBLE_EQ(200.0, accumulator.Get());
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
