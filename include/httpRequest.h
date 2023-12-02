#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <ctime>
#include <algorithm>
#include "utils.h"
#include "curl/curl.h"

struct DateData
{
    double Opening;
    double Highest;
    double Lower;
    double Price;
    double Closed;
    long Volume;
    std::tm Date;

    bool isUp() const
    {
        return Price > Opening;
    }

    bool empty() const
    {
        return Date.tm_mday==0;
    }

    bool isUpLadder(const double price) const
    {
        return price > Highest * 2 - Lower;
    }

    bool isDownLadder(const double price) const
    {
        return price < Lower * 2 - Highest;
    }
};

#pragma region 请求数据
std::string dataRequest(const std::string &);
#pragma endregion

#pragma region 处理数据
std::vector<DateData> dataRespond(const std::string &);
#pragma endregion
