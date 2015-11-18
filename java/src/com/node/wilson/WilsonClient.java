package com.node.wilson;

import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import com.ipc.IDomainBridgeCallback;
import com.util.log.Logd;
import com.util.log.Loge;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by major on 10/29/15.
 */
public class WilsonClient extends IWilsonRemoteListener.Stub implements IWilsonClient {

    private final String TAG_NAME = "WilsonClient";
    private Context mContext;
    private Boolean mConnected = false;
    private IWilsonService mBinder = null;
    private IWilsonListener mListener;
    private String mServerIP = null;
    private String mServerPort = null;

    private ServiceConnection mServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Logd.println(TAG_NAME, "onServiceConnected");
            WilsonClient.this.mBinder = IWilsonService.Stub.asInterface(service);

            try {
                WilsonClient.this.mBinder.addListener(WilsonClient.this);

                synchronized (WilsonClient.class) {
                    mConnected = true;
                }

            } catch (RemoteException re) {
                Loge.println(TAG_NAME, re.toString());
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            Logd.println(TAG_NAME, "onServiceDisconnected");

            synchronized (WilsonClient.class) {
                WilsonClient.this.mBinder = null;
                mConnected = false;
            }
        }
    };

    public WilsonClient(Context context) {
        this.mContext = context;

        this.connect();
    }

    public void onReceived(String data) {
        // IWilsonRemoteListener Callback
        if (this.mListener != null) {
            this.mListener.onMessageArrived(data);
        }
    }

    @Override
    public int connect() {
        // IWilsonClient
        if (this.mConnected == true) {
            Logd.println(TAG_NAME, "Already Connected");

            return 0;
        }

        Intent intent = new Intent(this.mContext, com.node.wilson.WilsonService.class);

        Logd.println(TAG_NAME, "Service Binding...");
        if (this.mContext.bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE) == false) {
            Loge.println(TAG_NAME, "Bind Failure");

            return -1;
        } else {
            Logd.println(TAG_NAME, "Bind OK");
        }

        return 0;
    }

    @Override
    public int connect(String ip, String port) {
        // IWilsonClient
        if (this.mConnected == true) {
            Logd.println(TAG_NAME, "Already Connected");

            return 0;
        }

        mServerIP = ip;
        mServerPort = port;

        Intent intent = new Intent(this.mContext, com.node.wilson.WilsonService.class);

        Logd.println(TAG_NAME, "Service Binding...");
        if (this.mContext.bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE) == false) {
            Loge.println(TAG_NAME, "Bind Failure");

            return -1;
        } else {
            Logd.println(TAG_NAME, "Bind OK");
        }

        return 0;
    }

    @Override
    public int register(String ip, String port) {
        mServerIP = ip;
        mServerPort = port;

        try {
            WilsonClient.this.mBinder.register(mServerIP, mServerPort);

            if (mListener != null) {
                JSONObject jsonObject = new JSONObject();

                try {
                    jsonObject.put("id", WilsonMessage.MSG_TYPE_REGOK);
                    jsonObject.put("text", "Wilson Server Connected");

                    mListener.onMessageArrived(jsonObject.toString());
                } catch (JSONException je) {
                    Loge.println(TAG_NAME, je.toString());
                }
            }
        } catch (RemoteException re) {
            Loge.println(TAG_NAME, re.toString());
        }

        return 0;
    }

    @Override
    public int disconnect() {
        // IWilsonClient
        try {
            this.mBinder.removeListener(this);
        } catch (RemoteException re) {
            re.printStackTrace();
        }

        return 0;
    }

    @Override
    public int send(String data) {
        // IWilsonClient
        synchronized (WilsonClient.class) {
            if ( this.mConnected == false ) return -1;
        }

        return 0;
    }

    @Override
    public int saveToUSB() {
        try {
            this.mBinder.saveToUSB();
        } catch (RemoteException re) {
            re.printStackTrace();
        }

        return 0;
    }

    @Override
    public int logcatToUSB() {
        try {
            this.mBinder.logcatToUSB();
        } catch (RemoteException re) {
            re.printStackTrace();
        }

        return 0;
    }

    public void addListener(IWilsonListener listener) {
        this.mListener = listener;
    }
}
