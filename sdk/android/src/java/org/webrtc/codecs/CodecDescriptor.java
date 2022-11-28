package org.webrtc.codecs;

import android.os.Build;
import androidx.annotation.NonNull;

public final class CodecDescriptor {
    private String codecPrefix;

    private int minimumAndroidVersion;

    private boolean encoder;

    private CodecDescriptor() {

    }

    public CodecDescriptor(String codecPrefix, int minimumAndroidVersion, boolean encoder) {
        this.codecPrefix = codecPrefix;
        this.minimumAndroidVersion = minimumAndroidVersion;
        this.encoder = encoder;
    }

    public String getCodecPrefix() {
        return codecPrefix;
    }

    public int getMinimumAndroidVersion() {
        return minimumAndroidVersion;
    }

    public boolean isEncoder() {
        return encoder;
    }

    public boolean isValid(@NonNull String name) {
        return name.startsWith(codecPrefix) && Build.VERSION.SDK_INT >= minimumAndroidVersion;
    }
}
