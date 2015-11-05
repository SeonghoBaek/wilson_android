#!/bin/bash
#==============================================
# dump log message to named pipe of nodebus
# Any question : sanghyeun.kim@lge.com
#==============================================
NPIPE="/tmp/Nodebus/atmgr.log";

if [ ! -d /tmp/Nodebus ] ; then mkdir -p /tmp/Nodebus ; fi
if [ ! -p $NPIPE ] ; then mkfifo $NPIPE ; fi

dmesg > $NPIPE & 

sleep 2;

dmesg > $NPIPE &

sleep 2;

dmesg > $NPIPE &

#cat $NPIPE;

#==============================================
