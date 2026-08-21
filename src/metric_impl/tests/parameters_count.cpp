#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

struct TestData {
    std::string filename;
    int expected_parameters;
};

class CountParametersTest : public ::testing::TestWithParam<TestData> {
protected:
    void SetUp() override {
        const auto &param = GetParam();
        analyzer::file::File test_file(test_files_path + param.filename);

        functions = extractor.Get(test_file);
    }

    void TearDown() override { functions.clear(); }

    std::vector<analyzer::function::Function> functions;
    CountParametersMetric metric;
    function::FunctionExtractor extractor;
    const std::string test_files_path = "/workspaces/cpp-middle-sprint-4/src/metric_impl/tests/files/";
};

TEST_P(CountParametersTest, Validtest) {
    const auto &param = GetParam();

    std::vector<MetricResult> results;
    results.reserve(functions.size());

    std::ranges::transform(functions, std::back_inserter(results),
                           [this](auto &func) { return metric.Calculate(func); });

    ASSERT_EQ(results[0].metric_name, "Parameters count");

    ASSERT_EQ(std::get<int>(results[0].value), param.expected_parameters)
        << "Parameters count mismatch for " << param.filename;
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(Files, CountParametersTest,
                         ::testing::Values( TestData{"comments.py",         3},
                                            TestData{"exceptions.py",       0},
                                            TestData{"if.py",               1},
                                            TestData{"loops.py",            1},
                                            TestData{"many_lines.py",       0},
                                            TestData{"many_parameters.py",  5},
                                            TestData{"match_case.py",       1},
                                            TestData{"nested_if.py",        2},
                                            TestData{"simple.py",           0},
                                            TestData{"ternary.py",          1}));
// clang-format on

}  // namespace analyzer::metric::metric_impl
