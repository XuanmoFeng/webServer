#!/bin/bash 
ROOT_PATH=`pwd`
BIN=$(ROOT_PATH)/thttpd
CONF=$(ROOT_PATH)/conf/thttpd.conf
PID=$ROOT_PATH/thttpd.pid

proc=`basename $0`
function http_start(){
    [[ -f $PID ]]&&{
        printf "httpd is exist ,pid is $(cat $PID)\n"
        return 
    }
    ip=$(grep -E '^IP:' $CONF | awk -F: '{print $2}')
    port=$(grep -E '^PORT:'$CONF |awk -F: '{print $2}')
    $BIN $ip $port
    pidof $(basename $BIN) > $PID
    printf"start done ,pid is :$(cat $PID)...\n "
}
function http_stop(){
    [[! -f $PID ]]&&{
        printf "httpd is exist ,pid is $(cat $PID)\n"
        return 
    }
    pid=$(cat $PID)
    kill -9 $PID
    rm -f $PID
    printf "stop done....\n"
}
[[ $# -ne 1 ]]&&{
    usage
    exit 1
}
case $1 in
    start|-s)
        http_start
    ;;
    stop|-t)
        http_stop
    ;;
    restart |-rt)
    ;;
    *)
    usage
    exit 2
esac

