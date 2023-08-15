#include <inverted_index.h>

	/**
	* �������� ��� ��������� ���� ����������, �� ������� ����� ���������
	�����
	* @param texts_input ���������� ����������
	*/
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	std::mutex freqLock;
	std::vector<std::string> filename; //������ ������ � �����
	nlohmann::json fileList; //������ ������ � ������� json
	nlohmann::json json; //���������� ��� ��������� ����������� ����� ������������ config.json

	const std::string directoryPath = "resources"; // ���� � ����� � �������

	int filesCount = 0; //������� ���������� ������ � �����

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) { //������� ������ � �����

		if (entry.is_regular_file()) {
			filename.push_back(entry.path().string()); //������ �������������� ���� � ����� � ������
			filesCount++;
		}
	}

	fileList = { {"files", filename} };

	std::fstream file;
	//�������� ����� ������������ ��� ��������� �����������
	file.open("json/config.json");
	file >> json;
	json.update(fileList); //������� ����������� ������ ������ � ����������� ����� ������������
	file.close();

	//�������� ����� ������������ ��� ������ ������ �����������
	file.open("json/config.json");
	file << std::setw(2) << json << std::endl;
	file.close();

	std::cout << "FILES: " << filesCount << std::endl;
	
	docs = input_docs; //������� ����������� ������ ������ ������
	
	for (int i = 0; i < docs.size(); ++i) { //���� �������� ������� ����� � ������
		std::string::iterator it;
		std::string bufer;
		for (it = docs[i].begin(); it != docs[i].end(); ++it) {
			if (*it != ',' && *it != '\n' && *it != ' ') {
				bufer.push_back(*it);
			}
			else {
				freq_dictionary.insert(std::pair<std::string, std::vector<Entry>>(bufer, GetWordCount(bufer)));
				bufer.clear();
			}
		}
	};

	for (std::map<std::string, std::vector<Entry>>::iterator it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {
		for (int i = 0; i < it->second.size(); ++i) {
			std::cout << it->first << " - " << it->second[i].doc_id << ":" << it->second[i].count << std::endl;
		}
	}
};
	/**
	* ����� ���������� ���������� ��������� ����� word � ����������� ����
	����������
	* @param word �����, ������� ��������� �������� ���������� ����������
	* @return ���������� �������������� ������ � �������� ����
	*/




std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	Entry list;
	list.count = 0;
	list.doc_id = 0;

	std::vector<Entry> wordsCount;

	for (int i = 0; i < docs.size(); ++i) { //���� ������ �������� ����� � ������ � ������� ��� ���������� � ������ �����
		std::string::iterator it;
		std::string bufer;
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
		if (list.count > 0) {
			wordsCount.push_back(list);
		}
		list.count = 0; //��������� ������ � ��������� ��� ����������� ��������
		list.doc_id = 0;
	};
	return wordsCount;
}
