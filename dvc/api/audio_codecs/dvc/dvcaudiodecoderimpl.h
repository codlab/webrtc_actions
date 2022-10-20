// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef DVC_API_AUDIO_CODECS_DVC_AUDIO_DECODER_DVC_IMPL_H_
#define DVC_API_AUDIO_CODECS_DVC_AUDIO_DECODER_DVC_IMPL_H_

#include "api/audio_codecs/audio_format.h"
#include "modules/audio_coding/codecs/audio_decoder.h"

namespace dolby_voice_client {
namespace webrtc_integration {

class AudioDecoderDVCImpl final : public webrtc::AudioDecoder {
 public:
  AudioDecoderDVCImpl(const webrtc::SdpAudioFormat& format);
  void Reset() override;
  int SampleRateHz() const override;
  size_t Channels() const override;
  int PacketDuration(const uint8_t* encoded, size_t encoded_len) const override;

 protected:
  int DecodeInternal(const uint8_t* encoded,
                     size_t encoded_len,
                     int sample_rate_hz,
                     int16_t* decoded,
                     SpeechType* speech_type) override;
  int clock_rate_hz_;
};

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif  // DVC_API_AUDIO_CODECS_DVC_AUDIO_DECODER_DVC_IMPL_H_
