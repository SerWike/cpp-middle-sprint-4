#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

struct TestData {
    std::string filename;
    int expected_complexity;
};

class CyclomaticComplexityTest : public ::testing::TestWithParam<TestData> {
protected:
    void SetUp() override {
        const auto &param = GetParam();
        analyzer::file::File test_file(test_files_path + param.filename);

        functions = extractor.Get(test_file);
    }

    void TearDown() override { functions.clear(); }

    std::vector<analyzer::function::Function> functions;
    CyclomaticComplexityMetric metric;
    function::FunctionExtractor extractor;
    const std::string test_files_path = "/workspaces/cpp-middle-sprint-4/src/metric_impl/tests/files/";
};

TEST_P(CyclomaticComplexityTest, Validtest) {
    const auto &param = GetParam();

    std::vector<MetricResult> results;
    results.reserve(functions.size());

    std::ranges::transform(functions, std::back_inserter(results),
                           [this](auto &func) { return metric.Calculate(func); });

    ASSERT_EQ(results[0].metric_name, "Cyclomatic Complexity");

    ASSERT_EQ(std::get<int>(results[0].value), param.expected_complexity)
        << "Cyclomatic complexity mismatch for " << param.filename;
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(Files, CyclomaticComplexityTest,
                         ::testing::Values( TestData{"comments.py",         1},
                                            TestData{"exceptions.py",       4},
                                            TestData{"if.py",               2},
                                            TestData{"loops.py",            4},
                                            TestData{"many_lines.py",       2},
                                            TestData{"many_parameters.py",  2},
                                            TestData{"match_case.py",       4},
                                            TestData{"nested_if.py",        4},
                                            TestData{"simple.py",           2},
                                            TestData{"ternary.py",          3}));
// clang-format on

}  // namespace analyzer::metric::metric_impl
