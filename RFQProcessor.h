#pragma once
#include "OpenAIClient.h"

class RFQProcessor {
public:
    RFQProcessor(OpenAIClient& client);
    nlohmann::json process(const std::string& rfqText);

private:
    OpenAIClient& api;
};