package com.ipc;

import android.net.LocalServerSocket;
import android.net.LocalSocket;
import com.util.log.Loge;
import com.util.log.Logi;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.*;

/**
 * Created by major on 2015-07-28.
 */
public class DomainBridgeServer {
    private final String TAG_NAME = "DOMAIN_BRIDGE_SERVER";

    public static final int READY = 0;
    public static final int PENDING = 1;
    public static final int FAIL = -1;

    private String      mReadBridge = null;
    private Listener    mListener = null;
    private IDomainBridgeCallback   mCallback = null;

    private int     mStatus = FAIL;

    public DomainBridgeServer(IDomainBridgeCallback callback, String readBridgeName) {
        mCallback = callback;
        mReadBridge = readBridgeName;
    }

    public synchronized int getStatus() {
        return mStatus;
    }

    public synchronized boolean run() {
        if (mReadBridge == null) {
            Loge.print(TAG_NAME, "Bridge Name Null");

            return false;
        }

        mStatus = PENDING;

        mListener = new Listener();
        mListener.start();

        return true;  // mStatus is PENDING
    }

    public synchronized void stop() {
        DomainBridgeClient client = new DomainBridgeClient(mReadBridge);
        client.sendMessage("stop");
        client.stop();
    }

    private class Listener extends Thread {
        @Override
        public void run() {
            LocalServerSocket server = null;
            BufferedReader br = null;
            LocalSocket receiver = null;

            try {
                server = new LocalServerSocket(mReadBridge);

                synchronized (DomainBridgeServer.this) {
                    mStatus = READY;
                }

                while (true) {
                    receiver = server.accept();

                    if (receiver != null) {
                        InputStream is = receiver.getInputStream();

                        byte[] typeBytes = new byte[4];

                        is.read(typeBytes, 0, 4); // Read message type

                        int messageType = java.nio.ByteBuffer.wrap(typeBytes).order(java.nio.ByteOrder.LITTLE_ENDIAN).getInt();

                        Logi.println(TAG_NAME, "Message type: " + messageType);

                        byte[] lengthBytes = new byte[4];

                        is.read(lengthBytes, 0, 4); // Read message length

                        int length = java.nio.ByteBuffer.wrap(lengthBytes).order(java.nio.ByteOrder.LITTLE_ENDIAN).getInt();

                        Logi.println(TAG_NAME, "Message length: " + length);

                        byte[] buffer = new byte[length];

                        is.read(buffer, 0, length);
                        is.close();

                        receiver.close();

                        String jsonString = new String(buffer, 0, length);

                        Logi.println(TAG_NAME, "Received: " + jsonString);

                        if (mCallback != null) {
                            if (messageType == DomainMessageType.JSON_MESSAGE) // Java Server receives only JSON message type.
                                mCallback.onMessageArrived(jsonString);
                        }
                    }
                }
            } catch (IOException e) {
                synchronized (DomainBridgeServer.this) {
                    mStatus = FAIL;
                }

                Loge.print(getClass().getName() + " " + e.getMessage());
            } finally {
                try {
                    if (br != null) br.close();
                } catch (IOException ioe) {}
                finally {
                    try {
                        if (receiver != null) receiver.close();
                    } catch (IOException ioe) {}
                }
            }
        }
    }

}
