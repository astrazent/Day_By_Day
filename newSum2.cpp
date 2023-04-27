#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;

const int day_landmark = 1;
const int month_landmark = 1;
const int year_landmark = 2023;
const int rank_landmark = 8;
// kiểm tra ngày hợp lệ
int day_check(int day, int month)
{
    if (month == 2)
    {
        if (day >= 1 && day <= 29)
            return 1;
    }
    else
    {
        if (day >= 1 && day <= 31)
            return 1;
    }
    return 0;
}

// kiểm tra tháng hợp lệ
int month_check(int month)
{
    if (month >= 1 && month <= 12)
        return 1;
    return 0;
}

// kiểm tra năm hợp lệ
int year_check(int year)
{
    int check_2 = year;
    if (year >= 1000 && year <= 2200)
        return 1;
    return 0;
}

// kiểm tra năm nhuận
int leapYear_check(int year)
{
    if (year % 400 == 0 || year % 4 == 0 && year % 100 != 0)
        return 1;
    return 0;
}

// tính số ngày dôi ra khi có sự xuất hiện của tháng có 31 ngày (từ tương lai đổ về mốc)
int month_31SumCheck_Future(int month)
{
    if (month == 12)
        return 7;
    else if (month == 11)
        return 6;
    else if (month == 10)
        return 6;
    else if (month == 9)
        return 5;
    else if (month == 8)
        return 5;
    else if (month == 7)
        return 4;
    else if (month == 6)
        return 3;
    else if (month == 5)
        return 3;
    else if (month == 4)
        return 2;
    else if (month == 3)
        return 2;
    else if (month == 2)
        return 1;
    else if (month == 1)
        return 1;
    else
        return 0;
}

// tính số ngày dôi ra khi có sự xuất hiện của tháng có 31 ngày ( từ quá khứ cho đến mốc)
int month_31SumCheck_Past(int month)
{
    if (month == 12)
        return 1;
    else if (month == 11)
        return 1;
    else if (month == 10)
        return 2;
    else if (month == 9)
        return 2;
    else if (month == 8)
        return 3;
    else if (month == 7)
        return 4;
    else if (month == 6)
        return 4;
    else if (month == 5)
        return 5;
    else if (month == 4)
        return 5;
    else if (month == 3)
        return 6;
    else if (month == 2)
        return 6;
    else if (month == 1)
        return 7;
    else
        return 0;
}

// kiểm tra xem tháng truyền vào có phải có 31 ngày không
int month_31Check(int month)
{
    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 1;
    default:
        return 0;
    }
}

// tính tổng số ngày dôi ra khi có sự xuất hiện của năm nhuận
int leapYear_sumCheck(int year, int year_gap)
{
    int sum = 0;
    // dùng để tính (tổng số ngày dôi ra) của năm nhuận ở phía sau mốc
    for (int i = 0; i < year_gap; i++)
    {
        if (leapYear_check(year + i))
            sum++;
    }
    // dùng để tính (tổng số ngày dôi ra) của năm nhuận ở phía trước mốc
    for (int i = 0; i > year_gap; i--)
    {
        if (leapYear_check(year - i))
            sum++;
    }
    return sum;
}

// chuyển ngày thành thứ khi biết mốc (thứ) và khoảng cách
int day_refreshToNewWeek(int day_convert, int rank_landmark, int year_gap)
{
    int rank;
    if (year_gap >= 0)
    {
        rank = rank_landmark + (day_convert % 7);
        if (rank > 8)
            rank -= 7;
    }
    else
    {
        rank = rank_landmark - (day_convert % 7);
        if (rank < 2)
            rank = 9 - (abs(day_convert % 7) - (rank_landmark - 2));
    }
    return rank;
}

