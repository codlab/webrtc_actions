package org.webrtc;

import android.os.Build;
import androidx.annotation.Nullable;
import android.util.Log;

import org.webrtc.codecs.CodecDescriptor;
import org.webrtc.codecs.CodecDescriptorHolder;

import java.util.HashMap;
import java.util.Set;

import static org.webrtc.MediaCodecUtils.EXYNOS_PREFIX;
import static org.webrtc.MediaCodecUtils.IMG_TOPAZ;
import static org.webrtc.MediaCodecUtils.QCOM_PREFIX;

public final class CodecDescriptorFactory {
    private final static String C2_ANDROID = "c2.android.";

    private static CodecDescriptorHolder decoders;
    private static HashMap<VideoCodecMimeType, CodecDescriptorHolder> encoders;

    private static void checkInit() {
        if (null != decoders && null != encoders) return;

        decoders = new CodecDescriptorHolder();
        encoders = new HashMap<>();

        decoders
                // Support H.264 HP decoding on
                .register(new CodecDescriptor(IMG_TOPAZ, Build.VERSION_CODES.KITKAT, false))
                // Support H.264 HP decoding on QCOM chips for Android L and above.
                .register(new CodecDescriptor(QCOM_PREFIX, Build.VERSION_CODES.LOLLIPOP, false))
                // Support H.264 HP decoding on Exynos chips for Android M and above.
                .register(new CodecDescriptor(EXYNOS_PREFIX, Build.VERSION_CODES.M, false))
                .register(new CodecDescriptor(C2_ANDROID, Build.VERSION_CODES.N, false));


        CodecDescriptorHolder h264 = new CodecDescriptorHolder();
        CodecDescriptorHolder vp8 = new CodecDescriptorHolder();
        CodecDescriptorHolder vp9 = new CodecDescriptorHolder();

        h264
                // QCOM H264 encoder is supported in KITKAT or later.
                .register(new CodecDescriptor(QCOM_PREFIX, Build.VERSION_CODES.KITKAT, true))
                // Topaz for Huawei H264 encoder is supported in LOLLIPOP or later.
                .register(new CodecDescriptor(IMG_TOPAZ, Build.VERSION_CODES.KITKAT, true))
                // Exynos H264 encoder is supported in LOLLIPOP or later.
                .register(new CodecDescriptor(EXYNOS_PREFIX, Build.VERSION_CODES.LOLLIPOP, true))
                .register(new CodecDescriptor(C2_ANDROID, Build.VERSION_CODES.N, true));

        vp9
                // Both QCOM and Exynos VP9 encoders are supported in N or later.
                .register(new CodecDescriptor(QCOM_PREFIX, Build.VERSION_CODES.N, true))
                .register(new CodecDescriptor(EXYNOS_PREFIX, Build.VERSION_CODES.N, true))
                .register(new CodecDescriptor(C2_ANDROID, Build.VERSION_CODES.N, true));

        vp8
                // QCOM Vp8 encoder is supported in KITKAT or later.
                .register(new CodecDescriptor(QCOM_PREFIX, Build.VERSION_CODES.KITKAT, true))
                .register(new CodecDescriptor(IMG_TOPAZ, Build.VERSION_CODES.KITKAT, true))
                // Exynos VP8 encoder is supported in M or later.
                .register(new CodecDescriptor(EXYNOS_PREFIX, Build.VERSION_CODES.M, true))
                .register(new CodecDescriptor(C2_ANDROID, Build.VERSION_CODES.N, true));

        encoders.put(VideoCodecMimeType.VP9, vp9);
        encoders.put(VideoCodecMimeType.VP8, vp8);
        encoders.put(VideoCodecMimeType.H264, h264);
    }

    private CodecDescriptorFactory() {

    }

    public static void activateOMXGoogle(boolean enabled) {
        checkInit();

        VideoCodecMimeType[] types = new VideoCodecMimeType[]{VideoCodecMimeType.VP8, VideoCodecMimeType.H264, VideoCodecMimeType.VP9};
        for (VideoCodecMimeType type : types) {
            CodecDescriptorHolder holder = encoders.get(type);
            if (null != holder) holder.activateOMXGoogle(enabled);
        }

        CodecDescriptorHolder decoders = getDecoders();
        if (null != decoders) decoders.activateOMXGoogle(enabled);
    }

    @Nullable
    public static CodecDescriptorHolder getEncoders(VideoCodecMimeType codecType) {
        checkInit();

        return encoders.get(codecType);
    }

    public static CodecDescriptorHolder getDecoders() {
        checkInit();

        return decoders;
    }
}
