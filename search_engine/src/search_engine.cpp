#include "search_engine.h"

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
	const vector<string> request = {"moscow is the capital of russia"};
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

	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/


bool sortByRank(const RelativeIndex& a, const RelativeIndex& b) { return a.rank > b.rank; }

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
	std::multimap<int, std::string> words; //список уникальных слов и их частоты для одного запроса
	std::vector<std::multimap<int, std::string>> requestUnique; //вектор уникальных слов и их частоты для всех запросов
	std::mutex mutex;
	std::vector<std::string> request;
	std::vector<std::vector<std::string>> requestList;

	for (auto& i : queries_input) {
		std::string str;
		std::jthread thr{ [this, &i, &request, &mutex, &str, &requestList] {
		for (auto& j : i) {
			bool letter = (j != ' ');
			if (str.empty() && letter) {
				str = j;
			}
			else if (!str.empty() && letter) {
				str.push_back(j);
			}
			if (!letter) {
				request.push_back(str);
				str.clear();
			}
		}
		if (!str.empty()) {
			request.push_back(str);
			str.clear();
		}
		mutex.lock();
		requestList.push_back(request);
		request.clear();
		mutex.unlock();
		} };
	}

	for (auto& i : requestList) {
		std::jthread thr{ [this, &i, &words, &mutex, &requestUnique] {
		for (auto& j : i) {
			if (_index.freqDictionary.find(j) != _index.freqDictionary.end()) {
				int count = 0;
				for (auto& it : _index.freqDictionary.find(j)->second) {
					count += it.count;
				}
				words.insert(std::pair(count, j));
			}
			else {
				words.insert(std::pair(0, j));
			}
		}
		mutex.lock();
		requestUnique.push_back(words);
		words.clear();
		mutex.unlock();
		}};
	}
		
	std::vector<std::set<size_t>> requestDocs; // список документов для всех запросов

	for (auto& i : requestUnique) {
		std::set<size_t> docs; // список документов для одного запроса
		std::jthread thr{ [this, &i, &docs, &mutex, &requestDocs] {
			for (auto& j : i) {
				if (_index.freqDictionary.find(j.second) != _index.freqDictionary.end()) { //поиск слова из запроса в частотном словаре
					for (auto& k : _index.freqDictionary.find(j.second)->second) {
						mutex.lock();
						docs.insert(k.doc_id); //получение doc_id для этого слова и запись в список документов по запросу
						mutex.unlock();
					}
				}
			}
			mutex.lock();
			requestDocs.push_back(docs); //составление списка документов по запросам
			mutex.unlock();
		} };
	}

	std::vector<std::vector<RelativeIndex>> result; //вектор релевантности по всем запросам
	ConverterJSON convJson;
	int responses = convJson.GetResponsesLimit();

	for (auto& i : requestDocs) { //получение списка документов по одному запросу
		std::vector<RelativeIndex> requestRelative; //вектор релевантности документов для одного запроса
		std::jthread thr{ [this, &i, &mutex, &requestDocs, &requestRelative, &requestUnique, &result] {
			for (auto& j : i) { //получение номера документа
				requestRelative.push_back(RelativeIndex{ j, 0.0 });
				float rank = 0.0;
				for (auto& k : requestUnique) {//получение списка слов по одному запросу
					for (auto& it : k) {//получение одного слова из запроса
						if (_index.freqDictionary.find(it.second) != _index.freqDictionary.end()) {//проверка наличия слова в частотном словаре
							for (auto& freq : _index.freqDictionary.find(it.second)->second) {//получение count для документов, входящих в запрос
								if (freq.doc_id == j) {
									rank += freq.count;
								}
							}
						}
					}
				}
				requestRelative[requestRelative.size() - 1].rank = rank;
				
			}
		mutex.lock();
		result.push_back(requestRelative);
		mutex.unlock();
		} };
	}

	float maxRank = 0.0;
	for (auto& i : result) {
		float rank = 0.0;
		for (auto& j : i) {
			if (j.rank > maxRank) maxRank = j.rank;
		}
		for (auto& j : i) {
			j.rank /= maxRank;
		}
		std::sort(i.begin(), i.end(), sortByRank);
		if (i.size() > responses) i.resize(responses);
	}

	return result;
};

void answerReturn(std::vector<std::vector<RelativeIndex>> relativeList, ConverterJSON con) {
	std::vector<std::pair<int, float>> reqAnswer;
	std::vector<std::vector<std::pair<int, float>>> answers;

	for (int i = 0; i < relativeList.size(); ++i) {
		for (int j = 0; j < relativeList[i].size(); ++j) {
			reqAnswer.push_back(std::pair<int, float>(relativeList[i][j].doc_id, relativeList[i][j].rank));
		}
		answers.push_back(reqAnswer);
		reqAnswer.clear();
	}
	con.putAnswers(answers);
}

void updateFileList() {
	std::vector<std::string> filename; //список файлов в папке
	nlohmann::json fileList; //список файлов в формате json
	nlohmann::json json; //переменная для получения содержимого файла конфигурации config.json

	const std::string directoryPath = "resources"; // путь к папке с файлами

	int filesCount = 0; //счетчик количества файлов в папке
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) { //подсчет файлов в папке
		if (entry.is_regular_file()) {
			filename.push_back(entry.path().string()); //Запись относительного пути к файлу в вектор
			filesCount++;
		}
	}
	fileList = { {"files", filename} };

	std::fstream file;
	//открытие файла конфигурации для получения содержимого
	file.open("json/config.json");
	if (file.is_open()) {
		file >> json;
		json.update(fileList); //слияние актуального списка файлов и содержимого файла конфигурации
		file.close();
	}
	else {
		std::cerr << "Error! Coudn't open config.json (updateFileList(), line 290)" << std::endl;
	}

	//открытие файла конфигурации для записи нового содержимого
	file.open("json/config.json");
	if (file.is_open()) {
		file << std::setw(2) << json << std::endl;
		file.close();
	}
	else {
		std::cerr << "Error! Coudn't open config.json (updateFileList(), line 301)" << std::endl;
	}

	std::cout << "Files founded: " << filesCount << std::endl;
	std::string command;
	std::cout << "Get files list? (y/n): ";
	std::cin >> command;
	if (command == "y") {
		for (auto& i : filename) {
			std::cout << i << std::endl;
		}
	}

}

int main() {
	std::string command; //команды для работы программы
	std::cout << "Enter command: ";
	std::cin >> command;

	if (command == "test") { //запуск тестов
		testing::InitGoogleTest();
		return RUN_ALL_TESTS();
	}
	if (command == "update") { //обновление списка документов
		updateFileList();
	}
	if (command == "search") { //выполнение поиска
		std::vector<std::vector<RelativeIndex>> result; //результат обработки запросов
		std::vector<std::string> text; //содержимое документов
		ConverterJSON convJson;
		InvertedIndex invIndex;
		text = convJson.GetTextDocuments();
		invIndex.UpdateDocumentBase(text);
		std::vector<std::string> requests = convJson.GetRequests(); //список запросов
		SearchServer ss(invIndex);
		result = ss.search(requests);
		answerReturn(result, convJson);
	}
	return 0;
};