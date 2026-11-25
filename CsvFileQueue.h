#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <mutex>

class CsvFileQueue
{
public:

    CsvFileQueue() { _row_num = 0; }
    ~CsvFileQueue() 
    {
        Close();
    }
    bool Open(const char* csv_file);
    void Close();
    std::vector<std::string> GetRow();

private:
    std::ifstream _file;
    std::mutex _mtx;
    int _row_num;
};

