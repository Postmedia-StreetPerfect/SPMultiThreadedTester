// SPMultiThreadedTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <chrono>
#include <thread>

#include "SPClient.h"
#include "SimpleLogger.h"
#include "CsvFileQueue.h"

int TestLpcClient(const int thread_num=10);
int LpcTask(int taskId);


void RunThreads(const int thread_num, int (*func)(int));
SimpleLogger logger;
CsvFileQueue CsvQueue;

int main(int argc, char* argv[])
{
    logger.printf(SimpleLogger::blue, "Bill's mutil-threaded StreetPerfect native client tester, Nov 24 2026");
    auto start = std::chrono::steady_clock::now();
    int ret = -1;
    
    ret = TestLpcClient();
    if (ret == 0) 
        ret = TestLpcClient(); // checks that previous Disconnect worked
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    logger.printf("Completed elapsed time: %ld ms", duration.count());

    return ret;

}

int TestLpcClient(const int thread_num)
{
    logger.printf("TestLpcClient() called");
    if (CsvQueue.Open("CP_CertsSampleData.txt")) {
        CsvQueue.GetRow(); // read the sample data file header
    }
    ConnectionResponse con_resp = StreetPerfectNativeClient::Connect("StreetPerfectAddressAccuracy.Ini");
    logger.printf("StreetPerfectConnect returned = %d, flag= %s, msg= %s", con_resp.ret, con_resp.status_flag.c_str(), con_resp.status_messages.c_str());

    RunThreads(thread_num, LpcTask);

    con_resp = StreetPerfectNativeClient::Disconnect();
    logger.printf("StreetPerfectDisconnect returned = %d, flag= %s, msg= %s", con_resp.ret, con_resp.status_flag.c_str(), con_resp.status_messages.c_str());

    return con_resp.ret;
}

void RunThreads(const int thread_num, int (*func)(int))
{
    std::thread *threads = new std::thread[thread_num];
    for (int task = 0; task < thread_num; task++) {
        threads[task] = std::thread(func, task);
    }
    for (int task = 0; task < thread_num; task++) {
        if (threads[task].joinable())
        {
            threads[task].join();
        }
    }
    delete[] threads;
}


int LpcTask(int taskId)
{
    logger.printf("LpcTask() called with taskId: %d", taskId);
	
    int rows_read = 0;
    std::vector<std::string> row;
    do {
        row = CsvQueue.GetRow();
        if (!row.empty())
        {
            caAddressRequest req;

            // sample data row== ROW_NUM,ADDRESSID,DESC,ADDRESS_1,CITY,PROVINCE,POSTAL_CODE
            if (row.size() > 6) {
                req.recipient = row[2];
                req.address_line = row[3];
                req.city = row[4];
                req.province = row[5];
                req.postal_code = row[6];

                req.options = "";

                caCorrectionResponse resp = StreetPerfectNativeClient::caCorrectAddress(req);

                SimpleLogger::color c = resp.ok() ? SimpleLogger::green : SimpleLogger::red;
                logger.printf(c, "thread: %d, row: %s, addr: %s, ret= %d, flag= %s, msg= %s"
                    , taskId, row[0].c_str(), req.address_line.c_str(), resp.ret, resp.status_flag.c_str(), resp.status_messages.c_str());
            }
            else if (row.size() > 0) {
                logger.printf("row: %s, missing columns (corrupted?)", row[0].c_str());
            }
            else {
                logger.printf("empty row! (can't happen?) ");
            }

            rows_read++;
        }
    } 
    while (!row.empty());

    logger.printf("LpcTask() completed for taskId: %d, %d rows read", taskId, rows_read);
	return 0;
}

