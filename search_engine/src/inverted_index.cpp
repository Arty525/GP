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
	
	docs = input_docs; //перенос содержимого файлов внутрь класса
	
	for (int i = 0; i < docs.size(); ++i) { //цикл подсчета каждого слова в файлах
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
	* Метод определяет количество вхождений слова word в загруженной базе
	документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
	*/




std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	Entry list;
	list.count = 0;
	list.doc_id = 0;

	std::vector<Entry> wordsCount;

	for (int i = 0; i < docs.size(); ++i) { //цикл поиска искомого слова в файлах и подсчет его количества в каждом файле
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
		list.count = 0; //обнуление данных в структуре для корректного подсчета
		list.doc_id = 0;
	};
	return wordsCount;
}
