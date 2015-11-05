package com.util.log;

import android.util.Log;

/**
 * Created by major on 7/24/15.
 */
public class Logv {

    public static void print(String log) {
        print(null, log);
    }

    public static void println(String log) {
        println(null, log);
    }

    public static void print(String tag, String log) {
        Log.v(tag, log);
    }

    public static void println(String tag, String log) {
        print(tag, log + "\n");
    }

}
