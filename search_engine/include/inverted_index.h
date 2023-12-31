#pragma once
#include "convertJSON.h"
#include <map>
#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <thread>
#include <string>
#include <mutex>
#include "gtest/gtest.h"

struct Entry {
	size_t doc_id, count;
	 //������ �������� ��������� ��� ���������� �������� ���������
	bool operator ==(const Entry& other) const {
		return (doc_id == other.doc_id && count == other.count);
	}
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
	friend class SearchServer;
	std::vector<std::string> docs; // ������ ����������� ����������
	std::map<std::string, std::vector<Entry>> freqDictionary; // ���������	�������
};
