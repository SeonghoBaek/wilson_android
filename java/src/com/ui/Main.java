package com.ui;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import com.node.R;
import com.node.wilson.IWilsonListener;
import com.node.wilson.WilsonClient;

import java.io.*;
import java.util.ArrayList;

public class Main extends Activity {

    private boolean bRunning = true;
    private FileOutputStream fostream = null;
    private TextView tv = null;

    private ArrayList<String> mLogCache = new ArrayList();

    private WilsonClient mWilsonClient;

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

        mWilsonClient = new WilsonClient(this);

        mWilsonClient.addListener(new IWilsonListener() {
            @Override
            public void onMessageArrived(String data) {
                AlertDialog.Builder bld = new AlertDialog.Builder(Main.this);
                bld.setTitle("Wilson");
                bld.setMessage(data);
                bld.show();
            }
        });

        Button connectBtn = (Button)findViewById(R.id.connectButton);

        connectBtn.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                mWilsonClient.connect();
            }
        });

        Button sendBtn = (Button)findViewById(R.id.sendButton);

        sendBtn.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {

            }
        });

        Button disconnectBtn = (Button)findViewById(R.id.disconnectButton);

        disconnectBtn.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                mWilsonClient.disconnect();
            }
        });
    }

}
