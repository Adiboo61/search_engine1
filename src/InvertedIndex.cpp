#include "InvertedIndex.h"

//не открывается файлы для чтения ИСПРАВИТЬ!!!!!!!!!!


// Обновить или заполнить базу документов, по которой будем совершать
// @param texts input содержимое документов
void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    std::vector<std::thread> threads;
    std::mutex freq_mutex;
    docs.clear();  // обнуляем старые документы
    docs.resize(input_docs.size());
    freq_dictionary.clear();

    for (size_t doc_id = 0; doc_id < input_docs.size(); ++doc_id) {
        threads.emplace_back([this, doc_id, &input_docs, &freq_mutex]() {
            std::stringstream buffer;
            if (std::filesystem::exists(input_docs[doc_id])) {
                std::ifstream file(input_docs[doc_id]);
                if (!file.is_open()) {
                    std::cerr << "Error: Cannot open file " << input_docs[doc_id] << std::endl;
                    return;
                }
                buffer << file.rdbuf();
            }
            else
            {
                buffer << input_docs[doc_id];
            }

            std::string document_text = buffer.str();
            std::map<std::string, size_t> count_word;
            std::string word;

            std::istringstream iss(document_text);
            while (iss >> word) {
                count_word[word]++;
            }

            {
                std::lock_guard<std::mutex> guard(freq_mutex);
                docs[doc_id] = document_text; // сохраняем исходный текст
                for (const auto& [key, count] : count_word) {
                    freq_dictionary[key].push_back({ doc_id, count });
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }
}


std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    if (freq_dictionary.count(word)) {
        auto result = freq_dictionary[word];
        std::sort(result.begin(), result.end(), [](const Entry& a, const Entry& b) {
            return a.doc_id < b.doc_id;
        });
        return result;
    } else {
        return {};
    }
}