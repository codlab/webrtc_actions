// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#include "dvc/api/audio_codecs/dvc/dvcaudiodecoderimpl.h"

#include "dvc/dvc_constants.h"

namespace dolby_voice_client {
namespace webrtc_integration {

AudioDecoderDVCImpl::AudioDecoderDVCImpl(const webrtc::SdpAudioFormat& format)
    : clock_rate_hz_(format.clockrate_hz) {}

void AudioDecoderDVCImpl::Reset() {}

int AudioDecoderDVCImpl::SampleRateHz() const {
  return clock_rate_hz_;
}

size_t AudioDecoderDVCImpl::Channels() const {
  return DVC_CHANNELS;
}

int AudioDecoderDVCImpl::PacketDuration(const uint8_t*, size_t) const {
  return DVC_NUMBER_10MS_FRAMES_IN_PACKET * clock_rate_hz_ / 100;
}

int AudioDecoderDVCImpl::DecodeInternal(const uint8_t* encoded,
                                        size_t encoded_len,
                                        int sample_rate_hz,
                                        int16_t* decoded,
                                        SpeechType* speech_type) {
  assert(false);
  return 0;
}

}  // namespace webrtc_integration
}  // namespace dolby_voice_client
