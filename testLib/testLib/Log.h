#ifndef LOG_H    
#define LOG_H    

//log文件路径  
#define LOG_FILE_NAME "./testModbusLib.log"

#include <fstream>    
#include <string>    
#include <ctime>

using namespace std;

#define DEBUG(x)          Log::WriteDebug(x);
#define LOG(x)			  Log::WriteLog(x);
#define INFO(x)           Log::WriteInfo(x);
#define TRACE(x)          Log::WriteTrace(x);
#define WARNING(x)		  Log::WriteWarning(x);
#define ERROR(x)          Log::WriteError(x);

class Log
{
public:
    template <class T>
    static void WriteLog(T x)
    {
        ofstream fout(LOG_FILE_NAME, ios::app);
        fout.seekp(ios::end);
        fout << GetSystemTime() << " [LOG] " << x << endl;
        fout.close();
    }

    template <class T>
    static void WriteDebug(T x)
    {
        ofstream fout(LOG_FILE_NAME, ios::app);
        fout.seekp(ios::end);
        fout << GetSystemTime() << " [DEBUG] " << x << endl;
        fout.close();
    }

    template <class T>
    static void WriteInfo(T x)
    {
        ofstream fout(LOG_FILE_NAME, ios::app);
        fout.seekp(ios::end);
        fout << GetSystemTime() << " [INFO] " << x << endl;
        fout.close();
    }

    template <class T>
    static void WriteTrace(T x)
    {
        ofstream fout(LOG_FILE_NAME, ios::app);
        fout.seekp(ios::end);
        fout << GetSystemTime() << " [TRACE] " << x << endl;
        fout.close();
    }

    template <class T>
    static void WriteError(T x)
    {
        ofstream fout(LOG_FILE_NAME, ios::app);
        fout.seekp(ios::end);
        fout << GetSystemTime() << " [ERROR] " << x << endl;
        fout.close();
    }

    template <class T>
    static void WriteWarning(T x)
    {
        ofstream fout(LOG_FILE_NAME, ios::app);
        fout.seekp(ios::end);
        fout << GetSystemTime() << " [WARNING] " << x << endl;
        fout.close();
    }

private:
    static string GetSystemTime()
    {
        time_t tNowTime;
        time(&tNowTime);
        tm* tLocalTime = localtime(&tNowTime);
        char szTime[30] = { '\0' };
        strftime(szTime, 30, "[%Y-%m-%d %H:%M:%S] ", tLocalTime);
        string strTime = szTime;
        return strTime;
    }
};
#endif  
