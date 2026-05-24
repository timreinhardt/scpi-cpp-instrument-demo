#include "TraceData.hpp"

#include <sstream>
#include <limits>
#include <stdexcept>

std::vector<double> parseTraceCsv(const std::string& csv)
{
    std::vector<double> values;
    std::stringstream ss(csv);
    std::string token;

    while (std::getline(ss, token, ','))
    {
        if (token.empty())
        {
            continue;
        }

        try
        {
            values.push_back(std::stod(token));
        }
        catch (const std::exception&)
        {
            throw std::runtime_error("Failed to parse trace value: " + token);
        }
    }

    return values;
}

TraceSummary summarizeTraceData(const std::vector<double>& values)
{
    TraceSummary summary;
    summary.pointCount = values.size();

    if (values.empty())
    {
        return summary;
    }

    summary.minValue = std::numeric_limits<double>::max();
    summary.maxValue = std::numeric_limits<double>::lowest();

    double sum = 0.0;

    for (double value : values)
    {
        if (value < summary.minValue)
        {
            summary.minValue = value;
        }

        if (value > summary.maxValue)
        {
            summary.maxValue = value;
        }

        sum += value;
    }

    summary.averageValue = sum / static_cast<double>(values.size());

    return summary;
}

std::string formatTraceSummary(const TraceSummary& summary,
                               const std::vector<double>& values)
{
    std::ostringstream out;

    out << "Trace Summary\n";
    out << "Points: " << summary.pointCount << "\n";
    out << "Min: " << summary.minValue << " dBm\n";
    out << "Max: " << summary.maxValue << " dBm\n";
    out << "Avg: " << summary.averageValue << " dBm\n";

    if (!values.empty())
    {
        out << "\nSample Values\n";
        out << "[1] = " << values.front() << " dBm\n";

        if (values.size() >= 2)
        {
            out << "[2] = " << values[1] << " dBm\n";
        }

        if (values.size() > 2)
        {
            out << "...\n";
            out << "[" << values.size() << "] = "
                << values.back() << " dBm";
        }
    }

    return out.str();
}