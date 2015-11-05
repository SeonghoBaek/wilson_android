#!/bin/sh
#==============================================
# Stop dumping log messages 
# Any question : sanghyeun.kim@lge.com
#==============================================

NPIPE="/tmp/Nodebus/atmgr.log";

cat $NPIPE;
rm $NPIPE;

kill -9 $(pidof syslogd);
kill -9 $(pidof klogd);

cd /tmp/Nodebus;
rm *.seg;
rm syslog_*;

#==============================================
