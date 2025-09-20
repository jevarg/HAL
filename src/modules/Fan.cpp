#include "Fan.h"
#include "Logger.h"

#define CHECK(call)                                                            \
  {                                                                            \
    const CC1101::Status state = call;                                         \
    if (state != CC1101::STATUS_OK) {                                          \
      Logger::Error(String("FATAL: ") + #call + " returned " + (int)state);    \
      return false;                                                            \
    }                                                                          \
  }

constexpr double RadioFrequency = 433.92;
constexpr double BitrateKbps = 2.5;
constexpr int8_t OutputPower = -5;

constexpr std::array<uint8_t, 9> MessageHeader{0x0e, 0x88, 0x8e, 0x8e, 0xee, 0xe8, 0x8e, 0xee, 0xe8};
constexpr size_t MessageSize = 13;
constexpr uint8_t MessageSendCount = 5;


namespace HAL {
Fan::Fan(const uint8_t id)
    : Module(id), m_characteristic("0001", BLEWrite), m_cc1101(10) {}

const char *Fan::Name() { return "Fan"; }

bool Fan::Setup() {
  m_service->addCharacteristic(m_characteristic);

  CHECK(m_cc1101.begin());
  m_cc1101.setModulation(CC1101::MOD_ASK_OOK);
  CHECK(m_cc1101.setFrequency(RadioFrequency));
  CHECK(m_cc1101.setDataRate(BitrateKbps));
  CHECK(m_cc1101.setManchester(false));
  CHECK(m_cc1101.setFEC(false));
  m_cc1101.setOutputPower(OutputPower);
  m_cc1101.setPacketLengthMode(CC1101::PKT_LEN_MODE_FIXED, MessageSize);
  m_cc1101.setAddressFilteringMode(CC1101::ADDR_FILTER_MODE_NONE);
  // CHECK(m_cc1101.setPreambleLength(16));
  // CHECK(m_cc1101.setSyncWord(0x3906));
  m_cc1101.setSyncMode(CC1101::SYNC_MODE_NO_PREAMBLE);
  m_cc1101.setCrc(false);
  m_cc1101.setDataWhitening(false);

  return true;
}

bool Fan::Start() {
  return true;
}

bool Fan::IsReady() const { return true; }

void Fan::Update() {
  if (!m_characteristic.written()) {
    return;
  }

  const Query message = m_characteristic.value();
  Logger::Info(String("[Fan] Received message: 0x") +
               String(static_cast<uint32_t>(message), HEX));

  std::array<uint8_t, MessageSize> buffer{};
  memcpy(buffer.data(), MessageHeader.data(), MessageHeader.size());
  memcpy(buffer.data() + MessageHeader.size(), &message, sizeof(message));

  String debugStr;
  for (size_t i = 0; i < buffer.size(); ++i) {
    debugStr += String(buffer[i], HEX);
    if (i + 1 < MessageSize) {
      debugStr += ' ';
    }
  }

  Logger::Debug(debugStr);
  Logger::Info("Transmitting ... ");
  for (uint8_t sentCount = 0; sentCount < MessageSendCount; sentCount++)
  {
    const CC1101::Status status = m_cc1101.transmit(buffer.data(), buffer.size());
    if (status != CC1101::STATUS_OK)
    {
      Logger::Error(status);
    }

    delayMicroseconds(2500);
  }
}

} // namespace HAL