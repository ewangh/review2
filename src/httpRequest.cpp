#pragma once
#include "httpRequest.h"

// 回调函数，用于处理接收到的数据
size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
    //std::cout << "Current function: " << __FUNCTION__ << std::endl;
    size_t totalSize = size * nmemb;
    output->append((char *)contents, totalSize);
    return totalSize;
}

std::string dataRequest(const std::string &url)
{
    //std::cout << "Current function: " << __FUNCTION__ << std::endl;
    std::string response;

    CURL *curl = curl_easy_init();
    if (curl)
    {
        // 设置请求的URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // 设置回调函数，用于处理接收到的数据
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // 发送请求
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // 清理curl
        curl_easy_cleanup(curl);
    }

    return response;
}

DateData str2DateData(const std::string &strData)
{
    //std::cout << "Current function: " << __FUNCTION__ << std::endl;

    std::string input(strData);
    std::string arrayData[6];
    std::string temp;

    // 去掉字符串中的方括号
    input = input.substr(1, input.length() - 2);

    const char delimiter=',';
    // 分割字符串
    std::stringstream ss(input);
    std::string token;
    int i=0;
    while (std::getline(ss, token, delimiter)) {
        token = token.substr(1, token.length() - 2);
        arrayData[i++]=token;
    }

    DateData obj;
    obj.Opening = convertToDouble(arrayData[1]);
    obj.Highest = convertToDouble(arrayData[3]);
    obj.Lower = convertToDouble(arrayData[4]);
    obj.Price = convertToDouble(arrayData[2]);
    obj.Volume = convertToLong(arrayData[5]);
    obj.Date = convertToDate(arrayData[0]);

    return obj;
}

std::vector<DateData> dataRespond(const std::string &strData)
{
    //std::cout << "Current function: " << __FUNCTION__ << std::endl;

    std::vector<DateData> dataList;
    std::string regexPattern(R"(\["\d{4}(-|\\)\d{2}(-|\\)\d{2}","\d+\.?\d+","\d+\.?\d+","\d+\.?\d+","\d+\.?\d+","\d+\.?\d+"\])");
    std::regex pattern(regexPattern);
    std::string content(strData);

    std::regex_iterator<std::string::iterator> iter(content.begin(), content.end(), pattern);
    std::regex_iterator<std::string::iterator> end;

    while (iter != end)
    {
        auto obj = str2DateData(iter->str());
        dataList.push_back(obj);
        ++iter;
    }

    return dataList;
}
