//
//  EventMessage.h
//  Wilson Android
//
//  Created by major on 11/11/15.
//  Copyright (c) 2015 major. All rights reserved.
//

#ifndef Wilson_Android_Message_h
#define Wilson_Android_Message_h

enum  // node event id
{
    MSG_TYPE_REGOK,
    MSG_TYPE_REGFAIL,
    MSG_TYPE_USB_NOT_READY,
    MSG_TYPE_USB_READY,
    MSG_TYPE_USB_WRITE_FAIL,
    MSG_TYPE_USB_WRITE_DONE,
    MSG_TYPE_USB_WRITE_WAIT,
    MSG_TYPE_USB_LOGCAT_WAIT,
    MSG_TYPE_USB_LOGCAT_FAIL,
    MSG_TYPE_USB_LOGCAT_DONE,
    
    MSG_TYPE_NONE
};

#define MSG_KEYWORD(name) MSG_TYPE_##name
#endif
