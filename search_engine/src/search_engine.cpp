#include "search_engine.h"

	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
	RelativeIndex docRelative; //объявление структуры для записи данных релевантности по документам
	std::vector<RelativeIndex> requestRelative; //вектор релевантности документов для одного запроса
	std::vector<std::vector<RelativeIndex>> relativeList; //вектор релевантности по всем запросам
	std::multimap<int, std::string> words; //список уникальных слов и их частоты для одного запроса
	std::vector<std::multimap<int, std::string>> requestUnique; //вектор уникальных слов и их частоты для всех запросов

	for (int i = 0; i < queries_input.size(); ++i) {//цикл разбивки запроса на отдельные слова и добавление их в список
		std::string bufer;
		for (auto it = queries_input[i].begin(); it != queries_input[i].end(); ++it) {
			if (*it != ' ' && *it != '\n' && it != queries_input[i].end()) {
				bufer.push_back(*it);
			}
			if (!bufer.empty() && (it + 1 == queries_input[i].end() || *it == ' ' || *it == '\n')) {
				int docCounter = 0;
				std::map<std::string, std::vector<Entry>>::iterator iter = _index.freq_dictionary.find(bufer);
				if (iter != _index.freq_dictionary.end()) {
					for (int j = 0; j < _index.freq_dictionary.find(bufer)->second.size(); ++j) { //подсчет общего количества слова по всем документам
						docCounter += _index.freq_dictionary.find(bufer)->second[j].count;
					}
					if (docCounter > 0) {
						words.emplace(docCounter, bufer);
					}
				}
				else {
					words.emplace(0, bufer);
				}
				bufer.clear();
			}
		}
		requestUnique.push_back(words);
		words.clear();
	}
	
	std::set<size_t> docs; //список документов по запросу
	std::vector<std::set<size_t>>reqDocs; //список документов по всем запросам

	for (int i = 0; i < requestUnique.size(); ++i) {
		for (auto it = requestUnique[i].begin(); it != requestUnique[i].end(); ++it) {
			std::set<size_t> docsBufer;
			if (_index.freq_dictionary.find(it->second) != _index.freq_dictionary.end()) {
				for (int j = 0; j < _index.freq_dictionary.find(it->second)->second.size(); ++j) {
					if (it == requestUnique[i].begin()) {
						docs.insert(_index.freq_dictionary.find(it->second)->second[j].doc_id);
					}
					else if(!docs.empty() && docs.find(_index.freq_dictionary.find(it->second)->second[j].doc_id) != docs.end()) {
						docsBufer.insert(_index.freq_dictionary.find(it->second)->second[j].doc_id);
					}
				}
				if (!docsBufer.empty()) {
					docs.clear();
					docs = docsBufer;
					docsBufer.clear();
				}
			}
			else {
				docs.clear();
				docs.insert(0);
			}
		}
		reqDocs.push_back(docs);
		docs.clear();
	}


	for (int i = 0; i < reqDocs.size(); ++i) { //подсчет релевантности
		float rank = 0.0;
		float max = 0.0; 
		for (auto it = reqDocs[i].begin(); it != reqDocs[i].end(); ++it) {
			if (*it != 0) {
				docRelative.doc_id = *it;
				for (auto iter = requestUnique[i].begin(); iter != requestUnique[i].end(); ++iter) {
					for (int j = 0; j < _index.freq_dictionary.find(iter->second)->second.size(); ++j) {
						if (_index.freq_dictionary.find(iter->second)->second[j].doc_id == *it) {
							rank += _index.freq_dictionary.find(iter->second)->second[j].count;
						}
					}
				}
				docRelative.rank = rank;
				if (rank > max) max = rank;
			}
			else {
				docRelative.rank = 0.0;
			}
			requestRelative.push_back(docRelative);

		}
		for (int j = 0; j < requestRelative.size(); ++j) {
			if(requestRelative[j].rank > 0) requestRelative[j].rank /= max;
		}
		relativeList.push_back(requestRelative);
		requestRelative.clear();
	}

	return relativeList;
};

void requestReturn(std::vector<std::vector<RelativeIndex>> relativeList, ConverterJSON con) {
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


int main() {
	//SetConsoleCP(1251); //локализация консоли для вывода кириллицы
	//SetConsoleOutputCP(1251);
	//setlocale(0, "Russian");
	//setlocale(LC_ALL, "Russian");

	std::vector<std::vector<RelativeIndex>> rl;
	std::vector<std::string> s;
	ConverterJSON con;
	InvertedIndex inv;
	
	std::cout << "GetTextDocuments()" << std::endl;
	s = con.GetTextDocuments();
	std::cout << "UpdateDocumentBase()" << std::endl;
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
	SearchServer ss(inv);
	rl = ss.search(requests);

	requestReturn(rl, con);


	std::cout << 10 % 100 << " | " << 10 / 100<< std::endl;
	return 0;
};