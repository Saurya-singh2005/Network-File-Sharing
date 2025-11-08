#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;
int main() {
 int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
 sockaddr_in serverAddr{};
 serverAddr.sin_family = AF_INET;
 serverAddr.sin_port = htons(8080);
 inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
 connect(clientSocket, (struct sockaddr*)&serverAddr, 
sizeof(serverAddr));
 cout << "Connected to server!\n";
 while (true) {
 cout << "\nEnter command (LIST or EXIT): ";
 string input;
 getline(cin, input);
 send(clientSocket, input.c_str(), input.size(), 0);
 if (input == "EXIT") break;
 char buffer[1024] = {0};
 int bytes = read(clientSocket, buffer, sizeof(buffer));
 cout << "Server Response:\n" << buffer << endl;
 }
 close(clientSocket);
 return 0;
}
