package com.ipc;

/**
 * Created by major on 7/24/15.
 */

import java.util.concurrent.ArrayBlockingQueue;
import com.util.log.*;

public class CommandQueue {

    private final int MIN_QUEUE_SIZE = 10;
    private final String TAG_NAME = "COMMAND_QUEUE";
    private ArrayBlockingQueue<Object> mQueue = null;

    public CommandQueue(int size) {

        mQueue = new ArrayBlockingQueue<Object>((size > MIN_QUEUE_SIZE) ? size : MIN_QUEUE_SIZE);
    }

    public synchronized boolean addCommand(Object cmd) {
        if (mQueue.offer(cmd) == true) {
            notifyAll();

            return true;
        }

        Logw.println(TAG_NAME, "Queue Full");

        return false;
    }

    public synchronized Object getCommand() {
        Object cmd = null;

        try {
            while ((cmd = mQueue.poll()) == null) {
                wait();
            }
        } catch (InterruptedException ie) {}

        return cmd;
    }

    public synchronized void flush() {
        mQueue.clear();
    }

}
