package com.ipc;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.UUID;

/**
 * Created by major on 7/28/15.
 */
public class IPCCommand {
    public static final String IPC_KEY_UUID = "_uuid";
    public static final String IPC_KEY_CMD = "_cmd";
    public static final String IPC_KEY_DATA = "_data";

    private JSONObject mJsonObject = null;


    public IPCCommand() {
        mJsonObject = new JSONObject();
    }

    public void setUUID(UUID uuid) throws JSONException {
        mJsonObject.put(IPC_KEY_UUID, uuid.toString());
    }

    public UUID getUUID() throws JSONException {
        String uuidString = (String)mJsonObject.get(IPC_KEY_UUID);
        return UUID.fromString(uuidString);
    }

    public void setCommand(String cmd) throws JSONException {
        mJsonObject.put(IPC_KEY_CMD, cmd);
    }

    public String getCommand() throws JSONException {
        return (String)mJsonObject.get(IPC_KEY_CMD);
    }

    public void setData(String data) throws JSONException {
        mJsonObject.put(IPC_KEY_DATA, data);
    }

    public String getData() throws JSONException {
        return (String)mJsonObject.get(IPC_KEY_DATA);
    }
}
