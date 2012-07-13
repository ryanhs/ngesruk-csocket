#!/bin/sh

start(){
	cd source
	gcc -Wall -o release main.c application.c client.c lookup.c queue.c -lm -lpthread
	cd ..
	mv source/release release/ngesruk-csocket
	./release/ngesruk-csocket
	echo "ngesruk-csocket started."
}

stop(){
	killall -q ngesruk-csocket
	echo "ngesruk-csocket stopped."
}

restart(){
	stop
	start
}


case "$1" in
	
	start)
		start
		;;
	
	stop)
		stop
		;;
	
	restart)
		restart
		;;
	
	*)
		echo "Usage: $0 start|stop|restart"
		exit 1
		;;
esac

:
