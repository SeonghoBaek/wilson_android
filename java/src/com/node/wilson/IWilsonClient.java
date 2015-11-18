package com.node.wilson;

/**
 * Created by major on 10/29/15.
 */
public interface IWilsonClient {
    int connect();
    int connect(String ip, String port);
    int register(String ip, String port);
    int disconnect();
    int send(String data);
    int saveToUSB();
    int logcatToUSB();
}
