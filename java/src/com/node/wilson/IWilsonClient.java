package com.node.wilson;

/**
 * Created by major on 10/29/15.
 */
public interface IWilsonClient {
    int connect();
    int disconnect();
    int send(String data);
}
