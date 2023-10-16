#include "InvertedIndex.h"

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
	
	for (auto& r : result) {
		for (auto& rr : r) {
			std::cout << "{" << rr.doc_id << ", " << rr.count << "}" << std::endl;
		}
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
			{1, 1}
		}, 
		{
			{1, 1}, {2, 3}
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
				{1, 4}, {2, 1}, {3, 5}
			}, {
				{1, 3}, {2, 2}, {3, 5}
			}, {
				{4, 1}
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
		const vector<vector<Entry>> expected = {{}, {{2, 1}}};
		TestInvertedIndexFunctionality(docs, requests, expected);
	}

	/**
	* Обновить или заполнить базу документов, по которой будем совершать
	поиск
	* @param texts_input содержимое документов
	*/
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	std::mutex freqLock;

	docs = input_docs; //перенос содержимого файлов внутрь класса
	
	for (int i = 0; i < docs.size(); ++i) { //цикл подсчета каждого слова в файлах
		std::string::iterator it;
		std::string bufer;
		std::jthread thr { [this, &i, &bufer, &it, &freqLock] {
			for (it = docs[i].begin(); it != docs[i].end(); ++it) {
				if (*it != ',' && *it != '\n' && *it != ' ') {
					bufer.push_back(*it);
				}
				else {
					freqLock.lock();
					freq_dictionary.insert(std::pair<std::string, std::vector<Entry>>(bufer, GetWordCount(bufer)));
					bufer.clear();
					freqLock.unlock();
				}
			}
		} };
	}
};
	/*
	* Метод определяет количество вхождений слова word в загруженной базе
	документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
	*/


std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	std::mutex countLock;
	Entry list;
	list.count = 0;
	list.doc_id = 0;

	std::vector<Entry> wordsCount;

	for (int i = 0; i < docs.size(); ++i) { //цикл поиска искомого слова в файлах и подсчет его количества в каждом файле
		std::string::iterator it;
		std::string bufer;
		std::jthread thr{ [this, &i, &bufer, &it, &list, &wordsCount, &word, &countLock] {
			for (it = docs[i].begin(); it != docs[i].end(); ++it) {
				if (*it != ',' && *it != '\n' && *it != ' ' && *it != '.') {
					bufer.push_back(*it);
				}
				if (bufer == word) {
					list.doc_id = i + 1;
					++list.count;
					bufer.clear();
				}
				if (*it == ',' || *it == ' ' || *it == '\n' || *it == '.') bufer.clear();
			}
			countLock.lock();
			if (list.count > 0) {
				wordsCount.push_back(list);
			}
			countLock.unlock();
			list.count = 0; //обнуление данных в структуре для корректного подсчета
			list.doc_id = 0;
		} 
		};
	};
	return wordsCount;
}
