#include "InvertedIndex.h"

	/**
	* �������� ��� ��������� ���� ����������, �� ������� ����� ���������
	�����
	* @param texts_input ���������� ����������
	*/
//InvertedIndex index;
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	std::mutex freqLock;
	std::vector<std::string> filename; //������ ������ � �����
	nlohmann::json fileList; //������ ������ � ������� json
	nlohmann::json json; //���������� ��� ��������� ����������� ����� ������������ config.json

	const std::string directoryPath = "resources"; // ���� � ����� � �������

	int filesCount = 0; //������� ���������� ������ � �����
	std::cout << "files count cycle1" << std::endl;
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) { //������� ������ � �����
		std::cout << "files count cycle2" << std::endl;
		if (entry.is_regular_file()) {
			filename.push_back(entry.path().string()); //������ �������������� ���� � ����� � ������
			filesCount++;
		}
	}
	std::cout << "Files count end" << std::endl;
	fileList = { {"files", filename} };

	std::fstream file;
	//�������� ����� ������������ ��� ��������� �����������
	file.open("json/config.json");
	if (file.is_open()) {
		file >> json;
		json.update(fileList); //������� ����������� ������ ������ � ����������� ����� ������������
		file.close();
	}
	else {
		std::cout << "config.json not open 1" << std::endl;
	}

	//�������� ����� ������������ ��� ������ ������ �����������
	file.open("json/config.json");
	if (file.is_open()) {
		file << std::setw(2) << json << std::endl;
		file.close();
	}
	else {
		std::cout << "config.json not open 2" << std::endl;
	}

	std::cout << "FILES: " << filesCount << std::endl;
	
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
				freq_dictionary.insert(std::pair<std::string, std::vector<Entry>>(bufer, GetWordCount(bufer)));
				bufer.clear();
					freqLock.unlock();
			}
		}
		} };
		}
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
					list.doc_id = i + 1;
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
		} };
	};
	return wordsCount;
}
