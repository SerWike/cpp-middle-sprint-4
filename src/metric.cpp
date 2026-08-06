#include "metric.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "function.hpp"

namespace analyzer::metric {
void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> metric) { metrics.push_back(std::move(metric)); }

/**
 * @brief Вычисляет все зарегистрированные метрики для заданной функции.
 *
 * Эта функция применяет каждый метрический объект из контейнера `metrics`
 * к переданной функции `func` и собирает результаты в вектор.
 */
MetricResults MetricExtractor::Get(const function::Function &func) const {
    MetricResults result;
    try {
        result.reserve(metrics.size());
    } catch (const std::exception &e) {
        throw std::runtime_error(std::format("Failed to reserve size for Metric Results: {}", e.what()));
    }

    std::ranges::transform(metrics, std::back_inserter(result),
                           [&func](auto &metric) { return metric->Calculate(func); });

    return result;
}

}  // namespace analyzer::metric
