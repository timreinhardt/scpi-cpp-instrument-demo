#include "FieldGrid.hpp"

#include <sstream>
#include <stdexcept>

FieldGrid parseFieldGrid(const std::string& text)
{
    FieldGrid grid;

    std::stringstream rowStream(text);
    std::string rowText;

    while (std::getline(rowStream, rowText, ';'))
    {
        if (rowText.empty())
        {
            continue;
        }

        std::vector<double> row;
        std::stringstream colStream(rowText);
        std::string valueText;

        while (std::getline(colStream, valueText, ','))
        {
            if (valueText.empty())
            {
                continue;
            }

            try
            {
                row.push_back(std::stod(valueText));
            }
            catch (const std::exception&)
            {
                throw std::runtime_error("Failed to parse field grid value: " + valueText);
            }
        }

        if (!row.empty())
        {
            grid.push_back(row);
        }
    }

    return grid;
}

std::size_t fieldGridRows(const FieldGrid& grid)
{
    return grid.size();
}

std::size_t fieldGridCols(const FieldGrid& grid)
{
    if (grid.empty())
    {
        return 0;
    }

    return grid.front().size();
}