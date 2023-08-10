#include <inverted_index.h>

	/**
	* Обновить или заполнить базу документов, по которой будем совершать
	поиск
	* @param texts_input содержимое документов
	*/
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	std::mutex freqLock;
	std::vector<std::string> filename; //список файлов в папке
	nlohmann::json fileList; //список файлов в формате json
	nlohmann::json json; //переменная для получения содержимого файла конфигурации config.json

	const std::string directoryPath = "resources"; // путь к папке с файлами

	int filesCount = 0; //счетчик количества файлов в папке

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) { //подсчет файлов в папке

		if (entry.is_regular_file()) {
			filename.push_back(entry.path().string()); //Запись относительного пути к файлу в вектор
			filesCount++;
		}
	}

	fileList = { {"files", filename} };

	std::fstream file;
	//открытие файла конфигурации для получения содержимого
	file.open("json/config.json");
	file >> json;
	json.update(fileList); //слияние актуального списка файлов и содержимого файла конфигурации
	file.close();

	//открытие файла конфигурации для записи нового содержимого
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
	* Метод определяет количество вхождений слова word в загруженной базе
	документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
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



	 // частотный	словарь