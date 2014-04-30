all: tcp_client tcp_server


udp_client:
	gcc -o tcp_client tcp_client.c

udp_server:
	gcc -o tcp_server tcp_server.c

clean: 
	rm tcp_client tcp_server