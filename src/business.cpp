#include <iostream>
#include <string>
#include <optional>
#include <iomanip>
#include "business.h"

int MarkDataPO::beginHour;
int MarkDataPO::beginMin;

MarkData getMarkData(std::vector<DateData> vec1)
{
    //std::cout << "Current function: " << __FUNCTION__ << std::endl;

    std::sort(vec1.begin(), vec1.end(), [](DateData data1, DateData data2)
              { return std::mktime(&data1.Date) < std::mktime(&data2.Date); });

    MarkData markInfo;
    DateData* prevInfo{nullptr};

    for (DateData &obj : vec1)
    {
        if (prevInfo->empty())
        {
            prevInfo = &obj;
            std::cout << std::put_time(&obj.Date, "%Y/%m/%d") << std::endl;
            continue;
        }

        obj.Closed = prevInfo->Price;

        if (markInfo.empty())
        {
            if (prevInfo->isDownLadder(obj.Lower))
            {
                markInfo.setBakMark(prevInfo->Opening, prevInfo->Highest, prevInfo->Lower, prevInfo->Price, prevInfo->Volume, prevInfo->Closed, prevInfo->Date);
                std::cout << "备份 " << std::put_time(&prevInfo->Date, "%Y/%m/%d") << std::endl;
            }
            else if (!prevInfo->isUpLadder(obj.Highest))
            {
                markInfo.setMark(prevInfo->Opening, prevInfo->Highest, prevInfo->Lower, prevInfo->Price, prevInfo->Volume, prevInfo->Closed, prevInfo->Date);
                std::cout << "新建 " << std::put_time(&markInfo.Date, "%Y/%m/%d") << std::endl;
            }
        }
        else if (std::mktime(&prevInfo->Date) > std::mktime(&markInfo.Date))
        {
            if (markInfo.isDownLadder(obj.Lower))
            {
                std::cout << "跌破 " << std::put_time(&obj.Date, "%Y/%m/%d") << std::endl;
                markInfo.setBakMark(markInfo.Opening, markInfo.Highest, markInfo.Lower, markInfo.Price, markInfo.Volume, markInfo.Closed, markInfo.Date);
                markInfo.clear();
            }
            else if (!markInfo.bakEmpty() && !markInfo.isDownBakLower(prevInfo->Highest) && markInfo.isUpLadder(prevInfo->Highest))
            {
                std::cout << "跌破新建 " << std::put_time(&prevInfo->Date, "%Y/%m/%d") << std::endl;
                markInfo.bakClear();
                markInfo.setMark(prevInfo->Opening, prevInfo->Highest, prevInfo->Lower, prevInfo->Price, prevInfo->Volume, prevInfo->Closed, prevInfo->Date);
            }
            else if (markInfo.isUpLadder(prevInfo->Highest))
            {
                markInfo.setMark(prevInfo->Opening, prevInfo->Highest, prevInfo->Lower, prevInfo->Price, prevInfo->Volume, prevInfo->Closed, prevInfo->Date);
                std::cout << "新平 " << std::put_time(&markInfo.Date, "%Y/%m/%d") << std::endl;
            }
            else
            {
                //std::cout << "忽略 " << std::put_time(&obj.Date, "%Y/%m/%d") << std::endl;
            }
        }
        
        markInfo.setOMark(obj.Opening, obj.Highest, obj.Lower, obj.Price, obj.Volume, obj.Closed, obj.Date);
        prevInfo = &obj;
    }
    
    return markInfo;
}

std::string setPreCode(const std::string &str)
{
    //std::cout << "Current function: " << __FUNCTION__ << std::endl;

    const std::string pre_sh = "sh";
    const std::string pre_sz = "sz";

    if (str.size() >= 8 && (str.substr(0, 2) == pre_sh || str.substr(0, 2) == pre_sz))
    {
        return str;
    }
    else if (str.size() >= 6 && (str.substr(0, 1) == "6"))
    {
        return pre_sh + str;
    }
    else
    {
        return pre_sz + str;
    }
}
