Это итоговый проект курса "Разработчик на C++" образовательной платформы Skillbox.

В данном проекте использованы следующие технологии:
  - Язык программирования C++;
  - Система сборки CMake;
  - Сторонняя библиотека JSON for Modern C++;
  - Система тестирования Google Test.

Для запуска проекта потребуется:
  - Скачать библиотеку JSON for Modern C++ по ссылке: https://github.com/nlohmann/json;
  - Распаковать библиотеку в корневой каталог проекта;
  - Переименовать папку json-develop в nlohmann_json;

Для работы Google Test без интернета необходимо:
  - Скачать библиотеку по ссылке: https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip;
  - Распаковать библиотеку в корневой каталог проекта;
  - Переименовать папку googletest-main в googletest;
  - В файл CMakeLists.txt добавить команду add_subdirectory(googletest).

После сборки проекта необходимо в папку src, находящуюся в корневом каталоге программы, переместить папки json и resources.
В папке json находятся файлы config.json, answers.json и requests.json. Файл config.json предназначен для настройки проекта. В файле requests.json находятся поисковые запросы по которым выполняется поиск при запуске программы.
В answers.json записываются результаты обработки поисковых запросов.
В папке resources находятся тектовые файлы по которым выполняется поиск.
Список команд для работы с проектом:
test - Запуск тестов;
update - Запуск обновления списка файлов;
search - Запуск основной логики программы - обработка поисковых запросов из файла requests.json.
Исходный код проекта находится в папке search_engine, которая является корневым каталогом сборки.