// chuyển theo đơn vị ngày khi biết ngày, tháng, năm và ngày chọn làm mốc
int day_convert(int day, int month, int year)
{
    int day_convert = 0;
    int year_gap = year - year_landmark;
    if (year_gap >= 0)
    {
        if (year_gap)
        {
            day_convert += (year_gap * 365) + leapYear_sumCheck(year_landmark, abs(year_gap));
        }
        if (month > 2)
        {
            day_convert += (30 * (month - 1)) + month_31SumCheck_Future(month - 1) - 2 + day - 1; //"day - 1": tính đến đầu của ngày hôm đó (không tính ngày hôm đó)
            if (leapYear_check(year))
                day_convert++;
        }
        if (month == 2)
            day_convert += 31 + day;
        if (month == 1)
            day_convert += day;
    }
    else if (year_gap < 0)
    {
        day_convert += ((abs(year_gap) - 1) * 365) + leapYear_sumCheck(year, year_gap);
        if (month <= 2)
        {
            day_convert -= 2;
            if (leapYear_check(year + year_gap))
                day_convert++;
        }
        day_convert += (30 * (12 - month)) + month_31SumCheck_Past(month) + (30 - day) + 1;
    }
    return day_convert;
    // Khi làm việc với thời gian, ta mặc định khi nói đến ngày 16 là ta nói đến 0h00p ngày 16(hay từ 23h59p + 1 ngày 15) (tức là ngày 16 thì chỉ tính đến hết ngày 15)
}

// kiểm tra input có hợp lệ hay không
int input_checkValidation(int day, int month, int year)
{
    if (!day_check(day, month))
    {
        cout << "Ngày không hợp lệ, vui lòng nhập lại: " << endl;
        return false;
    }
    if (!month_31Check(month) && day == 31)
    {
        cout << "Tháng " << month << " chỉ có 30 ngày, vui lòng nhập lại: " << endl;
        return false;
    }
    if (!leapYear_check(year) && month == 2 && day == 29)
    {
        cout << "Năm " << year << " không phải năm nhuận, tháng " << month << " chỉ có 28 ngày, vui lòng nhập lại: " << endl;
        return false;
    }
    if (!month_check(month))
    {
        cout << "Tháng không hợp lệ, vui lòng nhập lại: " << endl;
        return false;
    }
    if (!year_check(year))
    {
        cout << "Năm hợp lệ nằm trong khoảng từ 1000 đến 2200, vui lòng nhập lại: " << endl;
        return false;
    }
    return true;
}
int main()
{
    int day, month, year;
    int rank;
    int check = false; // đkien dùng để check input validation
    time_t now = time(nullptr);
    tm *local_time = std::localtime(&now);
    int day_Now = local_time->tm_mday;
    int month_Now = local_time->tm_mon + 1;
    int year_Now = local_time->tm_year + 1900;
    cout << "_______Thời gian không chờ đợi ai________\n"
         << endl;
    while (!check)
    {
        cout << "Nhập vào ngày chỉ định: " << endl;
        cin >> day;
        cout << "Nhập vào tháng chỉ định: " << endl;
        cin >> month;
        cout << "Nhập vào năm chỉ định: " << endl;
        cin >> year;
        check = input_checkValidation(day, month, year);
    }
    cout << "-------Kết quả:-------\n " << endl;
    rank = day_refreshToNewWeek(day_convert(day, month, year), rank_landmark, year - year_landmark);
    if (rank == 8)
        cout << "Chủ nhật rùi ngủ nướng hoyy ~~" << endl;
    else
        cout << "Thứ " << rank << endl;
    cout << "---------" << endl;
    int day_LandMarkToNow = day_convert(day_Now, month_Now, year_Now);
    int day_LandMarkToInput = day_convert(day, month, year);
    int day_Subtract = day_LandMarkToInput - day_LandMarkToNow;
    if (year - year_landmark >= 0)
    {
        if (day_Subtract > 0)
        {
            cout << "Còn " << day_Subtract - 1 << " ngày nữa" << endl;
        }
        else if (!day_Subtract)
            cout << "Tới lịch gòii trời má ơi !!!" << endl;
        else
        {
            cout << "Đã " << abs(day_Subtract) << " ngày trôi qua..." << endl;
        }
    }
    else
    {
        cout << "Tính đến ngày " << day_Now << " / " << month_Now << " / " << year_Now << " là " << endl;
        cout << day_LandMarkToInput + day_LandMarkToNow << "ngày" << endl;
    }
    cout << "---------" << endl;
}