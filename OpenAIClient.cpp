#include "OpenAIClient.h"
#include <stdexcept>
using json = nlohmann::json;

OpenAIClient::OpenAIClient(const std::string& apiKey)
    : cli("api.openai.com", 443),
    authorization("Bearer " + apiKey)
{
    cli.set_default_headers({
        {"Authorization", authorization},
        {"Content-Type", "application/json"}
        });
    cli.enable_server_certificate_verification(true);
}

json OpenAIClient::chatCompletion(const json& messages) {
    json request = {
        {"model", "gpt-4"},
        {"messages", messages},
        {"temperature", 0}
    };

    auto res = cli.Post("/v1/chat/completions", request.dump(), "application/json");

    if (!res || res->status != 200) {
        throw std::runtime_error("OpenAI request failed: " +
            std::to_string(res ? res->status : 0));
    }

    auto body = json::parse(res->body);
    return body["choices"][0]["message"]["content"];
}

