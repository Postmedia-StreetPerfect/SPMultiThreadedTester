#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "CsvFileQueue.h"


bool CsvFileQueue::Open(const char* csv_file)
{
    std::lock_guard<std::mutex> lock(_mtx);

    if (_file.is_open())
    {
        _file.close();
    }
    _row_num = 0;
    _file.open(csv_file, std::ios::in);
    if (!_file.is_open()) {
        std::error_code ec(errno, std::generic_category());

        std::cerr << "Open failed!\n"
            << "Error code: " << ec.value() << '\n'
            << "Message: " << ec.message() << '\n';
        return false;
    }
    return true;
}

void CsvFileQueue::Close()
{
    std::lock_guard<std::mutex> lock(_mtx);
    if (_file.is_open())
    {
        _file.close();
    }
}

// we DON'T check for double quotes
std::vector<std::string> CsvFileQueue::GetRow()
{
    std::lock_guard<std::mutex> lock(_mtx);
    char delimiter = ',';

    std::vector<std::string> row;
    std::string line;
    if (std::getline(_file, line)) {
        size_t start = 0;
        size_t end = line.find(delimiter);
        row.push_back(std::to_string(_row_num++)); // add row num as first column
 
        while (end != std::string::npos) {
            row.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find(delimiter, start);
        }
        // Always add the final column (even if empty after a trailing comma)
        row.push_back(line.substr(start));
    }

    return row;
}
