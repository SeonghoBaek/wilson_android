package com.nodebus;

import android.net.LocalSocket;
import android.net.LocalSocketAddress;
import com.ipc.DomainMessageType;
import com.util.ByteReorder;
import com.util.log.Loge;

import java.io.IOException;
import java.io.OutputStream;

/**
 * Created by major on 11/26/15.
 */
public class NodeNetwork {
    private static final String TAG_NAME = "com.nodebus.NodeNetwork";

    private static boolean isFilesystemSocket(String nodeName) {
        return nodeName.startsWith("/");
    }

    public static boolean sendMessage(String nodeName, byte[] msg) {
        LocalSocketAddress.Namespace namespace;

        if (isFilesystemSocket(nodeName)) {
            namespace = LocalSocketAddress.Namespace.FILESYSTEM;
        } else {
            namespace = LocalSocketAddress.Namespace.ABSTRACT;
        }

        LocalSocketAddress mAddress = new LocalSocketAddress(nodeName, namespace);

        try {
            LocalSocket localSocket = new LocalSocket();

            localSocket.connect(mAddress);

            OutputStream outputStream = localSocket.getOutputStream();

            outputStream.write(msg);

            outputStream.close();
        } catch (IOException ioe) {
            Loge.println(TAG_NAME, ioe.toString());

            return false;
        }

        return true;
    }
}
