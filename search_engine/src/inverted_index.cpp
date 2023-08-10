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

	for (int i = 0; i < docs.size(); ++i) {
		std::string::iterator it;
		std::string bufer;
		for (it = input_docs[i].begin(); it != input_docs[i].end(); ++it) {
			if (*it != ',' && *it != '\n') {
				bufer.push_back(*it);
			}
			if (*it == ',' || *it == ' ' || *it == '\n') {
				bufer.push_back(' ');
			}
		}
		docs.push_back(bufer);
		bufer.clear();
	};
};
	/**
	* ����� ���������� ���������� ��������� ����� word � ����������� ����
	����������
	* @param word �����, ������� ��������� �������� ���������� ����������
	* @return ���������� �������������� ������ � �������� ����
	*/




std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	Entry list;
	std::vector<Entry> wordsCount;

	for (int i = 0; i < docs.size(); ++i) {
		std::string::iterator it;
		std::string bufer;
		for (it = docs[i].begin(); it != docs[i].end(); ++it) {
			if (*it != ',' && *it != '\n') {
				bufer.push_back(*it);
			}
			if (bufer == word) {
				list.doc_id = i;
				if (list.count > 0) {
					++list.count;
				}
				else {
					list.count = 1;
				}
				std::cout << i << " | " << bufer << std::endl;
				bufer.clear();
			}
			if (*it == ',' || *it == ' ' || *it == '\n') bufer.clear();
		}
		wordsCount.push_back(list);

		return wordsCount;
	};
}



	 // ���������	�������