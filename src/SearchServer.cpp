#include "SearchServer.h"


std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> results;

    for (const std::string &query: queries_input) {
        std::istringstream iss(query);
        std::map<size_t, float> doc_relevance;
        std::string word;

        while (iss >> word) {
            std::vector<Entry> entries = _index.GetWordCount(word);
            for (const auto &entry: entries) {
                doc_relevance[entry.doc_id] += entry.count;
            }
        }

        float max_relevance = 0.0f;
        for (const auto &[_, relevance]: doc_relevance) {
            if (relevance > max_relevance) {
                max_relevance = relevance;
            }
        }

        std::vector<RelativeIndex> rel_results;
        for (const auto &[doc_id, abs_rel]: doc_relevance) {
            rel_results.push_back({doc_id, max_relevance ? abs_rel / max_relevance : 0});
        }

        std::sort(rel_results.begin(), rel_results.end(), [](const RelativeIndex &a, const RelativeIndex &b) {
            return b.rank < a.rank;
        });

        size_t max_results = 5;
        if (rel_results.size() > max_results) {
            rel_results.resize(max_results);
        }

        results.push_back(rel_results);
    }

    return results;
}