#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "nlohmann/json.hpp"
#include <exception>
#include "SearchServer.h"

class ConverterJson{

private:

    nlohmann::json j_conf;
    nlohmann::json j_req;
    nlohmann::json j_answers;
    std::string name;
    float version;

public:
    ConverterJson() = default;
    /**
  * Метод получения содержимого файлов
  * @return Возвращает список с содержимым файлов перечисленных в config.json
  */
    std::vector<std::string>GetTextDocuments();

/**
* Метод считывает поле maxresponses для определения предельного
 * количества ответов на один запрос @return
*/
    int GetResponsesLimit();

    /* Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json */
    std::vector<std::string> GetRequests();

    // Положить в файл answers.json результаты поисковых запросов
    void putAnswers(std::vector<std::vector<RelativeIndex>>& answers);
};