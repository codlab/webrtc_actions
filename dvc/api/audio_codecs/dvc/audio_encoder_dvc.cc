// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#include "dvc/api/audio_codecs/dvc/audio_encoder_dvc.h"

#include "absl/memory/memory.h"
#include "rtc_base/string_to_number.h"

#include "dvc/api/audio_codecs/dvc/dvcaudioencoderimpl.h"

namespace dolby_voice_client {
namespace webrtc_integration {

absl::optional<AudioEncoderDVC::Config> AudioEncoderDVC::SdpToConfig(
    const webrtc::SdpAudioFormat& format) {
  return absl::optional<webrtc::SdpAudioFormat>(format);
}

void AudioEncoderDVC::AppendSupportedEncoders(
    std::vector<webrtc::AudioCodecSpec>* specs) {
  for (auto rate : DVC_SAMPLING_RATES) {
    const webrtc::SdpAudioFormat fmt = {DVC_CODEC_NAME, rate,
                                        static_cast<size_t>(DVC_CHANNELS)};
    const webrtc::AudioCodecInfo info = QueryAudioEncoder(*SdpToConfig(fmt));
    specs->push_back({fmt, info});
  }
}

webrtc::AudioCodecInfo AudioEncoderDVC::QueryAudioEncoder(
    const AudioEncoderDVC::Config& config) {
  return {config.clockrate_hz, static_cast<size_t>(DVC_CHANNELS), DVC_BITRATE};
}

std::unique_ptr<webrtc::AudioEncoder> AudioEncoderDVC::MakeAudioEncoder(
    const AudioEncoderDVC::Config& config,
    int payload_type,
    absl::optional<webrtc::AudioCodecPairId> codec_pair_id) {
  return absl::make_unique<AudioEncoderDVCImpl>(payload_type, config);
}

}  // namespace webrtc_integration
}  // namespace dolby_voice_client
