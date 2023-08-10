#include "search_engine.h"
#include <Windows.h>

#if 0
void listing() { //функция для актуализации списка файлов в папке
	std::vector<std::string> filename;
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

	std::ifstream readfile("json/config.json"); //открытие файла конфигурации для получения содержимого
	readfile >> json;
	json.update(fileList); //слияние актуального списка файлов и содержимого файла конфигурации
	readfile.close();

	std::ofstream writefile("json/config.json"); //открытие файла конфигурации для записи нового содержимого
	writefile << std::setw(2) << json << std::endl;
	writefile.close();

	std::cout << "FILES: " << filesCount << std::endl;
}
#endif

int main() {
	SetConsoleCP(65001); //локализация консоли для вывода кириллицы
	SetConsoleOutputCP(65001);

	//listing();

	std::vector<std::string> s;
	ConverterJSON con;
	InvertedIndex inv;

	s = con.GetTextDocuments();
	inv.UpdateDocumentBase(s);
	

	for (int i = 0; i < s.size(); ++i) {
		std::cout << "FILE " << i+1 << std::endl << s[i] << std::endl;
	}

	int max_responses = con.GetResponsesLimit();
	std::cout << "max_responses: " << max_responses << std::endl;

	std::vector<std::string> requests = con.GetRequests();
	std::cout << "requests: " << std::endl;
	for (int i = 0; i < requests.size(); ++i) {
		std::cout << requests[i] << std::endl;
	}

	
	return 0;
}