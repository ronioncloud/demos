#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "PlrChrGen.h"

//省份简称
const char chr_zhs_1[31][4] = {"京", "沪", "津", "渝", "冀", "晋", "蒙", "辽", "吉", "黑", "苏", "浙", "皖", "闽", "赣", "鲁", 
                              "豫", "鄂", "湘", "粤", "桂", "琼", "川", "贵", "云", "藏", "陕", "甘", "青", "宁", "新"};
//省份简称+教练车
const char chr_zhs_2[32][4] = {"京", "沪", "津", "渝", "冀", "晋", "蒙", "辽", "吉", "黑", "苏", "浙", "皖", "闽", "赣", "鲁", 
                              "豫", "鄂", "湘", "粤", "桂", "琼", "川", "贵", "云", "藏", "陕", "甘", "青", "宁", "新", "学"};
//军警
const char chr_zhs_3[12][4] = {"警", "军", "海", "空", "南", "济", "沈", "京", "广", "兰", "成", "北"};
//使领馆
const char chr_zhs_4[2][4] = {"使", "领"};
//纯数字
const char chr_eng_1[10][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
//英文+数字
const char chr_eng_2[34][2] = {};
