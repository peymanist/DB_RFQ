#pragma once
#include "OpenAIClient.h"

class RFQProcessor {
public:
    RFQProcessor(OpenAIClient& client);
    std::optional<std::string>  process(const std::string& rfqText);

private:
    OpenAIClient& api;
};