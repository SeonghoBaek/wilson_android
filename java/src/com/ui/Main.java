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
import com.util.log.Logi;

import java.io.*;
import java.util.ArrayList;

public class Main extends Activity {

    private boolean bRunning = true;
    private FileOutputStream fostream = null;
    private TextView tv = null;

    private WilsonClient mWilsonClient;
    private EditText mTextServerIP;
    private EditText mTextServerPort;

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

        mTextServerIP = (EditText)findViewById(R.id.textServerIP);
        mTextServerPort = (EditText)findViewById(R.id.textServerPort);

        mWilsonClient = new WilsonClient(this);

        mWilsonClient.addListener(new IWilsonListener() {
            @Override
            public void onMessageArrived(String data) {
                /*
                AlertDialog.Builder bld = new AlertDialog.Builder(Main.this);
                bld.setTitle("Wilson");
                bld.setMessage(data);
                bld.show();
                */

                Button connectBtn = (Button)findViewById(R.id.connectButton);
                connectBtn.setText(data);
            }
        });

        Button connectBtn = (Button)findViewById(R.id.connectButton);

        connectBtn.setOnClickListener(new Button.OnClickListener() {
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
