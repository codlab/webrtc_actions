// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef WEBRTC_DVC_CONFERENCE_FACTORY_ENV_H_
#define WEBRTC_DVC_CONFERENCE_FACTORY_ENV_H_

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace dolby_voice_client {
namespace webrtc_integration {

enum class DvcRTPEncoding : int32_t {
  DVC2,
  PCMU,
  PCMA,
  G722,
  TELEPHONE_EVENT,
};

enum class DVC2ConnectionMode : int32_t {
  CC,
  CS,
};

enum class DVC2SourceSide : int32_t {
  CLIENT,
  SERVER,
};

struct DvcAudioCodec {
  DvcRTPEncoding encoding;
  uint32_t clock_rate;
  uint32_t rtp_payload_type;
  bool has_dvc2_params;
  DVC2ConnectionMode dvc2_connection_mode;
  DVC2SourceSide dvc2_source_side;
  uint32_t dvc2_protocol_version;
  bool operator==(const DvcAudioCodec& other) const {
    return encoding == other.encoding && clock_rate == other.clock_rate &&
           rtp_payload_type == other.rtp_payload_type &&
           has_dvc2_params == other.has_dvc2_params &&
           dvc2_connection_mode == other.dvc2_connection_mode &&
           dvc2_source_side == other.dvc2_source_side &&
           dvc2_protocol_version == other.dvc2_protocol_version;
  }
};

class DvcStatus {
 public:
  constexpr DvcStatus() = default;
  DvcStatus(std::string file, int line, std::string func, std::string expr, std::string result);
  DvcStatus(DvcStatus&&) = default;
  DvcStatus(const DvcStatus&) = default;
  DvcStatus& operator=(DvcStatus&&) = default;
  DvcStatus& operator=(const DvcStatus&) = default;

  static DvcStatus Ok() { return DvcStatus(); }

  const std::string& GetError() const& { return m_error; }
  std::string&& GetError() && { return std::move(m_error); }

  operator bool() const { return m_error.empty(); }

 private:
  std::string m_error = {};
};

enum class DvcDirection {
  INPUT,
  OUTPUT,
};

class DvcConferenceBase {
 public:
  class RtpSink {
   public:
    virtual ~RtpSink() {}
    virtual void SendRtp(const char* data,
                         size_t size,
                         int64_t system_time_us) = 0;
  };

  virtual ~DvcConferenceBase() {}
  virtual DvcStatus Update(const DvcAudioCodec& codec) = 0;
  virtual void Stop() = 0;
  virtual void HandleReceivedRtpPacket(const char* data, size_t size) = 0;

  virtual DvcStatus SetMinimumPlayoutDelay(int32_t delay_ms) = 0;
  virtual DvcStatus SetMute(bool mute, DvcDirection direction) = 0;
  virtual void SetRtpSink(RtpSink& sink) = 0;
};

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif /* WEBRTC__ENV_H_ */
