#pragma once

#include <string>
#include <vector>

using FieldGrid = std::vector<std::vector<double>>;

FieldGrid parseFieldGrid(const std::string& text);

std::size_t fieldGridRows(const FieldGrid& grid);
std::size_t fieldGridCols(const FieldGrid& grid);
