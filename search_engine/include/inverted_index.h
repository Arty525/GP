#include "convertJSON.h"
#include <map>
#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <thread>
#include <string>
#include <thread>
#include <mutex>

struct Entry {
	size_t doc_id, count;
	// ������ �������� ��������� ��� ���������� �������� ���������
	//bool operator ==(const Entry& other) const {
	//	return (doc_id == other.doc_id && count == other.count);
	//}
};
class InvertedIndex {
friend class SearchServer;
	
public:
	InvertedIndex() = default;
	/**
	* �������� ��� ��������� ���� ����������, �� ������� ����� ���������
	�����
	* @param texts_input ���������� ����������
	*/
	void UpdateDocumentBase(std::vector<std::string> input_docs);
	/**
	* ����� ���������� ���������� ��������� ����� word � ����������� ����
	����������
	* @param word �����, ������� ��������� �������� ���������� ����������
	* @return ���������� �������������� ������ � �������� ����
	*/
	std::vector<Entry> GetWordCount(const std::string& word);
private:
	std::vector<std::string> docs; // ������ ����������� ����������
	std::map<std::string, std::vector<Entry>> freq_dictionary; // ���������	�������
};
