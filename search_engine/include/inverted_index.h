#pragma once
#include <search_engine.h>
#include <thread>
#include <mutex>
#include <map>

struct Entry {
	size_t doc_id, count;
	// ������ �������� ��������� ��� ���������� �������� ���������
	//bool operator ==(const Entry& other) const {
	//	return (doc_id == other.doc_id && count == other.count);
	//}
};
class InvertedIndex {
	
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
	friend SearchServer;
	std::vector<std::string> docs; // ������ ����������� ����������
	std::map<std::string, std::vector<Entry>> freq_dictionary; // ���������	�������
};
