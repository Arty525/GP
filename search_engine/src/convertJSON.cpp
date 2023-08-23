#include "ConvertJSON.h"
/**
* ����� ��� ������ � JSON-�������
*/
	/**
	* ����� ��������� ����������� ������
	* @return ���������� ������ � ���������� ������ �������������
	* � config.json
	*/

	std::vector<std::string> ConverterJSON::GetTextDocuments() {
		std::vector<std::string> content; //������ ����������� ������
		
		nlohmann::json json; //���������� ��� ��������� ����������� ����� ������������

		std::ifstream config("json/config.json"); //�������� ����� config.json ��� ��������� �����������

		if (config.is_open()) {
			config >> json;

			for (int i = 0; i < json["files"].size(); ++i) { //��������� ����������� ������
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
	}; // ������




	/**
	* ����� ��������� ���� max_responses ��� ����������� �����������
	* ���������� ������� �� ���� ������
	* @return
	*/
	int ConverterJSON::GetResponsesLimit() {
		nlohmann::json json; //���������� ��� ��������� ����������� ����� ������������

		std::ifstream config("json/config.json"); //�������� ����� config.json ��� ��������� �����������

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
	* ����� ��������� �������� �� ����� requests.json
	* @return ���������� ������ �������� �� ����� requests.json
	*/
	std::vector<std::string> ConverterJSON::GetRequests() {
		std::vector<std::string> requests; //������ ����������� ������

		nlohmann::json json; //���������� ��� ��������� ����������� ����� ������������

		std::ifstream file("json/requests.json"); //�������� ����� requests.json ��� ��������� �����������

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
	* �������� � ���� answers.json ���������� ��������� ��������
	*/
	void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) { //����� ���������� ���������������� �������

	};