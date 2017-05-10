all: 
	g++ -std=c++11 myserver.cpp tcpstream.cpp tcpconnector.cpp tcpacceptor.cpp myqueue.cpp node.cpp -lpthread -o myserver
	g++ -std=c++11 myclient.cpp tcpstream.cpp tcpconnector.cpp tcpacceptor.cpp myqueue.cpp node.cpp -lpthread -o myclient
clean:
	rm myclient myserver
