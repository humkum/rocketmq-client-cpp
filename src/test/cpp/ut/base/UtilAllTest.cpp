#include "UtilAll.h"
#include "absl/strings/str_split.h"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"
#include <chrono>
#include <cstdint>

namespace rocketmq {

class UtilAllTest : public testing::Test {
public:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(UtilAllTest, testGetIPv4Addresses) {
  spdlog::set_level(spdlog::level::debug);
  std::vector<std::string> addresses = UtilAll::getIPv4Addresses();
  EXPECT_FALSE(addresses.empty());
  for (const auto& ip : addresses) {
    spdlog::debug("IP: {}", ip);
  }
  std::string chosen;
  if (UtilAll::pickIPv4Address(addresses, chosen)) {
    spdlog::debug("The chosen one is: {}", chosen);
  }
}

TEST_F(UtilAllTest, testGetHostIPv4) {
  std::string ip = UtilAll::getHostIPv4();
  EXPECT_TRUE(UtilAll::LOOP_BACK_IP != ip);
  spdlog::debug("Host IP: {}", ip);
}

TEST_F(UtilAllTest, testCompress) {
  std::string src("How are you doing?");
  std::string dst;
  bool success = UtilAll::compress(src, dst);
  EXPECT_TRUE(success);
  EXPECT_FALSE(dst.empty());
}

TEST_F(UtilAllTest, testUncompress) {
  std::string raw("What is your favorite color?");
  std::string compressed;
  EXPECT_TRUE(UtilAll::compress(raw, compressed));
  std::string uncompressed;
  EXPECT_TRUE(UtilAll::uncompress(compressed, uncompressed));
  EXPECT_EQ(raw.length(), uncompressed.length());
  EXPECT_EQ(raw, uncompressed);
}

TEST_F(UtilAllTest, benchmarkTest) {
  std::string raw;
  uint32_t len = 1024 * 1024;
  raw.reserve(len);
  for (uint32_t i = 0; i < len; i++) {
    raw.push_back(i % 128);
  }

  std::string compressed;
  auto now = std::chrono::steady_clock::now();
  UtilAll::compress(raw, compressed);
  auto elapsed = std::chrono::steady_clock::now() - now;
  EXPECT_TRUE(elapsed < std::chrono::milliseconds(100));
  EXPECT_TRUE(len / compressed.length() >= 5);
}

TEST_F(UtilAllTest, split) {
  std::string ip("8.8.8.8");
  std::vector<std::string> segments = absl::StrSplit(ip, '.');
  std::vector<std::string> expected = {"8", "8", "8", "8"};
  EXPECT_EQ(expected, segments);
}

} // namespace rocketmq