package com.node.micom.service;

import com.node.micom.service.IMicomListener;

interface IMicomService {
    String registerClient(IMicomListener client); // Return UUID String
    int  unregisterClient(IMicomListener client);
    int  sendCommand(String command);
}