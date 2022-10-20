// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef DVC_API_AUDIO_CODECS_DONOTHING_AUDIO_ENCODER_ADAPTER_H_
#define DVC_API_AUDIO_CODECS_DONOTHING_AUDIO_ENCODER_ADAPTER_H_

#include "api/audio_codecs/audio_encoder.h"
#include "api/audio_codecs/audio_format.h"

namespace dolby_voice_client {
namespace webrtc_integration {

class DoNothingEncoderAdapter : public webrtc::AudioEncoder {
 public:
  DoNothingEncoderAdapter(std::unique_ptr<webrtc::AudioEncoder> encoder,
                          CodecType codec_type,
                          int payload_type);
  virtual ~DoNothingEncoderAdapter() = default;

  int SampleRateHz() const override;
  size_t NumChannels() const override;
  int RtpTimestampRateHz() const override;
  size_t Num10MsFramesInNextPacket() const override;
  size_t Max10MsFramesInAPacket() const override;
  int GetTargetBitrate() const override;
  void Reset() override;
  bool SetFec(bool enable) override;
  bool SetDtx(bool enable) override;
  bool GetDtx() const override;
  bool SetApplication(Application application) override;
  void SetMaxPlaybackRate(int frequency_hz) override;
  void SetTargetBitrate(int target_bps) override;
  rtc::ArrayView<std::unique_ptr<AudioEncoder>> ReclaimContainedEncoders()
      override;
  bool EnableAudioNetworkAdaptor(const std::string& config_string,
                                 webrtc::RtcEventLog* event_log) override;
  void DisableAudioNetworkAdaptor() override;
  void OnReceivedUplinkPacketLossFraction(
      float uplink_packet_loss_fraction) override;
  void OnReceivedUplinkRecoverablePacketLossFraction(
      float uplink_recoverable_packet_loss_fraction) override;
  void OnReceivedTargetAudioBitrate(int target_bps) override;
  void OnReceivedUplinkBandwidth(int target_audio_bitrate_bps,
                                 absl::optional<int64_t> bwe_period_ms) override;
  void OnReceivedRtt(int rtt_ms) override;
  void OnReceivedOverhead(size_t overhead_bytes_per_packet) override;
  void SetReceiverFrameLengthRange(int min_frame_length_ms,
                                   int max_frame_length_ms) override;
  webrtc::ANAStats GetANAStats() const override;
  virtual absl::optional<std::pair<webrtc::TimeDelta, webrtc::TimeDelta>> GetFrameLengthRange() const override;

 protected:
  EncodedInfo EncodeImpl(uint32_t rtp_timestamp,
                         rtc::ArrayView<const int16_t> audio,
                         rtc::Buffer* encoded) override;

 private:
  std::unique_ptr<webrtc::AudioEncoder> _encoder;
  CodecType _codec_type = CodecType::kOther;
  int _payload_type = 0;
};

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif /* DVC_API_AUDIO_CODECS_DONOTHING_AUDIO_ENCODER_ADAPTER_H_ */
