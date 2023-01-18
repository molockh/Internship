#include <gtest/gtest.h>


bool IsLeapYear(int year) {
  if (year <= 0) return false;

  if (year % 4 == 0) {
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    return true;
  }

  return false;
}

TEST(LeapYearTest, NegativeYear) {
  EXPECT_FALSE(IsLeapYear(-1));
}

TEST(LeapYearTest, ZeroYear) {
  EXPECT_FALSE(IsLeapYear(0));
}

TEST(LeapYearTest, OnlyDivideBy4) {
  EXPECT_TRUE(IsLeapYear(4));
  EXPECT_TRUE(IsLeapYear(8));
  EXPECT_TRUE(IsLeapYear(20));
}

TEST(LeapYearTest, DivideBy4And100) {
  EXPECT_FALSE(IsLeapYear(100));
  EXPECT_FALSE(IsLeapYear(200));
  EXPECT_FALSE(IsLeapYear(300));
}

TEST(LeapYearTest, CorrectLeapYear) {
  EXPECT_TRUE(IsLeapYear(4));
  EXPECT_TRUE(IsLeapYear(400));
  EXPECT_TRUE(IsLeapYear(800));
}

