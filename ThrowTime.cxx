/**
 * \file ThrowTime.cxx
 * \brief Implementation of time related functions.
 */


// std
#include <string>
#include <vector>
#include <chrono>
// Throw
#include "Throw.h"


/**
 * \ingroup Time
 * \brief Get current time.
 */
tm Throw::GetCurrentTime() {
  typedef std::chrono::duration<int,
                                std::ratio_multiply<std::chrono::hours::period,
                                std::ratio<24>>::type> days;

  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::chrono::system_clock::duration tp = now.time_since_epoch();

  days d = std::chrono::duration_cast<days>(tp);
  tp -= d;
  std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(tp);
  tp -= h;
  std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(tp);
  tp -= m;
  std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(tp);
  tp -= s;

  time_t tt = std::chrono::system_clock::to_time_t(now);
  tm local_tm = *localtime(&tt);

  return local_tm;
}

/**
 * \ingroup Time
 * \brief Converts month number to it's name.
 *
 * \param month month number starts at 0
 *
 * Returns three characters long month shortcut.
 */
std::string Throw::MonthNumToName(int month) {
  std::vector<std::string> months;
  months.emplace_back("Jan");
  months.emplace_back("Feb");
  months.emplace_back("Mar");
  months.emplace_back("Apr");
  months.emplace_back("May");
  months.emplace_back("Jun");
  months.emplace_back("Jul");
  months.emplace_back("Aug");
  months.emplace_back("Sep");
  months.emplace_back("Oct");
  months.emplace_back("Nov");
  months.emplace_back("Dec");

  return months.at(month);
}

/**
 * \ingroup Time
 * \brief Get current year as a string.
 */
std::string Throw::GetCurrentYear() {
  tm now = GetCurrentTime();
  int year = now.tm_year + 1900;

  return std::to_string(year);
}

/**
 * \ingroup Time
 * \brief Get current month as a string.
 *
 * Returns three characters long month shortcut.
 */
std::string Throw::GetCurrentMonth() {
  tm now = GetCurrentTime();
  int month = now.tm_mon;

  return MonthNumToName(month);
}

/**
 * \ingroup Time
 * \brief Get current day as a string.
 */
std::string Throw::GetCurrentDay() {
  tm now = GetCurrentTime();
  int day = now.tm_mday;

  std::string sDay;
  if (day < 10) {
    sDay += "0";
  }
  sDay += std::to_string(day);

  return sDay;
}

/**
 * \ingroup Time
 * \brief Get current hour as a string.
 */
std::string Throw::GetCurrentHour() {
  tm now = GetCurrentTime();
  int hour = now.tm_hour;

  std::string sHour;
  if (hour < 10) {
    sHour += "0";
  }
  sHour += std::to_string(hour);

  return sHour;
}

/**
 * \ingroup Time
 * \brief Get current minute as a string.
 */
std::string Throw::GetCurrentMinute() {
  tm now = GetCurrentTime();
  int minute = now.tm_min;

  std::string sMinute;
  if (minute < 10) {
    sMinute += "0";
  }
  sMinute += std::to_string(minute);

  return sMinute;
}

/**
 * \ingroup Time
 * \brief Get current second as a string.
 */
std::string Throw::GetCurrentSecond() {
  tm now = GetCurrentTime();
  int second = now.tm_sec;

  std::string sSecond;
  if (second < 10) {
    sSecond += "0";
  }
  sSecond += std::to_string(second);

  return sSecond;
}

/**
 * \ingroup Time
 * \brief Get current time and date as a string.
 */
std::string Throw::Now() {
  std::string now = GetCurrentHour();
  now += ":";
  now += GetCurrentMinute();
  now += ":";
  now += GetCurrentSecond();
  now += " ";
  now += GetCurrentDay();
  now += " ";
  now += GetCurrentMonth();
  now += " ";
  now += GetCurrentYear();

  return now;
}

/**
 * \ingroup Time
 * \brief Get current date as a string.
 */
std::string Throw::Today() {
  std::string today = GetCurrentYear();
  today += "_";
  today += GetCurrentMonth();
  today += "_";
  today += GetCurrentDay();

  return today;
}

/**
 * \ingroup Time
 * \brief Get date of a previous day as a string.
 */
std::string Throw::Yesterday() {
  tm now = GetCurrentTime();
  int year = now.tm_year + 1900;
  int month = now.tm_mon + 1;
  int day = now.tm_mday;

  if (day == 1) {
    if (month == 1) {
      year = year - 1;
      month = 12;
      day = 31;
    } else if (month == 2) {
      month = 1;
      day = 31;
    } else if (month == 3) {
      month = 2;
      if (year % 4 == 0) {
        day = 29;
      } else {
        day = 28;
      }
    } else if (month == 4 ||
               month == 6 ||
               month == 8 ||
               month == 9 ||
               month == 11) {
      month -= 1;
      day = 31;
    } else if (month == 5 ||
               month == 7 ||
               month == 10 ||
               month == 12) {
      month -= 1;
      day = 30;
    }
  } else {
    day -= 1;
  }

  std::string today = std::to_string(year);
  today += "_";
  today += MonthNumToName(month - 1);
  today += "_";
  if (day < 10) {
    today += "0";
  }
  today += std::to_string(day);

  return today;
}
