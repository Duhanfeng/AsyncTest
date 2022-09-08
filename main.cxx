#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

static std::string CurrentTime(const std::string& str)
{
    std::chrono::system_clock::time_point a = std::chrono::system_clock::now();     //时间点可以做差
    time_t t1 = std::chrono::system_clock::to_time_t(a);                            //time_t可以格式化

    struct tm _tm;
    if (localtime_s(&_tm, &t1) == 0)
    {
        std::stringstream ss;
        ss << std::put_time(&_tm, str.c_str());
        return ss.str();
    }

    return "";
}

//异步任务
void AysnTask(const std::string& message)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "call: " << message << std::endl;
    std::cout << "run: " << CurrentTime("%Y-%m-%d %H-%M-%S") << std::endl;
}

//在这个线程中触发异步任务
void CreateTask(const std::string& message)
{
    //AysnTask(message);
    std::future<void> future = std::async(launch::async, AysnTask, message);

}

int main(int argc, char* argv[])
{
    while (true)
    {
        //周期触发任务
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::string message = CurrentTime("%Y-%m-%d %H-%M-%S");
        std::cout << "call_start: " << message << std::endl;
        CreateTask(message);
        std::cout << "call_finish: " << CurrentTime("%Y-%m-%d %H-%M-%S") << std::endl << std::endl << std::endl << std::endl;
    }

    return 0;
}
