#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>
#include <map>
#include <iostream>

/**
* ����� ��� ������ � JSON-�������
*/
class ConverterJSON {
public:
	//ConverterJSON() = default;
	/**
	* ����� ��������� ����������� ������
	* @return ���������� ������ � ���������� ������ �������������
	* � config.json
	*/
	std::vector<std::string> GetTextDocuments();
	/**
	* ����� ��������� ���� max_responses ��� ����������� �����������
	* ���������� ������� �� ���� ������
	* @return
	*/
	//int GetResponsesLimit();
	/**
	* ����� ��������� �������� �� ����� requests.json
	* @return ���������� ������ �������� �� ����� requests.json
	*/
	//std::vector<std::string> GetRequests();
	/**
	* �������� � ���� answers.json ���������� ��������� ��������
	*/
	//void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
};