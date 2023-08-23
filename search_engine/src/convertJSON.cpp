#include "ConvertJSON.h"
/**
* Класс для работы с JSON-файлами
*/
	/**
	* Метод получения содержимого файлов
	* @return Возвращает список с содержимым файлов перечисленных
	* в config.json
	*/

	std::vector<std::string> ConverterJSON::GetTextDocuments() {
		std::vector<std::string> content; //вектор содержимого файлов
		
		nlohmann::json json; //переменная для получения содержимого файла конфигурации

		std::ifstream config("json/config.json"); //открытие файла config.json для получения содержимого

		if (config.is_open()) {
			config >> json;

			for (int i = 0; i < json["files"].size(); ++i) { //получение содержимого файлов
				std::string path = json["files"][i];
				std::string str;
				std::ifstream file(path);

				if (file.is_open()) {
					std::cout << "FILE OPEN" << std::endl;
					std::string str((std::istreambuf_iterator<char>(file)),
						(std::istreambuf_iterator<char>()));
					content.push_back(str);
					file.close();
				}
				else {
					std::cout << "NOT OPEN (convertJSON GetTextDocuments 2)" << std::endl;
				}
			}
		}
		else {
			std::cout << "NOT OPEN (convertJSON GetTextDocuments 1)" << std::endl;
		}

		return content;
	}; // ГОТОВО




	/**
	* Метод считывает поле max_responses для определения предельного
	* количества ответов на один запрос
	* @return
	*/
	int ConverterJSON::GetResponsesLimit() {
		nlohmann::json json; //переменная для получения содержимого файла конфигурации

		std::ifstream config("json/config.json"); //открытие файла config.json для получения содержимого

		int max_responses = 5;

		if (config.is_open()) {
			config >> json;
			max_responses = json["config"]["max_responses"];
		}
		else {
			std::cout << "NOT OPEN (convertJSON GetResponsesLimit)" << std::endl;
		}		

		return max_responses;
	}; //READY

	/**
	* Метод получения запросов из файла requests.json
	* @return возвращает список запросов из файла requests.json
	*/
	std::vector<std::string> ConverterJSON::GetRequests() {
		std::vector<std::string> requests; //вектор содержимого файлов

		nlohmann::json json; //переменная для получения содержимого файла конфигурации

		std::ifstream file("json/requests.json"); //открытие файла requests.json для получения содержимого

		if (file.is_open()) {
			file >> json;
		}
		else {
			std::cout << "NOT OPEN (convertJSON GetRequests)" << std::endl;
		}

		nlohmann::json::iterator it = json["requests"].begin();

		while (it != json["requests"].end()) {
			requests.push_back(*it);
			++it;
		}

		return requests;
	};

	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) { //ПОСЛЕ ГОТОВНОСТИ ИНВЕРТИРОВАННОГО ИНДЕКСА

	};