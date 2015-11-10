package com.node.wilson;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.IBinder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.widget.Toast;
import com.ipc.CommandQueue;
import com.ipc.DomainBridgeClient;
import com.ipc.DomainBridgeServer;
import com.ipc.IDomainBridgeCallback;
import com.util.ByteReorder;
import com.util.log.Logd;
import com.util.log.Loge;
import com.util.log.Logi;

/**
 * Created by major on 10/29/15.
 */
public class WilsonService extends Service {

    private final String TAG_NAME = "WilsonService";
    private final String NODEBUS_SERVER = "defaultservice.service.local";
    private final String NODE_NAME = "node.wilson";

    private DomainBridgeClient mBridgeClient;
    private DomainBridgeServer mBridgeServer;
    private final RemoteCallbackList<IWilsonRemoteListener> mRemoteCallbackList = new RemoteCallbackList<IWilsonRemoteListener>();

    private final int MAX_QUEUE_SIZE = 1024;
    private CommandQueue mQueue = new CommandQueue(MAX_QUEUE_SIZE);

    private IWilsonService.Stub mBinder = new IWilsonService.Stub() {
        @Override
        public int register(String ip, String port) throws RemoteException {
            String registerXml = WilsonXML.XmlHeader;

            String serverIP = ip;
            String serverPort = port;

            registerXml += "<nodebus type=\"" + WilsonXML.BUS_EVENT_TYPE.NBUS_TYPE_REQUEST + "\" id=\"" + WilsonXML.BUS_COMMAND_ID.NBUS_CMD_REGISTER +
                    "\" ip=\"" + serverIP + "\" port=\"" + serverPort + "\"/>";

            Logi.println(TAG_NAME, "Register: " + registerXml);

            byte[] typeBytes = ByteReorder.toCBytesArray(WilsonXML.BUS_EVENT_TYPE.NBUS_TYPE_REQUEST);
            byte[] registerXMLBytes = registerXml.getBytes();
            byte[] lengthBytes = ByteReorder.toCBytesArray(registerXMLBytes.length);
            byte[] byteXML = new byte[typeBytes.length + lengthBytes.length + registerXml.length()];

            System.arraycopy(typeBytes, 0, byteXML, 0, typeBytes.length);
            System.arraycopy(lengthBytes, 0, byteXML, typeBytes.length, lengthBytes.length);
            System.arraycopy(registerXMLBytes, 0, byteXML, typeBytes.length + lengthBytes.length, registerXMLBytes.length);

            mBridgeClient.sendMessage(byteXML);

            return 0;
        }

        @Override
        public int addListener(IWilsonRemoteListener l) throws RemoteException {
            if (l != null) mRemoteCallbackList.register(l);

            return 0;
        }

        @Override
        public int removeListener(IWilsonRemoteListener l) throws RemoteException {
            if (l != null) mRemoteCallbackList.unregister(l);

            return 0;
        }

        @Override
        public int send(String data) throws RemoteException {
            return 0;
        }

        @Override
        public int saveToUSB() throws RemoteException {
            String registerXml = WilsonXML.XmlHeader;

            registerXml += "<nodebus type=\"" + WilsonXML.BUS_EVENT_TYPE.NBUS_TYPE_REQUEST + "\" id=\"" + WilsonXML.BUS_COMMAND_ID.NBUS_CMD_SAVE_USB + "\"/>";

            Logi.println(TAG_NAME, "Save To USB: " + registerXml);

            byte[] typeBytes = ByteReorder.toCBytesArray(WilsonXML.BUS_EVENT_TYPE.NBUS_TYPE_REQUEST);
            byte[] registerXMLBytes = registerXml.getBytes();
            byte[] lengthBytes = ByteReorder.toCBytesArray(registerXMLBytes.length);
            byte[] byteXML = new byte[typeBytes.length + lengthBytes.length + registerXml.length()];

            System.arraycopy(typeBytes, 0, byteXML, 0, typeBytes.length);
            System.arraycopy(lengthBytes, 0, byteXML, typeBytes.length, lengthBytes.length);
            System.arraycopy(registerXMLBytes, 0, byteXML, typeBytes.length + lengthBytes.length, registerXMLBytes.length);

            mBridgeClient.sendMessage(byteXML);

            return 0;
        }
    };

    public IBinder onBind(Intent intent) {

        return mBinder;
    }

    @Override
    public void onCreate() {
        mBridgeClient = new DomainBridgeClient(NODEBUS_SERVER);
        mBridgeServer = new DomainBridgeServer(new NodeBusListener(), NODE_NAME);

        mBridgeServer.run();

        new NodeBusDispatcher().start();

        String joinXml = WilsonXML.XmlHeader;

        joinXml += "<nodebus type=\"" + WilsonXML.BUS_EVENT_TYPE.NBUS_TYPE_REQUEST + "\" id=\"" + WilsonXML.BUS_COMMAND_ID.NBUS_CMD_JOIN + "\" node=\"" + NODE_NAME + "\"/>";

        Logi.println(TAG_NAME, "Join: " + joinXml);

        byte[] typeBytes = ByteReorder.toCBytesArray(WilsonXML.BUS_EVENT_TYPE.NBUS_TYPE_REQUEST);
        byte[] joinXMLBytes = joinXml.getBytes();
        byte[] lengthBytes = ByteReorder.toCBytesArray(joinXMLBytes.length);

        byte[] byteXML = new byte[typeBytes.length + lengthBytes.length + joinXMLBytes.length];

        System.arraycopy(typeBytes, 0, byteXML, 0, typeBytes.length);
        System.arraycopy(lengthBytes, 0, byteXML, typeBytes.length, lengthBytes.length);
        System.arraycopy(joinXMLBytes, 0, byteXML, typeBytes.length + lengthBytes.length, joinXMLBytes.length);

        mBridgeClient.sendMessage(byteXML);

        mQueue.addCommand("Connected");

        /*
        String registerXml = WilsonXML.XmlHeader;

        String ip = "10.0.0.4";
        String port = "8088";

        registerXml += "<nodebus type=\"" + WilsonXML.BUS_EVENT_TYPE.NBUS_TYPE_REQUEST + "\" id=\"" + WilsonXML.BUS_COMMAND_ID.NBUS_CMD_REGISTER +
                "\" ip=\"" + ip + "\" port=\"" + port + "\"/>";

        Logi.println(TAG_NAME, "Register: " + registerXml);

        byte[] registerXMLBytes = registerXml.getBytes();

        lengthBytes = ByteReorder.toCBytesArray(registerXMLBytes.length);

        byteXML = new byte[typeBytes.length + lengthBytes.length + registerXml.length()];

        System.arraycopy(typeBytes, 0, byteXML, 0, typeBytes.length);
        System.arraycopy(lengthBytes, 0, byteXML, typeBytes.length, lengthBytes.length);
        System.arraycopy(registerXMLBytes, 0, byteXML, typeBytes.length + lengthBytes.length, registerXMLBytes.length);

        mBridgeClient.sendMessage(byteXML);
        */

        super.onCreate();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Logd.println(TAG_NAME, "onStartCommand");
        return super.onStartCommand(intent, flags, startId);
    }

    private class NodeBusDispatcher extends Thread {
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

    private class NodeBusListener implements IDomainBridgeCallback {
        @Override
        public void onMessageArrived(Object msg) {
            mQueue.addCommand(msg);
        }
    }
}
