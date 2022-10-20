// Copyright 2019 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#include "dvc/api/audio_codecs/dvc/dvcaudioencoderimpl.h"

#include "absl/strings/match.h"

namespace dolby_voice_client {
namespace webrtc_integration {

absl::optional<webrtc::AudioCodecInfo> AudioEncoderDVCImpl::QueryAudioEncoder(
    const webrtc::SdpAudioFormat& format) {
  if (absl::EqualsIgnoreCase(format.name.c_str(), GetPayloadName())) {
    for (auto rate : DVC_SAMPLING_RATES) {
      if (format.clockrate_hz == rate)
        return absl::optional<webrtc::AudioCodecInfo>(
            {rate, static_cast<size_t>(DVC_CHANNELS), DVC_BITRATE});
    }
  }
  return absl::optional<webrtc::AudioCodecInfo>();
}

AudioEncoderDVCImpl::AudioEncoderDVCImpl(int payload_type,
                                         const webrtc::SdpAudioFormat& format)
    : clock_rate_hz_(format.clockrate_hz) {}

AudioEncoderDVCImpl::~AudioEncoderDVCImpl() = default;

int AudioEncoderDVCImpl::SampleRateHz() const {
  return clock_rate_hz_;
}

size_t AudioEncoderDVCImpl::NumChannels() const {
  return DVC_CHANNELS;
}

int AudioEncoderDVCImpl::RtpTimestampRateHz() const {
  return clock_rate_hz_;
}

size_t AudioEncoderDVCImpl::Num10MsFramesInNextPacket() const {
  return DVC_NUMBER_10MS_FRAMES_IN_PACKET;
}

size_t AudioEncoderDVCImpl::Max10MsFramesInAPacket() const {
  return DVC_NUMBER_10MS_FRAMES_IN_PACKET;
}

int AudioEncoderDVCImpl::GetTargetBitrate() const {
  return DVC_BITRATE;
}

void AudioEncoderDVCImpl::Reset() {}

bool AudioEncoderDVCImpl::SetFec(bool enable) {
  return true;
}

bool AudioEncoderDVCImpl::SetDtx(bool enable) {
  return true;
}

bool AudioEncoderDVCImpl::SetApplication(Application application) {
  return true;
}

webrtc::AudioEncoder::EncodedInfo AudioEncoderDVCImpl::EncodeImpl(
    uint32_t rtp_timestamp,
    rtc::ArrayView<const int16_t> audio,
    rtc::Buffer* encoded) {
  assert(false);
  return EncodedInfo();
}

absl::optional<std::pair<webrtc::TimeDelta, webrtc::TimeDelta>> AudioEncoderDVCImpl::GetFrameLengthRange() const {
  //TODO
  return absl::optional<std::pair<webrtc::TimeDelta, webrtc::TimeDelta>>();
}


}  // namespace webrtc_integration
}  // namespace dolby_voice_client
