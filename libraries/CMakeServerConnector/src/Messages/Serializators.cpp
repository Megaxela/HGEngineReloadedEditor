#include <Messages/Cast.hpp>
#include <Messages/HelloResponse.hpp>
#include <Messages/HandshakeRequest.hpp>
#include <Messages/HandshakeResponse.hpp>
#include <Messages/GlobalSettingsRequest.hpp>
#include <Messages/GlobalSettingsResponse.hpp>
#include <Messages/SetGlobalSettingsRequest.hpp>
#include <Messages/SetGlobalSettingsResponse.hpp>
#include <Messages/ConfigureRequest.hpp>
#include <Messages/ConfigureResponse.hpp>
#include <Messages/ComputeRequest.hpp>
#include <Messages/ComputeResponse.hpp>
#include <Messages/CodeModelRequest.hpp>
#include <Messages/CodeModelResponse.hpp>
#include <Messages/CTestInfoRequest.hpp>
#include <Messages/CTestInfoResponse.hpp>
#include <Messages/CMakeInputsRequest.hpp>
#include <Messages/CMakeInputsResponse.hpp>
#include <Messages/CacheRequest.hpp>
#include <Messages/CacheResponse.hpp>
#include <Messages/FileSystemWatchersRequest.hpp>
#include <Messages/FileSystemWatchersResponse.hpp>

namespace {
    void basicDeserialize(const nlohmann::json& json, BasicMessage& msg)
    {
        msg.type = Messages::Register::instance().stringTypeToEnum(
            json.value("type", std::string()),
            json.value("inReplyTo", std::string())
        );
        msg.replyTo = json.value("inReplyTo", std::string());
        msg.cookie = json.value("cookie", std::string());

        if (msg.type == Messages::Type::Unknown)
        {
            throw std::invalid_argument("Can't deserialize message");
        }
    }

    nlohmann::json basicSerialize(const BasicMessage& msg)
    {
        nlohmann::json result;

        result["type"] = Messages::Register::instance().enumTypeToString(msg.type);
        result["cookie"] = msg.cookie;

        return result;
    }
}

// Version
void to_json(nlohmann::json& json, const Version& version)
{
    json["major"] = version.major;
    json["minor"] = version.minor;
}

void from_json(const nlohmann::json& json, Version& version)
{
    version.major = json.value("major", 0);
    version.minor = json.value("minor", 0);
}

void HelloResponse::deserialize(const nlohmann::json &json, BasicMessage &msg)
{
    basicDeserialize(json, msg);

    auto& helloResponse = message_cast<HelloResponse>(msg);

    for (auto& version : json.value("supportedProtocolVersions", nlohmann::json::array()))
    {
        helloResponse.supportedProtocolVersions.emplace_back(version);
    }
}

nlohmann::json HandshakeRequest::serialize(const BasicMessage &msg)
{
    auto& handshakeRequest = message_cast<HandshakeRequest>(msg);

    nlohmann::json result = basicSerialize(msg);

    result["protocolVersion"] = handshakeRequest.protocolVersion;
    result["sourceDirectory"] = handshakeRequest.sourceDirectory;
    result["buildDirectory"] = handshakeRequest.buildDirectory;
    result["generator"] = handshakeRequest.generator;

    return result;
}

void HandshakeResponse::deserialize(const nlohmann::json &json, BasicMessage &msg)
{
    basicDeserialize(json, msg);
}

nlohmann::json GlobalSettingsRequest::serialize(const BasicMessage &msg)
{
    return basicSerialize(msg);
}

void GlobalSettingsResponse::deserialize(const nlohmann::json &json, BasicMessage &msg)
{
    basicDeserialize(json, msg);

    auto& globalSettingsResponse = message_cast<GlobalSettingsResponse>(msg);

    globalSettingsResponse.buildDirectory = json.value("buildDirectory", std::string());

    auto capabilities_json = json.value("capabilities", nlohmann::json::object());
    globalSettingsResponse.capabilities.serverMode = capabilities_json.value("serverMode", false);

    auto version_json = capabilities_json.value("version", nlohmann::json::object());
    globalSettingsResponse.capabilities.version.isDirty = version_json.value("isDirty", false);
    globalSettingsResponse.capabilities.version.minor = version_json.value("minor", 0);
    globalSettingsResponse.capabilities.version.major = version_json.value("major", 0);
    globalSettingsResponse.capabilities.version.patch = version_json.value("patch", 0);
    globalSettingsResponse.capabilities.version.string = version_json.value("string", std::string());
    globalSettingsResponse.capabilities.version.suffix = version_json.value("suffix", std::string());

    auto generators_json = capabilities_json.value("generators", nlohmann::json::array());
    
    for (auto& generator_json : generators_json)
    {
        globalSettingsResponse.capabilities.generators.push_back({
            generator_json.value("extraGenerators", std::vector<std::string>()),
            generator_json.value("name", std::string()),
            generator_json.value("platformSupport", false),
            generator_json.value("toolsetSupport", false)
        });
    }

    globalSettingsResponse.checkSystemVars = json.value("checkSystemVars", false);
    globalSettingsResponse.extraGenerator = json.value("extraGenerator", std::string());
    globalSettingsResponse.generator = json.value("generator", std::string());
    globalSettingsResponse.debugOutput = json.value("debugOutput", false);
    globalSettingsResponse.sourceDirectory = json.value("sourceDirectory", std::string());
    globalSettingsResponse.trace = json.value("trace", false);
    globalSettingsResponse.traceExpand = json.value("traceExpand", false);
    globalSettingsResponse.warnUninitialized = json.value("warnUninitialized", false);
    globalSettingsResponse.warnUnused = json.value("warnUnused", false);
    globalSettingsResponse.warnUnusedCli = json.value("warnUnusedCli", false);
}

nlohmann::json SetGlobalSettingsRequest::serialize(const BasicMessage &msg)
{
    auto json = basicSerialize(msg);

    auto& setGlobalSettingsRequest = message_cast<SetGlobalSettingsRequest>(msg);

    json["checkSystemVars"] = setGlobalSettingsRequest.checkSystemVars;
    json["debugOutput"] = setGlobalSettingsRequest.debugOutput;
    json["trace"] = setGlobalSettingsRequest.trace;
    json["traceExpand"] = setGlobalSettingsRequest.traceExpand;
    json["warnUninitialized"] = setGlobalSettingsRequest.warnUninitialized;
    json["warnUnused"] = setGlobalSettingsRequest.warnUnused;
    json["warnUnusedCli"] = setGlobalSettingsRequest.warnUnusedCli;

    return json;
}

void SetGlobalSettingsResponse::deserialize(const nlohmann::json &json, BasicMessage &msg)
{
    basicDeserialize(json, msg);
}

nlohmann::json ConfigureRequest::serialize(const BasicMessage &msg)
{
    auto json = basicSerialize(msg);

    auto& configureRequest = message_cast<ConfigureRequest>(msg);

    json["cacheArguments"] = configureRequest.cacheArguments;

    return json;
}

void ConfigureResponse::deserialize(const nlohmann::json &json, BasicMessage &msg)
{
    basicDeserialize(json, msg);
}

