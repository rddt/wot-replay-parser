cmake_minimum_required(VERSION 2.8.4)
project(wot-replay-parser)

add_subdirectory(ext/cryptopp/)
add_subdirectory(ext/zlib/)
set(ZLIB_INCLUDE_DIR  "ext/zlib/" "${CMAKE_BINARY_DIR}/ext/zlib/")

include_directories(JsonCpp/ ext/ ${ZLIB_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES BattleResult.cpp BattleSetup.cpp ChatMessage.cpp DataBlock.cpp GameState.cpp GameVersion.cpp MatchData.cpp MatchStart.cpp Packet.cpp Parser.cpp Personal.cpp RoamingSettings.cpp Vehicle.cpp VehiclePosition.cpp UnPickle.cpp Version.cpp JsonCpp/jsoncpp.cpp)
set(INCLUDE_FILES BattleResult.hpp BattleSetup.hpp ChatMessage.hpp Common.hpp DataBlock.hpp GameState.hpp GameVersion.hpp MatchData.hpp MatchStart.hpp Packet.hpp Packets.hpp Parser.hpp Payload.hpp Personal.hpp RoamingSettings.hpp Vehicle.hpp VehiclePosition.hpp Version.hpp UnPickle.hpp)
add_library(wot-replay-parser STATIC ${SOURCE_FILES})
target_link_libraries(wot-replay-parser cryptopp zlibstatic)

# Tests
include(test/GTest.cmake)
set(TEST_FILES test/Version.cc test/BattleResult.cc test/BattleSetup.cc test/ChatMessage.cc test/DataBlock.cc test/GameVersion.cc test/Parser.cc test/RoamingSettings.cc test/VehiclePosition.cc test/UnPickle.cc)
cxx_test(unitTests "${TEST_FILES}" "${CMAKE_CURRENT_SOURCE_DIR}/test/" gtest_main wot-replay-parser cryptopp)

install(TARGETS wot-replay-parser DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include)
