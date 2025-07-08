#include "ConverterJson.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <iostream>

int main() {
    try {
        // Создание объектов
        ConverterJson converter;
        InvertedIndex index;

        // Загрузка документов
        std::vector<std::string> docs = converter.GetTextDocuments(); // загрузка списка путей
        // Установка max_responses из конфига
        converter.GetResponsesLimit();
        // Обновление индекса
        index.UpdateDocumentBase(docs);

        // Загрузка запросов
        std::vector<std::string> requests = converter.GetRequests();

        // Поиск
        SearchServer server(index);
        std::vector<std::vector<RelativeIndex>> answers = server.search(requests);

        // Запись ответов в JSON
        converter.putAnswers(answers);

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
}