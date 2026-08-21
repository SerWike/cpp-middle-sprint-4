#include "metric_impl/code_lines_count.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <iterator>
#include <numeric>

namespace analyzer::metric::metric_impl {

struct TestData {
    std::string filename;
    int expected_lines;
};

class CodeLinesCountTest : public ::testing::TestWithParam<TestData> {
protected:
    void SetUp() override {
        const auto &param = GetParam();
        analyzer::file::File test_file(test_files_path + param.filename);

        functions = extractor.Get(test_file);
    }

    void TearDown() override { functions.clear(); }

    std::vector<analyzer::function::Function> functions;
    CodeLinesCountMetric metric;
    function::FunctionExtractor extractor;
    const std::string test_files_path = "/workspaces/cpp-middle-sprint-4/src/metric_impl/tests/files/";
};

TEST_P(CodeLinesCountTest, Validtest) {
    const auto &param = GetParam();

    std::vector<MetricResult> results;
    results.reserve(functions.size());

    std::ranges::transform(functions, std::back_inserter(results),
                           [this](auto &func) { return metric.Calculate(func); });

    ASSERT_EQ(results[0].metric_name, "Code lines count");

    ASSERT_EQ(std::get<int>(results[0].value), param.expected_lines) << "Total lines mismatch for " << param.filename;
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(Files, CodeLinesCountTest,
                         ::testing::Values( TestData{"comments.py",         3},
                                            TestData{"exceptions.py",       7},
                                            TestData{"if.py",               3},
                                            TestData{"loops.py",            6},
                                            TestData{"many_lines.py",       11},
                                            TestData{"many_parameters.py",  1},
                                            TestData{"match_case.py",       7},
                                            TestData{"nested_if.py",        8},
                                            TestData{"simple.py",           5},
                                            TestData{"ternary.py",          1}));
// clang-format on
}  // namespace analyzer::metric::metric_impl
