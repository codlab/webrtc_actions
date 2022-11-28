package org.webrtc.codecs;

import android.media.MediaCodecInfo;
import android.os.Build;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.ArrayList;
import java.util.List;

public class CodecDescriptorHolder {
    private final static String TAG = CodecDescriptorHolder.class.getSimpleName();

    private final List<CodecDescriptor> codecDescriptors = new ArrayList<>();
    private boolean OMX_google;

    public CodecDescriptorHolder() {
        OMX_google = false;
    }

    public CodecDescriptorHolder register(@NonNull CodecDescriptor descriptor) {
        codecDescriptors.add(descriptor);

        return this;
    }

    public void activateOMXGoogle(boolean enabled) {
        OMX_google = enabled;
    }

    /**
     * Method used by WebRTC to check if a given MediaCodecInfo can be used for decoding
     *
     * @param codecInfo the codec info holder to check with
     * @return true if either hardware or set as known
     */
    public boolean isSupported(@Nullable MediaCodecInfo codecInfo) {
        return isHardwareAccelerated(codecInfo) || isKnown(codecInfo);
    }

    /**
     * Check if the codec is known from its name inside the current descriptor holder
     *
     * @param codecInfo the codec info holder to check with
     * @return true if we are sure this codec can be used due to forced state
     */
    private boolean isKnown(@Nullable MediaCodecInfo codecInfo) {
        String name = null;
        if (null != codecInfo && android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN) {
            name = codecInfo.getName();
        }

        if (null == name) return false;

        if (OMX_google && name.indexOf("OMX.google") == 0) return true;

        for (CodecDescriptor descriptor : codecDescriptors) {
            if (descriptor.isValid(name)) {
                return true;
            }
        }

        return false;
    }

    /**
     * Check if the given MediaCodecInfo is hardware accelerated when possible
     *
     * @param codecInfo the codec info holder to check with
     * @return true if we are sure hardware acceleration is possible with this codec
     */
    private boolean isHardwareAccelerated(@Nullable MediaCodecInfo codecInfo) {
        // Android Q introduced the ability to get the HW status
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            return null != codecInfo && codecInfo.isHardwareAccelerated();
        }

        // we don't know the status, we just return false, to check if it's known internally
        return false;
    }
}
