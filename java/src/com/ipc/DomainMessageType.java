package com.ipc;

/**
 * Created by major on 11/11/15.
 */
public interface DomainMessageType {

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
    int NONE_MESSAGE = 10;
}
