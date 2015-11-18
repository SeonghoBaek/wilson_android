package com.node.wilson;

/**
 * Created by major on 11/11/15.
 */
public interface WilsonMessage {

    String MSG_TYPE_REGOK = "0";
    String MSG_TYPE_REGFAIL = "1";
    String MSG_TYPE_USB_NOT_READY = "2";
    String MSG_TYPE_USB_READY = "3";
    String MSG_TYPE_USB_WRITE_FAIL = "4";
    String MSG_TYPE_USB_WRITE_DONE = "5";
    String MSG_TYPE_USB_WRITE_WAIT = "6";
    String MSG_TYPE_USB_LOGCAT_WAIT = "7";
    String MSG_TYPE_USB_LOGCAT_FAIL = "8";
    String MSG_TYPE_USB_LOGCAT_DONE = "9";
    String MSG_TYPE_NONE = "10";

}
