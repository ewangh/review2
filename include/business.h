#pragma once
#include <iostream>
#include <string>
#include <optional>
#include <iomanip>
#include "httpRequest.h"

#define D_LASTS 19800 //9.30-15.00
#define D_MAXTS 14400 //9.30-11.30 13.00-15.00
#define D_PMTS 12600 //9.30-13.00
#define D_STOPTS 5400 //11.30-13.00
#define D_AMTS 7200 //9.30-11.30

struct MarkDataPO
{
    static int beginHour;
    static int beginMin;
};

struct MarkData
{
    double Opening;
    double Closed;
    double Highest;
    double Lower;
    double Price;
    long Volume;
    std::tm Date;

    std::optional<double> OOpening;
    std::optional<double> OClosed;
    std::optional<double> OHighest;
    std::optional<double> OLower;
    std::optional<double> OPrice;
    std::optional<long> OVolume;
    std::tm ODate;

    MarkData *BakMark = nullptr;

    bool isUp() const
    {
        return Price > Opening;
    }

    bool isUpLadder(const double price) const
    {
        return price > (Highest * 2 - Lower);
    }

    bool isDownLadder(const double price) const
    {
        return price < (Lower * 2 - Highest);
    }

    bool isDownBakLower(const double price) const
    {
        return price < BakMark->Lower;
    }

    bool empty() const
    {
        return Date.tm_mday == 0;
    }

    bool bakEmpty() const
    {
        return BakMark == nullptr;
    }

    void setOMark(const double oopening, const double ohighest, const double olower, const double oprice, const long ovolume, const double oclosed, const std::tm odate)
    {
        OOpening = oopening;
        OHighest = ohighest;
        OLower = olower;
        OPrice = oprice;
        OVolume = ovolume;
        OClosed = oclosed;
        ODate = odate;
    }

    void setMark(const double opening, const double highest, const double lower, const double price, const long volume, const double closed, const std::tm date)
    {
        Opening = opening;
        Highest = highest;
        Lower = lower;
        Price = price;
        Volume = volume;
        Closed = closed;
        Date = date;
    }

    void setBakMark(const double opening, const double highest, const double lower, const double price, const long volume, const double closed, const std::tm date)
    {
        BakMark = new MarkData();
        BakMark->setMark(opening, highest, lower, price, volume, closed, date);
    }

    void clear()
    {
        Date = {};
    }

    void bakClear()
    {
        if (BakMark != nullptr)
        {
            delete BakMark;
            BakMark = nullptr;
        }
    }

    ~MarkData()
    {
        bakClear();
    }

    friend std::ostream &operator<<(std::ostream &os, const MarkData &data)
    {
        if (data.empty())
        {
            std::cout << "NO MARK!!!" << std::endl;
        }

        std::string ladder = (data.OPrice.has_value() && data.OLower.has_value()) ? (!data.empty() && !data.bakEmpty() && !data.isDownBakLower(*data.OPrice))
                                                                                        ? "BACK"
                                                                                    : data.empty()
                                                                                        ? "EMPTY"
                                                                                    : data.isUpLadder(*data.OPrice)
                                                                                        ? "UP"
                                                                                    : (!data.isDownLadder(*data.OLower))
                                                                                        ? "DOWN"
                                                                                        : "UNKOWN"
                                                                                  : "WRONG";

        os << "[" << ladder << "]";

        if (!data.empty())
        {
            os << " UL: " << data.Highest * 2 - data.Lower
               << ", DL: " << data.Lower * 2 - data.Highest
               << ", " << std::put_time(&data.Date, "%Y/%m/%d");
        }

        os << " | ";

        if (!data.bakEmpty())
        {
            os << " ML: " << data.BakMark->Lower
               << ", " << std::put_time(&data.BakMark->Date, "%Y/%m/%d");
        }

        os << " | ";

        os << " P: " << *data.OPrice;

        // 获取当前时间
        std::time_t currentTime = std::time(nullptr);

        //将指定时间转换为std::time_t类型
        std::tm odate = data.ODate;
        odate.tm_hour=MarkDataPO::beginHour;
        odate.tm_min=MarkDataPO::beginMin;
        std::time_t specified_time = std::mktime(&odate);

        //计算两个时间之间的差值
        double diff_seconds = std::difftime(currentTime, specified_time);

        if (diff_seconds > D_LASTS)
        {
            diff_seconds = D_MAXTS;
        }
        else if (diff_seconds > D_PMTS)
        {
            diff_seconds -= D_STOPTS;
        }
        else if (diff_seconds < D_PMTS && diff_seconds > D_AMTS)
        {
            diff_seconds = D_AMTS;
        }

        double ave = 0.0;

        if (diff_seconds != 0 && data.OHighest.has_value() && data.OLower.has_value() && data.OVolume.has_value())
        {
            ave = (*data.OHighest + *data.OLower) **data.OVolume / (200 * diff_seconds);
        }

        os << ", AVE：" << std::fixed << std::setprecision(2) << ave << " w/s";

        os << ", " << std::put_time(&data.ODate, "%Y/%m/%d");

        // if (data.Closed != 0)
        //     os << std::fixed << std::setprecision(2) << (data.Price / data.Closed - 1) * 100 << "%";

        return os;
    }
};

MarkData getMarkData(std::vector<DateData>);

std::string setPreCode(const std::string &);