// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef DVC_API_AUDIO_CODECS_DVC_AUDIO_ENCODER_DVC_IMPL_H_
#define DVC_API_AUDIO_CODECS_DVC_AUDIO_ENCODER_DVC_IMPL_H_

#include "api/audio_codecs/audio_format.h"
#include "modules/audio_coding/codecs/audio_encoder.h"

#include "dvc/dvc_constants.h"

namespace dolby_voice_client {
namespace webrtc_integration {

class AudioEncoderDVCImpl : public webrtc::AudioEncoder {
 public:
  AudioEncoderDVCImpl(int payload_type, const webrtc::SdpAudioFormat& format);

  ~AudioEncoderDVCImpl() override;

  static constexpr const char* GetPayloadName() { return DVC_CODEC_NAME; }
  static absl::optional<webrtc::AudioCodecInfo> QueryAudioEncoder(
      const webrtc::SdpAudioFormat& format);

  int SampleRateHz() const override;
  size_t NumChannels() const override;
  int RtpTimestampRateHz() const override;
  size_t Num10MsFramesInNextPacket() const override;
  size_t Max10MsFramesInAPacket() const override;
  int GetTargetBitrate() const override;
  void Reset() override;
  bool SetFec(bool enable) override;
  bool SetDtx(bool enable) override;
  bool SetApplication(Application application) override;
  absl::optional<std::pair<webrtc::TimeDelta, webrtc::TimeDelta>> GetFrameLengthRange() const override;
 protected:
  EncodedInfo EncodeImpl(uint32_t rtp_timestamp,
                         rtc::ArrayView<const int16_t> audio,
                         rtc::Buffer* encoded) override;
  int clock_rate_hz_;
};

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif  // DVC_API_AUDIO_CODECS_DVC_AUDIO_ENCODER_DVC_IMPL_H_
