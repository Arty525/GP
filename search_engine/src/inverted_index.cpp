#include "inverted_index.h"


	/**
	* �������� ��� ��������� ���� ����������, �� ������� ����� ���������
	�����
	* @param texts_input ���������� ����������
	*/
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	std::mutex freqLock;

	docs = input_docs; //������� ����������� ������ ������ ������
	
	for (int i = 0; i < docs.size(); ++i) { //���� �������� ������� ����� � ������
		std::string::iterator it;
		std::string bufer;
		std::jthread thr { [this, &i, &bufer, &it, &freqLock] {
			for (it = docs[i].begin(); it != docs[i].end(); ++it) {
				if (*it != ',' && *it != '\n' && *it != ' ') {
					bufer.push_back(*it);
				}
				else {
					freqLock.lock();
					freqDictionary.insert(std::pair<std::string, std::vector<Entry>>(bufer, GetWordCount(bufer)));
					bufer.clear();
					freqLock.unlock();
				}
			}
		} };
	}
};
	/*
	* ����� ���������� ���������� ��������� ����� word � ����������� ����
	����������
	* @param word �����, ������� ��������� �������� ���������� ����������
	* @return ���������� �������������� ������ � �������� ����
	*/


std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	std::mutex countLock;
	Entry list;
	list.count = 0;
	list.doc_id = 0;

	std::vector<Entry> wordsCount;

	for (int i = 0; i < docs.size(); ++i) { //���� ������ �������� ����� � ������ � ������� ��� ���������� � ������ �����
		std::string::iterator it;
		std::string bufer;
		std::jthread thr{ [this, &i, &bufer, &it, &list, &wordsCount, &word, &countLock] {
			for (it = docs[i].begin(); it != docs[i].end(); ++it) {
				if (*it != ',' && *it != '\n' && *it != ' ' && *it != '.') {
					bufer.push_back(*it);
				}
				if (bufer == word) {
					list.doc_id = i;
					++list.count;
					bufer.clear();
				}
				if (*it == ',' || *it == ' ' || *it == '\n' || *it == '.') bufer.clear();
			}
			countLock.lock();
			if (list.count > 0) {
				wordsCount.push_back(list);
			}
			countLock.unlock();
			list.count = 0; //��������� ������ � ��������� ��� ����������� ��������
			list.doc_id = 0;
		} 
		};
	};
	return wordsCount;
}
