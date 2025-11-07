#include "RFQProcessor.h"
#include <string>
using json = nlohmann::json;

RFQProcessor::RFQProcessor(OpenAIClient& client) : api(client) {}

json RFQProcessor::process(const std::string& rfqText) {
    json messages = json::array({
    {
        {"role", "system"},
        {"content", R"(
You are an assistant that extracts structured RFQ data for interest rate swaps and swaptions.
Always respond ONLY in strict JSON format with the following fields:

{
  "product_type": "Swap" or "Swaption",
  "action": "Buy" or "Sell" or null,
  "option_type": "Payer" or "Receiver" or "ATM" or null,
  "currency": "USD" | "EUR" | "GBP" | "JPY" | etc,
  "notional": number,
  "tenor": "e.g., 10Y" or null,
  "expiry": "e.g., 1Y" or null,
  "start_date": "YYYY-MM-DD" or null,
  "strike_rate": number or "ATM" or null
}

If data is missing, use null. Do not include any text outside the JSON.
)"
        }
    },
    {
        {"role", "user"},
        {"content", rfqText}
    }
        });

    auto result = api.chatCompletion(messages);

    // Try parsing model response as JSON
    try {
        return result;
    }
    catch (...) {
        return { {"error", "Failed to parse model response"}, {"raw", result} };
    }
}
