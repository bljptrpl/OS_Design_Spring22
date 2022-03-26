all: bankClient.c bankServer.c banking.h
	gcc -pthread bankClient.c -o bankClient
	gcc -pthread bankServer.c -o bankServer
bankClient: bankClient.c banking.h
	gcc -pthread bankClient.c -o bankClient
bankServer: bankServer.c banking.h
	gcc -pthread bankServer.c -o bankServer
clean:
	rm *.o; rm bankClient; rm bankServer
