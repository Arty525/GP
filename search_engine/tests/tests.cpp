#include "tests.h"

using namespace std;

TEST(sample_test_case, sample_test)
{
	EXPECT_EQ(1, 1);
}

TEST(TestCaseSearchServer, TestSimple) {
	const vector<string> docs = {
		"milk milk milk milk water water water",
		"milk water water",
		"milk milk milk milk milk water water water water water",
		"americano cappuccino"
	};
	const vector<string> request = { "milk water", "sugar" };
	const std::vector<vector<RelativeIndex>> expected = {
		{
			{2, 1},{0, 0.7},{1, 0.3}
		},
		{
		}
	};
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);
	SearchServer srv(idx);
	std::vector<vector<RelativeIndex>> result = srv.search(request);

	ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5) {
	const vector<string> docs = {
		"london is the capital of great britain",//1
		"paris is the capital of france",//2
		"berlin is the capital of germany",//3
		"rome is the capital of italy",//4
		"madrid is the capital of spain",//5
		"lisboa is the capital of portugal",//6
		"bern is the capital of switzerland",//7
		"moscow is the capital of russia",//8
		"kiev is the capital of ukraine",//9
		"minsk is the capital of belarus",//10
		"astana is the capital of kazakhstan",//11
		"beijing is the capital of china",//12
		"tokyo is the capital of japan",//13
		"bangkok is the capital of thailand",//14
		"welcome to moscow the capital of russia the third rome",//15
		"amsterdam is the capital of netherlands",//16
		"helsinki is the capital of finland",//17
		"oslo is the capital of norway",//18
		"stockholm is the capital of sweden",//19
		"riga is the capital of latvia",//20
		"tallinn is the capital of estonia",//21
		"warsaw is the capital of poland",//22
	};
	const vector<string> request = { "moscow is the capital of russia" };
	const std::vector<vector<RelativeIndex>> expected = {
		{
			{7, 1},
			{14, 1},
			{0, 0.666666687},
			{1, 0.666666687},
			{2, 0.666666687}
		}
	};
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);
	SearchServer srv(idx);
	std::vector<vector<RelativeIndex>> result = srv.search(request);

	ASSERT_EQ(result, expected);
}

using namespace std;

void TestInvertedIndexFunctionality(
	const vector<string>& docs,
	const vector<string>& requests,
	const std::vector<vector<Entry>>& expected
) {
	std::vector<std::vector<Entry>> result;
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);
	for (auto& request : requests) {
		std::vector<Entry> word_count = idx.GetWordCount(request);
		result.push_back(word_count);
	}

	ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
	const vector<string> docs = {
	"london is the capital of great britain",
	"big ben is the nickname for the Great bell of the striking clock"
	};
	const vector<string> requests = { "london", "the" };
	const vector<vector<Entry>> expected = {
		{
			{0, 1}
		},
		{
			{0, 1}, {1, 3}
		}
	};
	TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
	const vector<string> docs = {
		"milk milk milk milk water water water",
		"milk water water",
		"milk milk milk milk milk water water water water water",
		"americano cappuccino"
	};
	const vector<string> requests = { "milk", "water", "cappuccino" };
	const vector<vector<Entry>> expected = {
		{
			{0, 4}, {1, 1}, {2, 5}
		}, {
			{0, 3}, {1, 2}, {2, 5}
		}, {
			{3, 1}
		}
	};
	TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
	const vector<string> docs = {
		"a b c d e f g h i j k l",
		"statement"
	};
	const vector<string> requests = { "m", "statement" };
	const vector<vector<Entry>> expected = { {}, {{1, 1}} };
	TestInvertedIndexFunctionality(docs, requests, expected);
}
