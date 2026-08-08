#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

TEST(SumAverageAccumulatorTest, ValidTest) {
    std::string metric_name = "Code lines count";
    std::vector<int> values = {11, 20, 30, 40, 50};
    SumAverageAccumulator accumulator;

    std::ranges::for_each(values, [&accumulator, &metric_name](int &val) {
        accumulator.Accumulate(metric::MetricResult{metric_name, val});
    });

    accumulator.Finalize();

    ASSERT_EQ(SumAverageAccumulator::SumAverage(151, 30.2), accumulator.Get());
}

TEST(SumAverageAccumulatorTest, EmptyTest) {
    std::string metric_name = "Code lines count";
    std::vector<int> values = {};
    SumAverageAccumulator accumulator;

    std::ranges::for_each(values, [&accumulator, &metric_name](int &val) {
        accumulator.Accumulate(metric::MetricResult{metric_name, val});
    });

    accumulator.Finalize();

    ASSERT_EQ(SumAverageAccumulator::SumAverage(0, 0.0), accumulator.Get());
}

TEST(SumAverageAccumulatorTest, InvalidGetTest) {
    std::string metric_name = "Code lines count";
    SumAverageAccumulator accumulator;

    accumulator.Accumulate(metric::MetricResult{metric_name, 100});
    ASSERT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(SumAverageAccumulatorTest, ResetTest) {
    std::string metric_name = "Code lines count";
    std::vector<int> values = {11, 20, 30, 40, 50};
    SumAverageAccumulator accumulator;

    std::ranges::for_each(values, [&accumulator, &metric_name](int &val) {
        accumulator.Accumulate(metric::MetricResult{metric_name, val});
    });

    accumulator.Reset();

    ASSERT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(SumAverageAccumulatorTest, ResetAfterFinalizeTest) {
    std::string metric_name = "Code lines count";
    SumAverageAccumulator accumulator;

    accumulator.Accumulate(metric::MetricResult{metric_name, 100});
    accumulator.Finalize();
    ASSERT_EQ(SumAverageAccumulator::SumAverage(100, 100.0), accumulator.Get());

    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{metric_name, 200});
    accumulator.Finalize();
    ASSERT_EQ(SumAverageAccumulator::SumAverage(200, 200.0), accumulator.Get());
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
