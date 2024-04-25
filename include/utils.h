#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <ctime>
#include <iomanip>

// 将字符串转换为日期
std::tm convertToDate(const std::string&);

// 将字符串转换为双精度浮点数
double convertToDouble(const std::string&);
// 将字符串转换为长整型
long convertToLong(const std::string&);