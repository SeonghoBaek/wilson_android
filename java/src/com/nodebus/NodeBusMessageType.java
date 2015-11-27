package com.nodebus;

/**
 * Created by major on 11/26/15.
 */
public interface NodeBusMessageType {
    int GLOBAL_MESSAGE = 0;
    int LOCAL_MESSAGE = 1;
    int CLIENT_MESSAGE = 2;
    int KILL_MESSAGE = 3;
    int SETUP_MESSAGE = 4;
    int BC_LOCAL_MESSAGE = 5;
    int BC_CLIENT_MESSAGE = 6;
    int BUS_MESSAGE = 7;
    int DB_MESSAGE = 8;
    int CUSTOM_MESSAGE = 9;
    int JSON_MESSAGE = 10;
    int BC_JSON_MESSAGE = 11;
    int MICOM_MESSAGE = 12;
    int BC_MICOM_MESSAGE = 13;
    int MICOM_REQUEST_MESSAGE = 14;
    int NONE_MESSAGE = 15;
}
