#include "search_engine.h"
#include <Windows.h>

class SearchServer {
public:
	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/
	std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input) {
		std::vector<std::vector<RelativeIndex>> relativeList;
		float relative;
		std::map<std::string, int> words; //список уникальных слов и их частоты для одного запроса

		for (int i = 0; i < queries_input.size(); ++i) {//цикл разбивки запроса на отдельные слова и добавление их в список
			std::string bufer;
			
			for (auto it = queries_input[i].begin(); it != queries_input[i].end(); ++it) {
				if (*it != ' ') {
					bufer.push_back(*it);
				}
				else {
					words.insert(std::pair<std::string, int>(bufer, 0));
					for (int j = 0; j < _index.freq_dictionary.find(bufer)->second.size(); ++j) { //подсчет общего количества слова по всем документам
						words.find(bufer)->second += _index.freq_dictionary.find(bufer)->second[i].count;
					}
					if (words.find(bufer)->second == 0) {
						std::cout << bufer << "0 docs found" << std::endl;
					}
					bufer.clear();
					//дописать расчет релевантности
				}
			}

		}
	};
private:
	InvertedIndex _index;
};

int main() {
	SetConsoleCP(65001); //локализация консоли для вывода кириллицы
	SetConsoleOutputCP(65001);

	//listing();

	std::vector<std::string> s;
	ConverterJSON con;
	InvertedIndex inv;

	s = con.GetTextDocuments();
	inv.UpdateDocumentBase(s);
	

	for (int i = 0; i < s.size(); ++i) {
		std::cout << "FILE " << i+1 << std::endl << s[i] << std::endl;
	}

	int max_responses = con.GetResponsesLimit();
	std::cout << "max_responses: " << max_responses << std::endl;

	std::vector<std::string> requests = con.GetRequests();
	std::cout << "requests: " << std::endl;
	for (int i = 0; i < requests.size(); ++i) {
		std::cout << requests[i] << std::endl;
	}

	
	return 0;
}