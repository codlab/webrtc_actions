// Copyright 2020 Dolby Laboratories and Dolby International AB
// Use of this source code is governed by the MIT license that can be
// found in the dolby/LICENSE file.

#ifndef WEBRTC_DVC_VOICEMEDIACHANNEL_H_
#define WEBRTC_DVC_VOICEMEDIACHANNEL_H_

#include "dvc/audiopackethandler.h"
#include "media/engine/webrtc_voice_engine.h"

#include <atomic>

namespace dolby_voice_client {
namespace webrtc_integration {

class DvcVoiceMediaChannel : public cricket::WebRtcVoiceMediaChannel {
public:
  enum Status {
    READY,
    CLOSED
  };

  DvcVoiceMediaChannel(cricket::WebRtcVoiceEngine* engine,
                       const cricket::MediaConfig& config,
                       const cricket::AudioOptions& options,
                       const webrtc::CryptoOptions& crypto_options,
                       webrtc::Call* call);
  virtual ~DvcVoiceMediaChannel();

  bool GetStats(cricket::VoiceMediaInfo* info, bool get_and_clear_legacy_stats) override;
  void OnPacketReceived(rtc::CopyOnWriteBuffer packet,
                        int64_t packet_time_us) override;

  bool SetMinimumPlayoutDelay(int delay_ms);
  uint32_t GetPlayoutDelay() const;
  AudioPacketHandler* GetAudioPacketHandler() const;
  bool GetPacketsToSend(std::vector<AudioBuffer>& packets);
  void SetInputMute(bool mute);

  void OnReadyToSend(bool ready) override;
  bool IsReadyToSend() { return ready_to_send_; }

  bool IsUsingDvcCodec() const { return GetAudioPacketHandler() != nullptr; }

  DvcVoiceMediaChannel* GetDvcVoiceMediaChannel() const override {
    return const_cast<DvcVoiceMediaChannel*>(this);
  }

  bool SetExternalPacketHandler(AudioPacketHandler* client) override;

  sigslot::signal1<Status> SignalStatusUpdate;

protected:
  mutable std::mutex mutex_;
  AudioPacketHandler* _audio_packet_handler = nullptr;
  int64_t _last_audio_session_stats_log_ms = -1;
  int64_t _last_voice_media_info_log_ms = -1;
  bool is_muted_ = false;
  uint32_t min_playout_delay_ = 0;
  std::atomic<bool> ready_to_send_{};
private:
  void SetAudioPacketHandler(AudioPacketHandler* client);
};

}  // namespace webrtc_integration
}  // namespace dolby_voice_client

#endif /* WEBRTC_DVC_VOICEMEDIACHANNEL_H_ */
