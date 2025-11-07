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

std::string OpenAIClient::chatCompletion(const json& messages) {
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

/*
auto body = json::parse(res->body);
res->body is the HTTP response body from OpenAI, usually a string containing JSON.
json::parse(res->body) converts that JSON string into a nlohmann::json object.
auto just means C++ will automatically set body’s type to json.
{
  "id": "chatcmpl-123",
  "object": "chat.completion",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": "{ \"product_type\": \"Swap\", \"action\": \"Buy\" }"
      },
      "finish_reason": "stop"
    }
  ]
}
return body["choices"][0]["message"]["content"];
body["choices"] → accesses the array of responses from the API.
[0] → takes the first choice (usually there’s only one unless you request multiple).
["message"]["content"] → accesses the actual text content of the assistant’s message.
return → gives you the string inside content, which in your case is the structured JSON string.
So effectively:
You send an RFQ to OpenAI → get a JSON response → parse it → extract the assistant’s reply text.
That reply text is usually the structured JSON string you later parse with json::parse() to get a proper JSON object in C++.
*/
