package org.webrtc.audio;

public class WebRtcOverrideRecordThread {

    public static void activateMicrophone(boolean state) {
        WebRtcAudioRecord.activated = state;
        org.webrtc.voiceengine.WebRtcAudioRecord.activated = state;
    }
}
