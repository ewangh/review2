#include <iostream>
#include <string>
#include <optional>
#include <iomanip>
#include "business.h"

void run(const std::string &code, const std::string &day)
{
    std::string url = R"(https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?_var=kline_dayqfq&param=)" + code + R"(,day,,,)" + day + R"(,qfq)";
    auto response = dataRequest(url);

    if (response.empty())
    {
        std::cout << "ERROR REQUEST!!!" << std::endl;
        return;
    }

    try
    {
        std::vector<DateData> list = dataRespond(response);

        if (list.empty())
        {
            std::cout << "HAS EMPTY QUEUE!!!" << std::endl;
            return;
        }

        auto makeInfo = getMarkData(list);
        std::cout << makeInfo << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char *argv[])
{
    // for (int i = 0;i < argc;i ++)
    //     std::cout << argv[i] << std::endl;
    SetConsoleOutputCP(CP_UTF8);

    std::string day = "80";
    MarkDataPO::beginHour = 9;
    MarkDataPO::beginMin = 30;

    if (argc >= 2)
    {
        MarkDataPO::beginHour = std::stoi(argv[1]);
    }

    if (argc >= 3)
    {
        MarkDataPO::beginMin = std::stoi(argv[2]);
    }

    if (argc >= 4)
    {
        day = argv[3];
    }

    std::string input;

    while (1)
    {
        std::cout << "Code :";
        std::getline(std::cin, input);

        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());

        if (input.empty())
        {
            continue;
        }

        auto code = setPreCode(input);
        run(code, day);
    }

    return 0;
}
