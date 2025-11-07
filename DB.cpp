#include "RFQProcessor.h"
#include <iostream>
#include <string>


using json = nlohmann::json;

int main() {
    // OpenAI API key
    std::string apiKey;
    std::cout << "Enter your OpenAI API key: ";
    std::getline(std::cin, apiKey);

    if (apiKey.empty()) {
        std::cerr << "API key cannot be empty.\n";
        return 1;
    }

    try {
        OpenAIClient client(apiKey);
        RFQProcessor processor(client);

        std::cout << "\nEnter RFQ text (or type 'exit' to quit):\n";
        while (true) {
            std::string rfqText;
            std::cout << "\nRFQ> ";
            std::getline(std::cin, rfqText);

            if (rfqText == "exit" || rfqText == "quit")
                break;

            if (rfqText.empty()) {
                std::cout << "Please enter some text.\n";
                continue;
            }

            std::cout << "\nProcessing...\n";
            auto structured = processor.process(rfqText);

            std::cout << "\nStructured Output:\n";
            std::cout << structured.dump(4) << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\nProgram ended.\n";
    return 0;
}