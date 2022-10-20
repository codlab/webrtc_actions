// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef DVC_API_AUDIO_CODECS_DONOTHING_AUDIO_DECODER_ADAPTER_H_
#define DVC_API_AUDIO_CODECS_DONOTHING_AUDIO_DECODER_ADAPTER_H_

#include "api/audio_codecs/audio_decoder.h"
#include "api/audio_codecs/audio_format.h"

namespace dolby_voice_client {
namespace webrtc_integration {

class DoNothingDecoderAdapter : public webrtc::AudioDecoder {
 public:
  DoNothingDecoderAdapter(std::unique_ptr<webrtc::AudioDecoder> decoder);
  virtual ~DoNothingDecoderAdapter() = default;

  bool HasDecodePlc() const override;
  size_t DecodePlc(size_t num_frames, int16_t* decoded) override;
  void Reset() override;
  int ErrorCode() override;
  int PacketDuration(const uint8_t* encoded, size_t encoded_len) const override;
  int PacketDurationRedundant(const uint8_t* encoded,
                              size_t encoded_len) const override;
  bool PacketHasFec(const uint8_t* encoded, size_t encoded_len) const override;
  int SampleRateHz() const override;
  size_t Channels() const override;

 protected:
  int DecodeInternal(const uint8_t* encoded,
                     size_t encoded_len,
                     int sample_rate_hz,
                     int16_t* decoded,
                     SpeechType* speech_type) override;

 private:
  std::unique_ptr<webrtc::AudioDecoder> _decoder;
};

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif /* DVC_API_AUDIO_CODECS_DONOTHING_AUDIO_DECODER_ADAPTER_H_ */
