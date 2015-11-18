package com.node.wilson;

/**
 * Created by major on 10/29/15.
 */
public class WilsonXML {
    public static String XmlHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

    public interface BUS_EVENT_TYPE
    {
        int NBUS_TYPE_EVENT = 0; // bus event
        int NBUS_TYPE_REQUEST = 1; // command - use command id
        int NBUS_TYPE_RESPONSE = 2; // response of command - use command id
        int NBUS_TYPE_NONE = 3;
    }

    public interface BUS_COMMAND_ID // bus command id
    {
        int NBUS_CMD_JOIN = 0;
        int NBUS_CMD_DROP = 1;
        int NBUS_CMD_LIST = 2;
        int NBUS_CMD_CLIENT_BROADCAST = 3;
        int NBUS_CMD_REGISTER = 4;
        int NBUS_CMD_UNREGISTER = 5;
        int NBUS_CMD_SAVE_USB = 6;
        int NBUS_CMD_LOGCAT_USB = 7;
        int NBUS_CMD_NONE = 8;
    }

}
