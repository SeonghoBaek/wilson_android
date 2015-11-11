package com.node.wilson;

import com.node.wilson.IWilsonRemoteListener;

interface IWilsonService {
    int addListener(IWilsonRemoteListener l);
    int removeListener(IWilsonRemoteListener l);
    int send(String data);
    int register(String ip, String port);
    int saveToUSB();
}