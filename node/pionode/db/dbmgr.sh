#!/bin/bash




sqlite3 ./ATMgrCTL.db " update atmgr_cmd set command='$(cat ./nbus_dumplog.sh)' \
		where cmd_name='nbus_dumplog' ;";
sqlite3 ./ATMgrCTL.db " select command from atmgr_cmd where cmd_name='nbus_dumplog' ; ";


sqlite3 ./ATMgrCTL.db " update atmgr_cmd set command='$(cat ./nbus_stopdump.sh)' \
		where cmd_name='nbus_stoplog' ;";
sqlite3 ./ATMgrCTL.db " select command from atmgr_cmd where cmd_name='nbus_stoplog' ; ";
