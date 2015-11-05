#!/bin/sh
#==============================================
# dump log message to named pipe of nodebus
# Any question : sanghyeun.kim@lge.com
#==============================================
NPIPE="/tmp/Nodebus/atmgr.log";
CTLPIPE="/tmp/Nodebus/acnode.ctl";
PRE_LOG="/tmp/Nodebus/syslog_pre";
CUR_LOG="/tmp/Nodebus/syslog_cur";

if [ -f $CUR_LOG ] ; then exit 0 ; fi
if [ ! -d /tmp/Nodebus ] ; then mkdir -p /tmp/Nodebus ; fi
if [ ! -p $NPIPE ] ; then mkfifo $NPIPE ; fi
if [ ! -p $CTLPIPE ] ; then mkfifo $CTLPIPE ; fi

MACH="$(uname -m)";

# on server 
if test "$MACH" = "x86_64" ; then

while /bin/true ; do
	dmesg > $NPIPE;
sleep 5;
break;
done 

else
# on target 

touch $PRE_LOG;
touch $CUR_LOG;

klogd ;
syslogd -S -C8 ;
echo "printk:1" > /proc/logm/status;

dmesg > $NPIPE;
cat /proc/logm/lmsg > $NPIPE;

while /bin/true ; do
		logread > $CUR_LOG;
		DIFF="$(diff $CUR_LOG $PRE_LOG)";

		if [ -n "$DIFF" ] ; then 
		diff $CUR_LOG $PRE_LOG | grep -E "^-[^-]" > $NPIPE;
		cp $CUR_LOG $PRE_LOG;
		fi
sleep 3;
done 

fi
#==============================================
