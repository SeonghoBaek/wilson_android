package com.ui;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import com.node.R;
import com.node.wilson.IWilsonListener;
import com.node.wilson.WilsonClient;
import com.node.wilson.WilsonMessage;
import com.util.log.Loge;
import com.util.log.Logi;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.*;
import java.util.ArrayList;

public class Main extends Activity {
    private final String TAG_NAME = "Wilson.Ui.Main";
    private boolean bRunning = true;
    private FileOutputStream fostream = null;
    private TextView tv = null;

    private WilsonClient mWilsonClient;
    private EditText mTextServerIP;
    private EditText mTextServerPort;
    private Button mConnectBtn;

    private final int WHAT_REGISTER = 0;
    private final int WHAT_SAVED = 1;

    private Handler mHandler = new Handler() {
      public void handleMessage(Message msg) {
          String jsonData = (String)msg.obj;

          String id = "";
          String text = "";

          try {
              JSONObject jsonObject = new JSONObject(jsonData);
              id = jsonObject.getString("id");
              text = jsonObject.getString("text");
          } catch (JSONException je) {
              Loge.println(TAG_NAME, je.toString());
          }

          Toast.makeText(Main.this, text, Toast.LENGTH_SHORT).show();

          if (id.equals(WilsonMessage.MSG_TYPE_USB_NOT_READY)) {

          } else if (id.equals(WilsonMessage.MSG_TYPE_USB_WRITE_DONE)) {

          } else if (id.equals(WilsonMessage.MSG_TYPE_REGOK)) {
            mConnectBtn.setText("Wilson Server Registered");
          }
      }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mConnectBtn = (Button)findViewById(R.id.connectButton);

        mTextServerIP = (EditText)findViewById(R.id.textServerIP);
        mTextServerPort = (EditText)findViewById(R.id.textServerPort);

        mWilsonClient = new WilsonClient(this);

        mWilsonClient.addListener(new IWilsonListener() {
            @Override
            public void onMessageArrived(String data) {
                Logi.println("Wilson Main Activity", "Received: " + data);

                Message msg = new Message();
                msg.obj = data;

                mHandler.sendMessage(msg);
            }
        });

        mConnectBtn.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                ConnectivityManager manager = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);

                NetworkInfo wifi = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);

                if ( wifi.isConnected() ) {
                    Logi.println("Wilson Main Activity", "WiFi: " + wifi.getTypeName() + " connected.");
                    mWilsonClient.connect(mTextServerIP.getText().toString(), mTextServerPort.getText().toString());
                } else{
                    Toast.makeText(Main.this, "No WiFi Network Connection.", Toast.LENGTH_SHORT).show();
                }
            }
        });

        Button saveBtn = (Button)findViewById(R.id.btnSave);

        saveBtn.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                mWilsonClient.saveToUSB();
            }
        });
    }

}
