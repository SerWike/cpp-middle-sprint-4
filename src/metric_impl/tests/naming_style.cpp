#include "metric_impl/naming_style.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <iterator>
#include <numeric>

namespace analyzer::metric::metric_impl {

struct NamingStyleTestData {
    std::string filename;
    std::string expected_naming_style;
};

class NamingStyleTest : public ::testing::TestWithParam<NamingStyleTestData> {
protected:
    void SetUp() override {
        const auto &param = GetParam();
        analyzer::file::File test_file(test_files_path + param.filename);

        functions = extractor.Get(test_file);
    }

    void TearDown() override { functions.clear(); }

    std::vector<analyzer::function::Function> functions;
    NamingStyleMetric metric;
    function::FunctionExtractor extractor;
    const std::string test_files_path = "/workspaces/cpp-middle-sprint-4/src/metric_impl/tests/files/";
};

TEST_P(NamingStyleTest, Validtest) {
    const auto &param = GetParam();

    std::vector<MetricResult> results;
    results.reserve(functions.size());

    std::ranges::transform(functions, std::back_inserter(results),
                           [this](auto &func) { return metric.Calculate(func); });

    ASSERT_EQ(results[0].metric_name, "Naming style");

    ASSERT_EQ(std::get<std::string>(results[0].value), param.expected_naming_style)
        << "Naming style mismatch for " << param.filename;
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(Files, NamingStyleTest,::testing::Values(
                            NamingStyleTestData{"comments.py",         "Unknown"},
                            NamingStyleTestData{"exceptions.py",       "Unknown"},
                            NamingStyleTestData{"if.py",               "Camel Case"},
                            NamingStyleTestData{"loops.py",            "Pascal Case"},
                            NamingStyleTestData{"many_lines.py",       "Lower Case"},
                            NamingStyleTestData{"many_parameters.py",  "Snake Case"},
                            NamingStyleTestData{"match_case.py",       "Unknown"},
                            NamingStyleTestData{"nested_if.py",        "Pascal Case"},
                            NamingStyleTestData{"simple.py",           "Snake Case"},
                            NamingStyleTestData{"ternary.py",          "Unknown"}));
// clang-format on

}  // namespace analyzer::metric::metric_impl
