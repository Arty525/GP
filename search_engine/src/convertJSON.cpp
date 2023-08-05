#include "convertJSON.h"
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
		}
		else {
			std::cout << "NOT OPEN" << std::endl;
		}

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
				std::cout << "NOT OPEN" << std::endl;
			}
		}

		return content;
	}; // ������




	/**
	* ����� ��������� ���� max_responses ��� ����������� �����������
	* ���������� ������� �� ���� ������
	* @return
	*/
	//int GetResponsesLimit() {

	//};
	/**
	* ����� ��������� �������� �� ����� requests.json
	* @return ���������� ������ �������� �� ����� requests.json
	*/
	//std::vector<std::string> GetRequests() {
		
	//};
	/**
	* �������� � ���� answers.json ���������� ��������� ��������
	*/
	//void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {

	//};