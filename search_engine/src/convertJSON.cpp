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

			for (int i = 0; i < json["files"].size(); ++i) { //��������� ����������� ������
				std::string path = json["files"][i];
				std::string str;
				std::ifstream file(path);

				if (file.is_open()) {
					std::string str((std::istreambuf_iterator<char>(file)),
						(std::istreambuf_iterator<char>()));
					content.push_back(str);
					file.close();
				}
			}
		}
		else {
			std::cerr << "Error! Coudn't open config.json (GetTextDocuments(), line 19)" << std::endl;
		}
		return content;
	};




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
			std::cerr << "Error! Coudn't open config.json (GetResponsesLimit(), line 54)" << std::endl;
		}

		config.close();
		return max_responses;
	};

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
			std::cerr << "Error! Coudn't open requests.json (GetRequests(), line 79)" << std::endl;
		}

		nlohmann::json::iterator it = json["requests"].begin();

		while (it != json["requests"].end()) {
			requests.push_back(*it);
			++it;
		}

		file.close();
		return requests;
	};

	/**
	* �������� � ���� answers.json ���������� ��������� ��������
	*/
	void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
		int docId;
		float rank;
		std::string reqId;
		bool result;

		nlohmann::json relative = {
					{"doc_id", docId},
					{"rank", rank} 
		};

		std::vector<nlohmann::json> docsRelative;

		nlohmann::json request;

		std::vector<nlohmann::json> reqList;

		std::ofstream file;

		file.open("json/answers.json");
		if (file.is_open()) {
			for (int i = 0; i < answers.size(); ++i) {
				result = !answers[i].empty();
				reqId = "request";
				int index = i + 1;
				reqId += std::to_string(index / 100);
				reqId += std::to_string(index / 10);
				reqId += std::to_string(index - (index / 10 * 10));


				for (int j = 0; j < answers[i].size(); ++j) {

					if (result) {
						docId = answers[i][j].first;
						rank = answers[i][j].second;
						relative = {
						{"doc_id", docId},
						{"rank", rank} };
					}

					docsRelative.push_back(relative);
					relative.clear();
				}

				if (result && docsRelative.size() > 1) request = { reqId, {"result", result}, { "relevance", docsRelative} };
				else if (result && docsRelative.size() == 1) request = { reqId, {"result", result}, docsRelative };
				else if (!result) request = { reqId, {"result", result} };
				docsRelative.clear();
				reqList.push_back(request);
				request.clear();
			}

			nlohmann::json record = { "Answers", reqList };
			file << std::setw(2) << record << std::endl;
			file.close();
		}
		else {
			std::cerr << "Error! Coudn't open answers.json (putAnswers(), line 121)" << std::endl;
		}
	};