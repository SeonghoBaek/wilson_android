package com.nodebus;

import java.util.HashMap;

/**
 * Created by major on 11/26/15.
 */
public class NodeBusService {
    private static INodeBusService mDefaultService = null;
    private static HashMap<String, INodeBusService> mServiceList;

    public static INodeBusService getDefaultService() {

        synchronized (NodeBusService.class) {
            if (mDefaultService == null) {
                mDefaultService = new DefaultServiceImpl();
            }
        }

        return mDefaultService;
    }

    public static INodeBusService getService(String serviceName) {

        synchronized (NodeBusService.class) {
            if (mServiceList.containsKey(serviceName) == true) {
                return mServiceList.get(serviceName);
            } else {
                mServiceList.put(serviceName, new DefaultServiceImpl(serviceName));
                return mServiceList.get(serviceName);
            }
        }
    }
}
