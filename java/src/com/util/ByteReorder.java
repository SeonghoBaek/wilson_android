package com.util;

/**
 * Created by major on 11/2/15.
 */
public class ByteReorder {
    public static byte[] toCBytesArray(int value) {
        byte[] cBytes = new byte[4];

        // Bytes reordering
        cBytes[3] = (byte) ((value & 0xFF000000) >> 24);
        cBytes[2] = (byte) ((value & 0x00FF0000) >> 16);
        cBytes[1] = (byte) ((value & 0x0000FF00) >> 8);
        cBytes[0] = (byte) ((value & 0x000000FF) >> 0);

        return cBytes;
    }

    public static byte[] toJBytesArray(int value) {
        byte[] jBytes = new byte[4];

        // Bytes reordering
        jBytes[0] = (byte) ((value & 0xFF000000) >> 24);
        jBytes[1] = (byte) ((value & 0x00FF0000) >> 16);
        jBytes[2] = (byte) ((value & 0x0000FF00) >> 8);
        jBytes[3] = (byte) ((value & 0x000000FF) >> 0);

        return jBytes;
    }
}
