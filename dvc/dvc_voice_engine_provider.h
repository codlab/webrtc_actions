
/*
 *  Copyright 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef DVC_VOICE_ENGINE_PROVIDER_
#define DVC_VOICE_ENGINE_PROVIDER_

#include "absl/memory/memory.h"
#include "api/scoped_refptr.h"

#include "media/engine/webrtc_voice_engine.h"

namespace dolby_voice_client {

namespace webrtc_integration {


class DvcVoiceEngineModuleProvider {
 public:
  
  typedef std::unique_ptr<cricket::WebRtcVoiceEngine> (* WebRtcVoiceEngineProvider)(
        webrtc::TaskQueueFactory* task_queue_factory,
        webrtc::AudioDeviceModule* adm,
        const rtc::scoped_refptr<webrtc::AudioEncoderFactory>& encoder_factory,
        const rtc::scoped_refptr<webrtc::AudioDecoderFactory>& decoder_factory,
        rtc::scoped_refptr<webrtc::AudioMixer> audio_mixer,
        rtc::scoped_refptr<webrtc::AudioProcessing> audio_processing,
        webrtc::AudioFrameProcessor* audio_frame_processor,
        const webrtc::FieldTrialsView& trials);

  static WebRtcVoiceEngineProvider provider;

  static std::unique_ptr<cricket::WebRtcVoiceEngine> Create(
      webrtc::TaskQueueFactory* task_queue_factory,
      webrtc::AudioDeviceModule* adm,
      const rtc::scoped_refptr<webrtc::AudioEncoderFactory>& encoder_factory,
      const rtc::scoped_refptr<webrtc::AudioDecoderFactory>& decoder_factory,
      rtc::scoped_refptr<webrtc::AudioMixer> audio_mixer,
      rtc::scoped_refptr<webrtc::AudioProcessing> audio_processing,
      webrtc::AudioFrameProcessor* audio_frame_processor,
      const webrtc::FieldTrialsView& trials) {
    if (provider) {
      return provider(task_queue_factory,
                      adm,
                      std::move(encoder_factory),
                      std::move(decoder_factory),
                      std::move(audio_mixer),
                      std::move(audio_processing),
                      audio_frame_processor,
                      trials);
    }
    return nullptr;
  }
};

}

}

#endif  // PC_PEER_CONNECTION_FACTORY_H_
