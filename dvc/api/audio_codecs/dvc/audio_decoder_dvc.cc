// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#include "dvc/api/audio_codecs/dvc/audio_decoder_dvc.h"

#include <memory>
#include <vector>

#include "absl/types/optional.h"
#include "absl/memory/memory.h"
#include "rtc_base/numerics/safe_conversions.h"

#include "dvc/api/audio_codecs/dvc/dvcaudiodecoderimpl.h"
#include "dvc/dvc_constants.h"

namespace dolby_voice_client {
namespace webrtc_integration {

absl::optional<AudioDecoderDVC::Config> AudioDecoderDVC::SdpToConfig(
    const webrtc::SdpAudioFormat& format) {
  return absl::optional<AudioDecoderDVC::Config>(format);
}

void AudioDecoderDVC::AppendSupportedDecoders(
    std::vector<webrtc::AudioCodecSpec>* specs) {
  for (auto rate : DVC_SAMPLING_RATES) {
    specs->push_back(webrtc::AudioCodecSpec{
        webrtc::SdpAudioFormat{DVC_CODEC_NAME, rate,
                               static_cast<size_t>(DVC_CHANNELS)},
        webrtc::AudioCodecInfo{rate, static_cast<size_t>(DVC_CHANNELS),
                               DVC_BITRATE}});
  }
}

std::unique_ptr<webrtc::AudioDecoder> AudioDecoderDVC::MakeAudioDecoder(
    Config config,
    absl::optional<webrtc::AudioCodecPairId> codec_pair_id) {
  return absl::make_unique<AudioDecoderDVCImpl>(config);
}

}  // namespace webrtc_integration
}  // namespace dolby_voice_client
