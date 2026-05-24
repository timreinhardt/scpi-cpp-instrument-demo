#pragma once

#include <string>
#include <vector>

struct TraceSummary {
    std::size_t pointCount = 0;
    double minValue = 0.0;
    double maxValue = 0.0;
    double averageValue = 0.0;
};

std::vector<double> parseTraceCsv(const std::string& csv);
TraceSummary summarizeTraceData(const std::vector<double>& values);
std::string formatTraceSummary(const TraceSummary& summary,
                               const std::vector<double>& values);