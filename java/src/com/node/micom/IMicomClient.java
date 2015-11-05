package com.node.micom;

/**
 * Created by major on 8/11/15.
 */
public interface IMicomClient {
    boolean  sendCommand(String command);
    boolean  connect();
    boolean  disconnect();
}
