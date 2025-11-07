#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "nlohmann/json.hpp"
#include <string>

class OpenAIClient {
public:
    OpenAIClient(const std::string& apiKey);

    std::string chatCompletion(const nlohmann::json& messages);

private:
    httplib::SSLClient cli;
    std::string authorization;
};
