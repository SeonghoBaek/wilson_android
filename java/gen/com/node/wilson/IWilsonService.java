/*___Generated_by_IDEA___*/

/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: /mnt/Wilson/major/android/android_avn_kitkat/external/wilson_android/java/src/com/node/wilson/IWilsonService.aidl
 */
package com.node.wilson;
public interface IWilsonService extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.node.wilson.IWilsonService
{
private static final java.lang.String DESCRIPTOR = "com.node.wilson.IWilsonService";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an com.node.wilson.IWilsonService interface,
 * generating a proxy if needed.
 */
public static com.node.wilson.IWilsonService asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.node.wilson.IWilsonService))) {
return ((com.node.wilson.IWilsonService)iin);
}
return new com.node.wilson.IWilsonService.Stub.Proxy(obj);
}
@Override public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_addListener:
{
data.enforceInterface(DESCRIPTOR);
com.node.wilson.IWilsonRemoteListener _arg0;
_arg0 = com.node.wilson.IWilsonRemoteListener.Stub.asInterface(data.readStrongBinder());
int _result = this.addListener(_arg0);
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_removeListener:
{
data.enforceInterface(DESCRIPTOR);
com.node.wilson.IWilsonRemoteListener _arg0;
_arg0 = com.node.wilson.IWilsonRemoteListener.Stub.asInterface(data.readStrongBinder());
int _result = this.removeListener(_arg0);
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_send:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
int _result = this.send(_arg0);
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_register:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
java.lang.String _arg1;
_arg1 = data.readString();
int _result = this.register(_arg0, _arg1);
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_saveToUSB:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.saveToUSB();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_logcatToUSB:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.logcatToUSB();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.node.wilson.IWilsonService
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
@Override public android.os.IBinder asBinder()
{
return mRemote;
}
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
@Override public int addListener(com.node.wilson.IWilsonRemoteListener l) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeStrongBinder((((l!=null))?(l.asBinder()):(null)));
mRemote.transact(Stub.TRANSACTION_addListener, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int removeListener(com.node.wilson.IWilsonRemoteListener l) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeStrongBinder((((l!=null))?(l.asBinder()):(null)));
mRemote.transact(Stub.TRANSACTION_removeListener, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int send(java.lang.String data) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(data);
mRemote.transact(Stub.TRANSACTION_send, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int register(java.lang.String ip, java.lang.String port) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(ip);
_data.writeString(port);
mRemote.transact(Stub.TRANSACTION_register, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int saveToUSB() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_saveToUSB, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int logcatToUSB() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_logcatToUSB, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
}
static final int TRANSACTION_addListener = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
static final int TRANSACTION_removeListener = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
static final int TRANSACTION_send = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
static final int TRANSACTION_register = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
static final int TRANSACTION_saveToUSB = (android.os.IBinder.FIRST_CALL_TRANSACTION + 4);
static final int TRANSACTION_logcatToUSB = (android.os.IBinder.FIRST_CALL_TRANSACTION + 5);
}
public int addListener(com.node.wilson.IWilsonRemoteListener l) throws android.os.RemoteException;
public int removeListener(com.node.wilson.IWilsonRemoteListener l) throws android.os.RemoteException;
public int send(java.lang.String data) throws android.os.RemoteException;
public int register(java.lang.String ip, java.lang.String port) throws android.os.RemoteException;
public int saveToUSB() throws android.os.RemoteException;
public int logcatToUSB() throws android.os.RemoteException;
}
