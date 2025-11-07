#include "RFQProcessor.h"
#include <string>
#include <optional>
using json = nlohmann::json;

RFQProcessor::RFQProcessor(OpenAIClient& client) : api(client) {}

std::optional<std::string> RFQProcessor::process(const std::string& rfqText) {
    json messages = json::array({
    {
        {"role", "system"},
        {"content", R"(
You are an assistant that extracts structured RFQ data for interest rate swaps and swaptions.
if the RFQ mentions a floating rate index (e.g., 3M SOFR, 6M Euribor, 1M LIBOR), include it in the "floating_index" field. 
If no floating rate is mentioned, set "floating_index": null.
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
  "strike_rate": number or "ATM" or null,
  "floating_index": "3M SOFR" | "6M Euribor" | null
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

   
    // Try parsing model response as JSON
    try {
       
        std::optional<std::string> ret = api.chatCompletion(messages);
        return ret;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return std::nullopt;
    }
}
