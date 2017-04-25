#!/bin/bash
ROOT_PATH=$(pwd)
INCLUDE=$ROOT_PATH

#http src
SRC=$(ls *.c|tr '\n' ' ')
OBJ=$(ls *.c|sed 's/\.c/\.o/g'|tr '\n' ' ')
SERVER_NAME=thttpd
LIB=-lpthread
cc=gcc


#cgi
CGI_PATH=$ROOT_PATH/cgi/
CGI_SRC=$(ls $ROOT_PATH/cgi/ |grep -E '.c$')
CGI_BIN=$(ls $ROOT_PATH/cgi/ |grep -E '.c$'|sed 's/\.c/ /g')
cat << EOF >$CGI_PATH/makefile
${CGI_BIN}:${CGI_SRC}
	$cc -o \$@ \$^ 
.PHONY:clean clean:
	rm	-f $CGI_BIN
.PHONY:output
output:
	cp $CGI_BIN ..
EOF

#thttp Makfile
cat << EOF > makefile
.PHONY:all
all:${SERVER_NAME} cgi
${SERVER_NAME}:${OBJ}
	$cc -o \$@ \$^ $LIB
%.o:%.c
	$cc -c \$<
cgi:
	cd cgi;make;make output;cd -
.PHONY:clean
clean:
	rm	-rf *.o $SERVER_NAME output; cd cgi; make clean; cd -
.PHONY:output
output:all
	mkdir output
	cp ${SERVER_NAME} output/
	cp -rf log output/
	cp -rf conf output/
	cp -rf wwwroot output/
	mkdir -p output/wwwroot/cgi_bin   
	cp -r $CGI_BIN	output/wwwroot/cgi_bin
	cp -r http_ctl.sh output/
EOF
