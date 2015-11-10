package com.ipc;

import android.net.LocalServerSocket;
import android.net.LocalSocket;
import com.util.log.Loge;
import com.util.log.Logi;

import java.io.*;

/**
 * Created by major on 2015-07-28.
 */
public class DomainBridgeServer {
    private final String TAG_NAME = "DOMAIN_BRIDGE";

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
                        br = new BufferedReader(new InputStreamReader(receiver.getInputStream()));

                        String json = br.readLine();

                        br.close();
                        receiver.close();

                        Logi.println(getClass().getName(), "Received: " + json);

                        if (json.equals("stop")) {
                            server.close();
                            server = null;

                            break;
                        } else {

                            if (mCallback != null) {
                                mCallback.onMessageArrived(json);
                            }
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
