package com.nodebus;

import com.node.wilson.WilsonXML;
import com.util.ByteReorder;
import com.util.log.Logi;
import org.w3c.dom.Node;

/**
 * Created by major on 11/26/15.
 */
public class DefaultServiceImpl implements INodeBusService {
    private final String TAG_NAME = "com.nodebus.DefaultServiceImpl";
    private String mServiceName = null;

    public DefaultServiceImpl() {
        this.mServiceName = "defaultservice.service.local";
    }

    public DefaultServiceImpl(String seviceName) {
        this.mServiceName = mServiceName;
    }

    @Override
    public int join(String nodeName) {
        String joinXml = NodeBusXML.XmlHeader;

        joinXml += "<nodebus type=\"" + NodeBusXML.EventType.NBUS_TYPE_REQUEST + "\" id=\"" +
                    NodeBusXML.CommandID.NBUS_CMD_JOIN + "\" node=\"" + nodeName + "\"/>";

        Logi.println(TAG_NAME, "Join: " + joinXml);

        byte[] typeBytes = ByteReorder.toCBytesArray(NodeBusMessageType.LOCAL_MESSAGE);
        byte[] joinXMLBytes = joinXml.getBytes();
        byte[] lengthBytes = ByteReorder.toCBytesArray(joinXMLBytes.length);

        byte[] byteXML = new byte[typeBytes.length + lengthBytes.length + joinXMLBytes.length];

        System.arraycopy(typeBytes, 0, byteXML, 0, typeBytes.length);
        System.arraycopy(lengthBytes, 0, byteXML, typeBytes.length, lengthBytes.length);
        System.arraycopy(joinXMLBytes, 0, byteXML, typeBytes.length + lengthBytes.length, joinXMLBytes.length);

        NodeNetwork.sendMessage(mServiceName, byteXML);

        return 0;
    }

    @Override
    public int drop(String nodeName) {
        String joinXml = WilsonXML.XmlHeader;

        joinXml += "<nodebus type=\"" + NodeBusXML.EventType.NBUS_TYPE_REQUEST + "\" id=\"" +
                NodeBusXML.CommandID.NBUS_CMD_DROP + "\" node=\"" + nodeName + "\"/>";

        Logi.println(TAG_NAME, "Drop: " + joinXml);

        byte[] typeBytes = ByteReorder.toCBytesArray(NodeBusMessageType.LOCAL_MESSAGE);
        byte[] joinXMLBytes = joinXml.getBytes();
        byte[] lengthBytes = ByteReorder.toCBytesArray(joinXMLBytes.length);

        byte[] byteXML = new byte[typeBytes.length + lengthBytes.length + joinXMLBytes.length];

        System.arraycopy(typeBytes, 0, byteXML, 0, typeBytes.length);
        System.arraycopy(lengthBytes, 0, byteXML, typeBytes.length, lengthBytes.length);
        System.arraycopy(joinXMLBytes, 0, byteXML, typeBytes.length + lengthBytes.length, joinXMLBytes.length);

        NodeNetwork.sendMessage(mServiceName, byteXML);

        return 0;
    }

    @Override
    public int cast(String targetNodeName, String msg, int msgType) {

        Logi.println(TAG_NAME, "cast: msg, type: " + msgType);

        byte[] typeBytes = ByteReorder.toCBytesArray(msgType);
        byte[] msgBytes = msg.getBytes();
        byte[] lengthBytes = ByteReorder.toCBytesArray(msgBytes.length);

        byte[] packet = new byte[typeBytes.length + lengthBytes.length + msgBytes.length];

        System.arraycopy(typeBytes, 0, packet, 0, typeBytes.length);
        System.arraycopy(lengthBytes, 0, packet, typeBytes.length, lengthBytes.length);
        System.arraycopy(msgBytes, 0, packet, typeBytes.length + lengthBytes.length, msgBytes.length);

        NodeNetwork.sendMessage(mServiceName, packet);

        return 0;
    }

    @Override
    public int broadcast(String msg, int msgType) {
        cast(mServiceName, msg, msgType);
        return 0;
    }
}
