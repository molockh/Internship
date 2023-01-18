#include <vector>
#include <algorithm>
#include <gtest/gtest.h>


int GetProductsAmount(std::vector<int> prices, int amountOfMoney) {
  std::sort(prices.begin(), prices.end());

  int sum = 0;
  for (int i = 0; i < prices.size(); ++i) {
    sum += prices[i];
    if (sum > amountOfMoney) {
      return i;
    }
  }

  return prices.size();
}

TEST(GetProductsAmountTests, EmptyPrices) {
std::vector<int> prices;

EXPECT_EQ(GetProductsAmount(prices, -1), 0);
EXPECT_EQ(GetProductsAmount(prices, 0), 0);
EXPECT_EQ(GetProductsAmount(prices, 1), 0);
}

TEST(GetProductsAmountTests, OrderedPrices) {
std::vector<int> prices = {1, 2, 3, 4, 5, 6, 7, 8};

EXPECT_EQ(GetProductsAmount(prices, 11), 4);
EXPECT_EQ(GetProductsAmount(prices, 0), 0);
EXPECT_EQ(GetProductsAmount(prices, -1), 0);
}

TEST(GetProductsAmountTests, UnorderedPrices) {
std::vector<int> prices = {8, 7, 6, 5, 4, 3, 2, 1};

EXPECT_EQ(GetProductsAmount(prices, 11), 4);
EXPECT_EQ(GetProductsAmount(prices, 0), 0);
EXPECT_EQ(GetProductsAmount(prices, -1), 0);
}

TEST(GetProductsAmountTests, DuplicatedPrices) {
std::vector<int> prices = {1, 1, 2, 2, 3, 3, 4, 5};

EXPECT_EQ(GetProductsAmount(prices, 4), 3);
EXPECT_EQ(GetProductsAmount(prices, 0), 0);
EXPECT_EQ(GetProductsAmount(prices, -1), 0);
}

TEST(GetProductsAmountTests, NegativePrices) {
std::vector<int> prices = {-5, -4, -3, -2, -1, 0};

EXPECT_EQ(GetProductsAmount(prices, 1), prices.size());
EXPECT_EQ(GetProductsAmount(prices, 0), prices.size());
EXPECT_EQ(GetProductsAmount(prices, -1), prices.size());
}


