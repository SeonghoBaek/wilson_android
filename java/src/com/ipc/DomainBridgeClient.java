package com.ipc;

import android.net.LocalSocket;
import android.net.LocalSocketAddress;
import com.util.ByteReorder;
import com.util.log.Loge;
import com.util.log.Logi;
import org.json.JSONObject;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;

/**
 * Created by major on 2015-07-28.
 */
public class DomainBridgeClient {
    private static String TAG_NAME = "DOMAIN_BRIDGE_CLIENT";

    private String mWriteBridge = null;
    private BufferedWriter mServerWriter = null;
    private LocalSocketAddress mAddress = null;
    private LocalSocket mSocket = null;

    private boolean isFilesystemSocket(String name) {
        return name.startsWith("/");
    }

    public DomainBridgeClient(String targetBridgeName) {

        mWriteBridge = targetBridgeName;

        mSocket = new LocalSocket();

        LocalSocketAddress.Namespace namespace;

        if (isFilesystemSocket(mWriteBridge)) {
            namespace = LocalSocketAddress.Namespace.FILESYSTEM;
        } else {
            namespace = LocalSocketAddress.Namespace.ABSTRACT;
        }

        mAddress = new LocalSocketAddress(mWriteBridge, namespace);
    }

    public synchronized void stop() {
        if (mServerWriter != null) {
            try {
                mServerWriter.close();
            } catch (IOException ioe) {}
        }

        if (mSocket != null) {
            try {
                mSocket.close();
            } catch (Exception e) {}
        }

        mSocket = null;
        mAddress = null;
        mServerWriter = null;
    }

    public synchronized boolean sendJSONObject(JSONObject obj) {
        return sendMessage(obj.toString());
    }

    public synchronized boolean sendMessage(byte[] msg) {

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

    public synchronized boolean sendMessage(String msg) {
        try {
            LocalSocket localSocket = new LocalSocket();

            localSocket.connect(mAddress);

            OutputStream outputStream = localSocket.getOutputStream();

            int type = 1;
            int size = msg.getBytes().length;

            byte[] typeByte = ByteReorder.toCBytesArray(type);

            //Byte.
            outputStream.write(typeByte);

            byte[] sizeByte = ByteReorder.toCBytesArray(size);

            outputStream.write(sizeByte);

            outputStream.write(msg.getBytes());

            outputStream.close();
        } catch (IOException ioe) {
            Loge.println(TAG_NAME, ioe.toString());

            return false;
        }

        return true;
    }
}
