// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#include "dvc/api/audio_codecs/dvcaudiocodecfactories.h"

#include <memory>
#include <vector>

#include "absl/memory/memory.h"

#include "api/audio_codecs/L16/audio_decoder_L16.h"
#include "api/audio_codecs/audio_decoder_factory_template.h"
#include "api/audio_codecs/g711/audio_decoder_g711.h"
#include "api/audio_codecs/g722/audio_decoder_g722.h"  // nogncheck
#include "api/audio_codecs/opus/audio_decoder_opus.h"

#include "api/audio_codecs/L16/audio_encoder_L16.h"
#include "api/audio_codecs/audio_encoder_factory_template.h"
#include "api/audio_codecs/g711/audio_encoder_g711.h"
#include "api/audio_codecs/g722/audio_encoder_g722.h"  // nogncheck
#include "api/audio_codecs/opus/audio_encoder_opus.h"

#if WEBRTC_USE_BUILTIN_ILBC
#include "api/audio_codecs/ilbc/audio_decoder_ilbc.h"  // nogncheck
#include "api/audio_codecs/ilbc/audio_encoder_ilbc.h"  // nogncheck
#endif

#include "dvc/api/audio_codecs/donothingaudiodecoderadapter.h"
#include "dvc/api/audio_codecs/donothingaudioencoderadapter.h"

#include "dvc/api/audio_codecs/dvc/audio_decoder_dvc.h"
#include "dvc/api/audio_codecs/dvc/audio_encoder_dvc.h"

namespace dolby_voice_client {
namespace webrtc_integration {

namespace {

template <typename T>
struct DoNothingDecoder {
  using Config = typename T::Config;
  static absl::optional<Config> SdpToConfig(
      const webrtc::SdpAudioFormat& audio_format) {
    return T::SdpToConfig(audio_format);
  }
  static void AppendSupportedDecoders(
      std::vector<webrtc::AudioCodecSpec>* specs) {
    T::AppendSupportedDecoders(specs);
  }
  static std::unique_ptr<webrtc::AudioDecoder> MakeAudioDecoder(
      const Config& config,
      absl::optional<webrtc::AudioCodecPairId> codec_pair_id = absl::nullopt) {
    std::unique_ptr<webrtc::AudioDecoder> decoder = T::MakeAudioDecoder(config, codec_pair_id);
    if (!decoder)
      return nullptr;
    return absl::make_unique<DoNothingDecoderAdapter>(std::move(decoder));
  }
};

template <typename T>
struct DoNothingEncoder {
  using Config = typename T::Config;
  static absl::optional<Config> SdpToConfig(
      const webrtc::SdpAudioFormat& audio_format) {
    return T::SdpToConfig(audio_format);
  }
  static void AppendSupportedEncoders(
      std::vector<webrtc::AudioCodecSpec>* specs) {
    T::AppendSupportedEncoders(specs);
  }
  static webrtc::AudioCodecInfo QueryAudioEncoder(const Config& config) {
    return T::QueryAudioEncoder(config);
  }
  static std::unique_ptr<webrtc::AudioEncoder> MakeAudioEncoder(
      const Config& config,
      int payload_type,
      absl::optional<webrtc::AudioCodecPairId> codec_pair_id = absl::nullopt) {
    webrtc::AudioEncoder::CodecType codec_type =
        webrtc::AudioEncoder::CodecType::kMaxLoggedAudioCodecTypes;
    if (std::is_same<T, AudioEncoderDVC>::value) {
      codec_type = webrtc::AudioEncoder::CodecType::kOther;
    } else if (std::is_same<T, webrtc::AudioEncoderG722>::value) {
      codec_type = webrtc::AudioEncoder::CodecType::kG722;
    }
#if WEBRTC_USE_BUILTIN_ILBC
    else if (std::is_same<T, webrtc::AudioEncoderIlbc>::value) {
      codec_type = webrtc::AudioEncoder::CodecType::kIlbc;
    }
#endif
    else if (std::is_same<T, webrtc::AudioEncoderG711>::value) {
      codec_type = (payload_type == 0 ? webrtc::AudioEncoder::CodecType::kPcmU
                                      : webrtc::AudioEncoder::CodecType::kPcmA);
    }

    RTC_DCHECK(codec_type !=
               webrtc::AudioEncoder::CodecType::
                   kMaxLoggedAudioCodecTypes);  // Unrecognized codec!

    std::unique_ptr<webrtc::AudioEncoder> encoder =
        T::MakeAudioEncoder(config, payload_type, codec_pair_id);
    if (!encoder)
      return nullptr;
    return absl::make_unique<DoNothingEncoderAdapter>(std::move(encoder),
                                                    codec_type, payload_type);
  }
};

}  // namespace

rtc::scoped_refptr<webrtc::AudioDecoderFactory> CreateDVCAudioDecoderFactory() {
  return webrtc::CreateAudioDecoderFactory<
      webrtc::AudioDecoderOpus, webrtc::AudioDecoderG722,
#if WEBRTC_USE_BUILTIN_ILBC
      webrtc::AudioDecoderIlbc,
#endif
      webrtc::AudioDecoderG711, DoNothingDecoder<AudioDecoderDVC> >();
}

rtc::scoped_refptr<webrtc::AudioEncoderFactory> CreateDVCAudioEncoderFactory() {
  return webrtc::CreateAudioEncoderFactory<
      webrtc::AudioEncoderOpus, webrtc::AudioEncoderG722,
#if WEBRTC_USE_BUILTIN_ILBC
      webrtc::AudioEncoderIlbc,
#endif
      webrtc::AudioEncoderG711, DoNothingEncoder<AudioEncoderDVC> >();
}

}  // namespace webrtc_integration
}  // namespace dolby_voice_client
