all: index.cgi start_kodi
	chmod a+s index.cgi start_kodi
	ls -l

index.cgi: index.c
	gcc -Wall -O3 -o index.cgi index.c

start_kodi: start_kodi.c
	gcc -Wall -O3 -o start_kodi start_kodi.c
