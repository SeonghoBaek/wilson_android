package com.node.micom.service;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import com.ipc.CommandQueue;
import com.ipc.DomainBridgeClient;
import com.ipc.DomainBridgeServer;
import com.ipc.IDomainBridgeCallback;
import com.util.log.Logd;
import com.util.log.Loge;
import com.util.log.Logi;

import java.util.UUID;

/**
 * Created by major on 7/27/15.
 */
public class MicomService extends Service {
    private static final String TAG_NAME = "MICOM_SERVICE";
    public static final String INTENT_ACTION = "com.node.micom.service.MicomService";

    public static final String MICOM_SERVICE_SERVER = "micom_svc_server";
    public static final String MICOM_SERVICE_CLIENT = "micom_svc_client";  // Need to be shared by native daemon

    private final int MAX_QUEUE_SIZE = 1024;

    private CommandQueue mQueue = new CommandQueue(MAX_QUEUE_SIZE);

    private DomainBridgeServer mMicomDaemonReceiver = null;
    private DomainBridgeClient mMicomDaemonSender = null;

    private final RemoteCallbackList<IMicomListener> mRemoteCallbackList = new RemoteCallbackList<IMicomListener>();

    private final IMicomService.Stub mBinder = new IMicomService.Stub() {
        @Override
        public String registerClient(IMicomListener client) throws RemoteException {

            // To do: Check collision
            mRemoteCallbackList.register(client);
            Logd.println(TAG_NAME, "Register client");

            return UUID.randomUUID().toString();
        }

        @Override
        public int unregisterClient(IMicomListener client) throws RemoteException {

            // To do: check non exist client
            mRemoteCallbackList.unregister(client);
            Logd.println(TAG_NAME, "Unregister client");

            return 0;
        }

        @Override
        public int sendCommand(String command) throws RemoteException {

            Logd.println(TAG_NAME, "Send command: " + command);

            // To do: Create micom protocol JSON string
            mMicomDaemonSender.sendMessage(command);

            return 0;
        }
    };

    private class MicomDispatcher extends Thread {
        public void run() {
            while (true) {
                Object command = mQueue.getCommand();

                String cmd = (String) command;

                if (cmd.equals("exit")) {
                    return;
                }

                int numOfListener = mRemoteCallbackList.beginBroadcast();

                try {
                    for (int i = 0; i < numOfListener; i++) {
                        mRemoteCallbackList.getBroadcastItem(i).onReceived((String) command);
                    }
                } catch (RemoteException re) {
                    Loge.println(TAG_NAME, re.toString());
                } finally {
                    mRemoteCallbackList.finishBroadcast();
                }
            }
        }
    }

    private class DaemonListener implements IDomainBridgeCallback {
        @Override
        public void onMessageArrived(Object msg) {
            mQueue.addCommand(msg);
        }
    }

    /*
    private class MicomDeamonTester extends Thread {
        public void run() {
            int i = 0;

            while (i < 100) {
                if ( mQueue.addCommand("Hello From MICOM " + i) == true ) i++;

                try {
                    Thread.sleep(100);
                } catch (InterruptedException ie) {}
            }
        }
    }
    */

    @Override
    public IBinder onBind(Intent intent) {
        Logi.println(TAG_NAME, "onBind:");

        return mBinder;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Logd.println(TAG_NAME, "onStartCommand");
        return super.onStartCommand(intent, flags, startId);
    }

    private boolean init() {
        Logd.println(TAG_NAME, "Micom Service Init");
        mMicomDaemonReceiver = new DomainBridgeServer(new DaemonListener(), MICOM_SERVICE_SERVER);

        mMicomDaemonReceiver.run();

        // Need to check if the micom daemon ready.
        mMicomDaemonSender = new DomainBridgeClient(MICOM_SERVICE_CLIENT);

        new MicomDispatcher().start();

        return true;
    }

    @Override
    public void onDestroy() {
        Logd.println(TAG_NAME, "onDestroy");

        mQueue.flush();
        mQueue.addCommand("exit");

        if (mMicomDaemonReceiver != null) {
            mMicomDaemonReceiver.stop();
        }

        if (mMicomDaemonSender != null) {
            mMicomDaemonSender.stop();
        }

        super.onDestroy();
    }

    @Override
    public void onCreate() {
        init(); // Can be fail. Race condition with micom daemon: If this case, move init() to onBind.

        Logd.println(TAG_NAME, "onCreate");

        //new MicomDeamonTester().start();

        super.onCreate();
    }

    public MicomService() {}
}
