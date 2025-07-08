#include "ConverterJson.h"
#include <regex>


std::vector<std::string>ConverterJson::GetTextDocuments(){
    std::vector<std::string>data_files;
    try {
        // проверка файла конфиг
        if(!std::filesystem::exists("JSON/config.json")) {
            throw std::runtime_error("config file is missing.");
        }
        std::ifstream file_conf("JSON/config.json");

        file_conf >> j_conf;

        if(j_conf.find("config") == j_conf.end()){
            throw std::runtime_error("config file is empty");
        }
        // извлечение файлов
        auto data_config = j_conf["config"];
        name = data_config["name"];
        version = data_config["version"];

        if(version < 0.2) throw std::runtime_error("Version config.json incorrect");

        if(j_conf.find("files") == j_conf.end()) throw std::runtime_error("file is empty");

        data_files = j_conf["files"].get<std::vector<std::string>>();
        for (int i = 0; i < data_files.size(); ++i) {
            std::cout << data_files[i];
        }
        //std::cout << "Starting " << name << " version " << version << std::endl;
    }
    catch (const std::exception &e){
        std::cerr << "ERROR\n" << e.what() << std::endl;
    }
    return data_files;
}

/**
* Метод считывает поле maxresponses для определения предельного
 * количества ответов на один запрос @return
*/
int ConverterJson::GetResponsesLimit(){
    int max_responses = j_conf.value("max_responses", 5);
    return max_responses;
}

/* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json */
std::vector<std::string>ConverterJson::GetRequests(){
    std::vector<std::string>req;
    std::ifstream file_req("JSON/requests.json");
    if(!file_req.is_open()){
        throw std::runtime_error("Не удалось открыть файл requests.json");
    }
    file_req >> j_req;
    if (j_req.find("requests") == j_req.end()) {
        throw std::runtime_error("Поле 'requests' отсутствует в requests.json");
    }
    auto raw_requests = j_req["requests"];
    if(raw_requests.size() > 1000) {
        throw std::runtime_error("Превышено максимальное количество запросов (1000)");
    }

    std::regex valid_words("[A-Za-z]+");

    for(auto& item: raw_requests){
        std::string line = item.get<std::string>();
        std::istringstream iss(line);
        std::string word;
        int count = 0;
        while (iss >> word){
            if(!std::regex_match(word,valid_words))
                throw std::runtime_error("Недопустимое слово в запросе: " + word);
            req.push_back(word);
            count++;
        }
        if(count < 1 || count > 10) throw std::runtime_error("Запрос должен содержать от 1 до 10 слов " + line);

    }
    std::cout << "Search words loaded:\n\t{\n\t";
    for (int i = 0; i < req.size(); ++i) {
        std::cout << req[i] << "\n\t";
    }
    std::cout << "}\n";
    return req;
}

// Положить в файл answers.json результаты поисковых запросов
void ConverterJson::putAnswers(std::vector<std::vector<RelativeIndex>>&answers){
    try {
        nlohmann::json json_data;

        for (size_t i = 0; i < answers.size(); ++i) {
            std::string request_name = "request" + std::string(3 - std::to_string(i + 1).length(), '0') + std::to_string(i + 1);

            if (answers[i].empty()) {
                json_data["answers"][request_name]["result"] = "false";
            } else {
                json_data["answers"][request_name]["result"] = "true";
                json_data["answers"][request_name]["relevance"] = nlohmann::json::array();

                for (const auto& entry : answers[i]) {
                    json_data["answers"][request_name]["relevance"].push_back({
                                                                                      {"docid", entry.doc_id},
                                                                                      {"rank", entry.rank}
                                                                              });
                }
            }
        }

        std::ofstream out_file("JSON/answers.json", std::ios::trunc);
        out_file << std::setw(4) << json_data << std::endl;
        out_file.close();

        std::cout << "answers.json has been created successfully" << std::endl;
    }
    catch (const std::exception& e){
        std::cerr << "Error writing answers.json: " << e.what() << std::endl;
    }
}

