package com.ipc;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.UUID;

/**
 * Created by major on 2015-07-28.
 */
public class DomainBridgeCommand {
    public static final String DBC_KEY_UUID = "_uuid";
    public static final String DBC_KEY_CMD = "_cmd";
    public static final String DBC_KEY_DATA = "_data";

    private JSONObject  mJsonObject = null;


    public DomainBridgeCommand() {
        mJsonObject = new JSONObject();
    }

    public void setUUID(UUID uuid) throws JSONException {
        mJsonObject.put(DBC_KEY_UUID, uuid.toString());
    }

    public UUID getUUID() throws JSONException {
        String uuidString = (String)mJsonObject.get(DBC_KEY_UUID);
        return UUID.fromString(uuidString);
    }

    public void setCommand(String cmd) throws JSONException {
        mJsonObject.put(DBC_KEY_CMD, cmd);
    }

    public String getCommand() throws JSONException {
        return (String)mJsonObject.get(DBC_KEY_CMD);
    }

    public void setData(String data) throws JSONException {
        mJsonObject.put(DBC_KEY_DATA, data);
    }

    public String getData() throws JSONException {
        return (String)mJsonObject.get(DBC_KEY_DATA);
    }
}
