package com.node.micom;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import com.node.micom.service.IMicomListener;
import com.node.micom.service.IMicomService;
import com.util.log.Logd;
import com.util.log.Loge;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.UUID;

/**
 * Created by major on 8/11/15.
 */
public class MicomClient extends IMicomListener.Stub implements IMicomClient {
    private String          TAG_NAME = MicomClient.class.getName();
    private Context         mContext = null;
    private IMicomService   mBinder = null;
    private UUID            mUUID = null;
    private boolean         mConnected = false;
    private MicomListener   mListener = null;

    private ServiceConnection mServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Logd.println(TAG_NAME, "onServiceConnected");
            MicomClient.this.mBinder = IMicomService.Stub.asInterface(service);

            try {
                String uuidString = MicomClient.this.mBinder.registerClient(MicomClient.this);

                Logd.println(TAG_NAME, "Client UUID: " + uuidString);
                mUUID = UUID.fromString(uuidString);
                mConnected = true;
            } catch (RemoteException re) {
                Loge.println(TAG_NAME, re.toString());
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            Logd.println(TAG_NAME, "onServiceDisconnected");
            MicomClient.this.mBinder = null;
            mConnected = false;
        }
    };

    public MicomClient(Context context) {
        this.mContext = context;
    }

    @Override
    public void onReceived(String data) throws RemoteException {
        if (this.mListener != null) {
            this.mListener.onMessageArrived(data);
        }
    }

    @Override
    public synchronized boolean connect() {
        if (this.mConnected == true) {
            Logd.println(TAG_NAME, "Already Connected");

            return true;
        }

        Intent intent = new Intent(this.mContext, com.node.micom.service.MicomService.class);

        Logd.println(TAG_NAME, "Service Binding...");
        if (this.mContext.bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE) == false) {
            Loge.println(TAG_NAME, "Bind Failure");
        } else {
            Logd.println(TAG_NAME, "Bind OK");
        }

        return this.mConnected;
    }

    @Override
    public synchronized boolean disconnect() {
        if (this.mConnected == false) {
            Logd.println(TAG_NAME, "Already Disconnected");

            return true;
        }

        try {
            this.mBinder.unregisterClient(this);
            this.mContext.unbindService(mServiceConnection);
            this.mUUID = null;
            this.mConnected = false;
            this.mBinder = null;
        } catch (RemoteException re) {
            Loge.println(TAG_NAME, re.toString());
        }

        return !(this.mConnected);
    }

    @Override
    public synchronized boolean sendCommand(String command) {
        if (this.mBinder == null) {
            Loge.println(TAG_NAME, "Connect First");

            return false;
        }

        try {
            /*
              Command JSON Syntax
               {
                    "uuid" : "uuid_string_data",
                    "command" : "predefined_micom_command_strign"
               }
             */
            JSONObject jsonObject = new JSONObject();

            jsonObject.put("uuid", mUUID.toString());
            jsonObject.put("command", command);

            this.mBinder.sendCommand(jsonObject.toString()); // MicomCommand
        } catch (RemoteException re) {
            Loge.println(TAG_NAME, re.toString());

            return false;
        } catch (JSONException je) {
            Loge.println(TAG_NAME, je.toString());

            return false;
        }

        return true;
    }

    public void addMicomListener(MicomListener listener) {
        this.mListener = listener;
    }
}
