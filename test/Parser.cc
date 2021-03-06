#include <gtest/gtest.h>
#include <json/json.h>
#include <fstream>
#include "../Parser.hpp"
#include "../Packet.hpp"
#include "../BattleSetup.hpp"

using namespace WotReplayParser;

class ParserTests : public ::testing::Test{
protected:
    Parser parser;
};

TEST_F(ParserTests, ParseV094) {
    std::ifstream is("data/v0.9.4_replay.wotreplay", std::ios::binary);
    parser.parse(is);
}

TEST_F(ParserTests, ParseV094ClanWars) {
    std::ifstream is("data/v0.9.4_cw_replay.wotreplay", std::ios::binary);
    parser.parse(is);
}

TEST_F(ParserTests, Parse) {
    std::ifstream is("data/test_replay.wotreplay", std::ios::binary);
    parser.parse(is);
}

TEST_F(ParserTests, packetCallbackIsCalled) {
    bool called = false;
    parser.setPacketCallback([&called](const Packet& packet) {
        called = true;
    });
    std::ifstream is("data/test_replay.wotreplay", std::ios::binary);
    parser.parse(is);
    ASSERT_TRUE(called);
}

TEST_F(ParserTests, eosPacketIsReceivedInCallback) {
    bool eosReceived = false;
    parser.setPacketCallback([&eosReceived](const Packet& packet) {
        ASSERT_FALSE(eosReceived);
        if (packet.getType() == Packet::EOS)
            eosReceived = true;
    });
    std::ifstream is("data/test_replay.wotreplay", std::ios::binary);
    parser.parse(is);
    ASSERT_TRUE(eosReceived);
}

TEST_F(ParserTests, getPayload) {
    bool called = false;
    parser.setPacketCallback([&called](const Packet& packet) {
        if (packet.getType() == Packet::BattleSetup) {
            ASSERT_NO_THROW(packet.getPayload<Payload::BattleSetup>());
            called = true;
        }
    });
    std::ifstream is("data/test_replay.wotreplay", std::ios::binary);
    parser.parse(is);
    ASSERT_TRUE(called) << "Did not find a BattleSetup package";
}

TEST_F(ParserTests, getPayloadThrow) {
    bool thrown = false;
    parser.setPacketCallback([&thrown](const Packet& packet) {
        if (packet.getType() != Packet::BattleSetup) {
            ASSERT_ANY_THROW(packet.getPayload<Payload::BattleSetup>());
            thrown = true;
        }
    });
    std::ifstream is("data/test_replay.wotreplay", std::ios::binary);
    parser.parse(is);
    ASSERT_TRUE(thrown) << "Found only BattleSetup packages";
}

TEST_F(ParserTests, extractChatMessages) {
    std::ifstream is("data/test_replay.wotreplay", std::ios::binary);
    ASSERT_TRUE(is.is_open()) << "Unable to open test_replay.wotreplay";
    std::vector<std::string> messages = Parser::extractChatMessages(is);
    // We do make the assumption that there always are some chatting in the replay, this might not be true
    ASSERT_GT(messages.size(), 0);
}

TEST_F(ParserTests, extractJson) {
    std::ifstream is("data/test_replay.wotreplay", std::ios::binary);
    ASSERT_TRUE(is.is_open()) << "Unable to open test_replay.wotreplay";
    std::vector<std::string> jsons = parser.extractJson(is);
    ASSERT_GT(jsons.size(), 0);
    for (auto json : jsons) {
        Json::Value root;
        ASSERT_TRUE(Json::Reader().parse(json, root, true)) << "Unable to parse json from replay";
    }
}
