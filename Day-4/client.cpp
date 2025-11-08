#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
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
 cout << "\nEnter command (LIST, GET <file>, PUT <file>, or EXIT): ";
 string input;
 getline(cin, input);
 send(clientSocket, input.c_str(), input.size(), 0);
 if (input == "EXIT") {
 cout << "Disconnected from server.\n";
 break;
 }
 else if (input == "LIST") {
 char buffer[2048] = {0};
 int bytes = read(clientSocket, buffer, sizeof(buffer));
 cout << "\nFiles on server:\n" << buffer << endl;
 }
 else if (input.rfind("GET", 0) == 0) {
 string filename = input.substr(4);
 ofstream file("downloaded_" + filename, ios::binary);
 char buffer[1024];
 int bytes;
 while ((bytes = read(clientSocket, buffer, sizeof(buffer))) > 0) {
 file.write(buffer, bytes);
 if (bytes < 1024) break;
 }
 file.close();
 cout << "File downloaded as downloaded_" << filename << endl;
 }
 else if (input.rfind("PUT", 0) == 0) {
 string filename = input.substr(4);
 ifstream file(filename, ios::binary);
 if (!file.is_open()) {
 cout << "File not found: " << filename << endl;
 continue;
 }
 char buffer[1024];
 while (!file.eof()) {
 file.read(buffer, sizeof(buffer));
 send(clientSocket, buffer, file.gcount(), 0);
 }
 file.close();
 cout << "File uploaded successfully!\n";
 }
 }
 close(clientSocket);
 return 0;
}
