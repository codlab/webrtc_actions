// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef DVC_API_AUDIO_CODECS_DVCAUDIOCODECFACTORIES_H_
#define DVC_API_AUDIO_CODECS_DVCAUDIOCODECFACTORIES_H_

#include "api/audio_codecs/audio_decoder_factory.h"
#include "api/audio_codecs/audio_encoder_factory.h"
#include "api/scoped_refptr.h"

namespace dolby_voice_client {
namespace webrtc_integration {

rtc::scoped_refptr<webrtc::AudioEncoderFactory> CreateDVCAudioEncoderFactory();
rtc::scoped_refptr<webrtc::AudioDecoderFactory> CreateDVCAudioDecoderFactory();

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif  // DVC_API_AUDIO_CODECS_DVCAUDIOCODECFACTORIES_H_
