#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;
int main() {
 int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
 sockaddr_in serverAddr{};
 serverAddr.sin_family = AF_INET;
 serverAddr.sin_port = htons(8080);
 serverAddr.sin_addr.s_addr = INADDR_ANY;
 bind(serverSocket, (struct sockaddr*)&serverAddr, 
sizeof(serverAddr));
 listen(serverSocket, 1);
 cout << "Server waiting for connection..." << endl;
 int clientSocket = accept(serverSocket, NULL, NULL);
 cout << "Client connected!" << endl;
 close(clientSocket);
 close(serverSocket);
 return 0;
}
