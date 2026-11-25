# SPMultiThreadedTester
StreetPerfect client test/example c++ multithreaded console app

(currently LPC only and requires v12.5.2 client library build min)

- SPMultiThreadedTester.cpp main app logic
- SPClient.cpp light c++ wrapper around SP client imports (currently very minimal)
- CStringOut.cpp helper class for SPClient
- CsvFileQueue.cpp parses a (simple) CSV file line by line for multithreaded access
- SimpleLogger.cpp console logger for multithreaded access

* To build this example add your SP SharedFiles folder to the header and library includes path

