#include "gtest/gtest.h"

#include "dataCleaning.h"

using namespace std;
using namespace gwDainese;


TEST(DataCleaningTests, GuessNumInvalid0Success){
    ASSERT_EQ(0, (guessNumInvalid(0)));
}
TEST(DataCleaningTests, GuessNumInvalid1Success){
    ASSERT_EQ(0, (guessNumInvalid(1)));
}
TEST(DataCleaningTests, GuessNumInvalid2Success){
    ASSERT_EQ(0, (guessNumInvalid(2)));
}
TEST(DataCleaningTests, GuessNumInvalid3Success){
    ASSERT_EQ(0, (guessNumInvalid(3)));
}
TEST(DataCleaningTests, GuessNumInvalid4Success){
    ASSERT_EQ(1, (guessNumInvalid(4)));
}
TEST(DataCleaningTests, GuessNumInvalid5Success){
    ASSERT_EQ(1, (guessNumInvalid(5)));
}
TEST(DataCleaningTests, GuessNumInvalid403Success){
    ASSERT_EQ(100, (guessNumInvalid(403)));
}

vector<double> values = {-.1234, -1.23, -333.223, -5};
vector<double> values2 = {.1234, 1.23, 333.223, 5, 432.3312, 6677.88, 
                        0.00001, 3.4432, 333.223, 333.2229};
vector<double> values3 = {0, 1, 2, 3};
 
TEST(DataCleaningTests, cleanVectorsNoRemovals) {
    auto [vec1, vec2, vec3] = cleanVectors((double) -999, (double) 0, values, values2, values3);

    ASSERT_EQ(4, vec1.size());
    ASSERT_EQ(10, vec2.size());
    ASSERT_EQ(4, vec3.size());
}

TEST(DataCleaningTests, cleanVectorsOneRemoval) {
    auto [vec1, vec2, vec3] = cleanVectors((double) -99, (double) 0, values, values2, values3);

    vector<double> expected1 = {-.1234, -1.23, -5};
    vector<double> expected2 = {.1234, 1.23, 5, 432.3312, 6677.88, 
                        0.00001, 3.4432, 333.223, 333.2229};
    vector<double> expected3 = {0, 1, 3};
    ASSERT_EQ(3, vec1.size());
    ASSERT_EQ(9, vec2.size());
    ASSERT_EQ(3, vec3.size());
    ASSERT_EQ(expected1, vec1);
    ASSERT_EQ(expected2, vec2);
    ASSERT_EQ(expected3, vec3);
}

TEST(DataCleaningTests, cleanVectorsTwoRemovalsTwoVecs) {
    auto [vec1, vec3] = cleanVectors((double) -4, (double) 0, values, values3);

    vector<double> expected = {0, 1};
    ASSERT_EQ(2, vec1.size());
    ASSERT_EQ(2, vec3.size());
    ASSERT_EQ(expected, vec3);
}

TEST(DataCleaningTests, NoBadValues){
    ASSERT_EQ(0, GetBadIndexes(values, (double) -9999, (double) 0).size());
}

TEST(DataCleaningTests, OneBadValue){
    auto actual = GetBadIndexes(values, (double) -99999, (double) -0.1235);
    vector<size_t> expected = {0};

    ASSERT_EQ(expected, actual);
}

TEST(DataCleaningTests, TwoBadValues){
    auto actual = GetBadIndexes(values2, (double) 0, (double) 333.223);
    vector<size_t> expected = {4, 5};

    ASSERT_EQ(expected, actual);
}

TEST(DataCleaningTests, TwoGoodValues){
    auto actual = GetBadIndexes(values2, (double) 333.223, (double) 9999);
    vector<size_t> expected = {0, 1, 3, 6, 7, 9};

    ASSERT_EQ(expected, actual);
}

// Full data removal tests.
// 1. no data, no bad data.
// 2. 1 item, no bad data.
// 3. 1 item, 1 bad item.
// 4. 2 items, 1 bad item.
// 5. 6 items, 2 bad items.

TEST(DataCleaningTests, noData){
    vector<int> values;
    vector<size_t> badIndexes;
    auto results = RemoveBadData(values, badIndexes);
    ASSERT_TRUE(results.empty());
}

TEST(DataCleaningTests, oneItemNoBad){
    vector<int> values = {345};
    vector<size_t> badIndexes;
    auto results = RemoveBadData(values, badIndexes);
    ASSERT_EQ(values, results);
}

TEST(DataCleaningTests, oneItemOneBad){
    vector<int> values = {345};
    vector<size_t> badIndexes = {0};
    auto results = RemoveBadData(values, badIndexes);
    ASSERT_TRUE(results.empty());
}

TEST(DataCleaningTests, twoItemsSecondBad){
    vector<int> values = {345, 432};
    vector<size_t> badIndexes = {1};
    auto results = RemoveBadData(values, badIndexes);
    vector<int> expected = {345};
    ASSERT_EQ(expected, results);
}

TEST(DataCleaningTests, twoItemsFirstBad){
    vector<int> values = {345, 432};
    vector<size_t> badIndexes = {0};
    auto results = RemoveBadData(values, badIndexes);
    vector<int> expected = {432};
    ASSERT_EQ(expected, results);
}

TEST(DataCleaningTests, twoItemsTwoBad){
    vector<int> values = {345, 432};
    vector<size_t> badIndexes = {0, 1};
    auto results = RemoveBadData(values, badIndexes);
    ASSERT_TRUE(results.empty());
}

TEST(DataCleaningTests, sixItemsTwoBad){
    vector<int> values = {345, 432, 4, 3, 2, 888};
    vector<size_t> badIndexes = {0, 3};
    auto results = RemoveBadData(values, badIndexes);
    vector<int> expected = {432, 4, 2, 888};
    ASSERT_EQ(expected, results);
}
