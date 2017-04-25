.PHONY:all
all:thttpd cgi
thttpd:main.o thttp.o 
	gcc -o $@ $^ -lpthread
%.o:%.c
	gcc -c $<
cgi:
	cd cgi;make;make output;cd -
.PHONY:clean
clean:
	@rm	-rf *.o thttpd output; cd cgi; make clean; cd -
.PHONY:output
output:all
	mkdir output
	cp thttpd output/
	cp -rf log output/
	cp -rf conf output/
	cp -rf wwwroot output/
	mkdir -p output/wwwroot/cgi_bin   
	cp -r hello 	output/wwwroot/cgi_bin
	cp -r http_ctl.sh output/
