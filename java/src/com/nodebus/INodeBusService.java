package com.nodebus;

/**
 * Created by major on 11/26/15.
 */
public interface INodeBusService {

    int join(String nodeName);

    int drop(String nodeName);

    int cast(String targetNodeName, String msg, int msgType);

    int broadcast(String msg, int msgType);
}
