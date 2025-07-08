#pragma once

#include <iostream>
#include <vector>
#include "InvertedIndex.h"
#include <cmath>


struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex &other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
/* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,чтобы SearchServer мог узнать частоту слов встречаемых в запросе
*/
class SearchServer {
public:
    SearchServer(InvertedIndex& idx) : _index(idx){};
/*
* Метод обработки поисковых запросов @param queries
input поисковые запросы взятые из файла requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>>search(const std::vector<std::string>& queries_input);
    std::vector<std::vector<std::pair<int, float>>> convertToPairs(
            const std::vector<std::vector<RelativeIndex>>& input);

private:
    InvertedIndex _index;
};