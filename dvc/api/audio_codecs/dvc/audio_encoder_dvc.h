// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef DVC_API_AUDIO_CODECS_DVC_AUDIO_ENCODER_DVC_H_
#define DVC_API_AUDIO_CODECS_DVC_AUDIO_ENCODER_DVC_H_

#include <memory>
#include <vector>

#include "api/audio_codecs/audio_encoder.h"
#include "api/audio_codecs/audio_format.h"
#include "api/audio_codecs/audio_codec_pair_id.h"
#include "absl/types/optional.h"

namespace dolby_voice_client {
namespace webrtc_integration {

struct AudioEncoderDVC {
  using Config = webrtc::SdpAudioFormat;
  static absl::optional<Config> SdpToConfig(
      const webrtc::SdpAudioFormat& audio_format);
  static void AppendSupportedEncoders(
      std::vector<webrtc::AudioCodecSpec>* specs);
  static webrtc::AudioCodecInfo QueryAudioEncoder(const Config& config);
  static std::unique_ptr<webrtc::AudioEncoder> MakeAudioEncoder(
      const Config& config,
      int payload_type,
      absl::optional<webrtc::AudioCodecPairId> codec_pair_id = absl::nullopt);
};

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif  // DVC_API_AUDIO_CODECS_DVC_AUDIO_ENCODER_DVC_H_
