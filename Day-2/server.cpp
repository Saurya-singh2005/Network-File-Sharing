#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
using namespace std;
void sendFileList(int clientSocket) {
 DIR* dir = opendir("server_files");
 struct dirent* entry;
 string files = "";
 while ((entry = readdir(dir)) != NULL) {
 if (entry->d_type == DT_REG)
 files += string(entry->d_name) + "\n";
 }
 closedir(dir);
 if (files.empty()) files = "No files found\n";
 send(clientSocket, files.c_str(), files.size(), 0);
}
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
 while (true) {
 char buffer[1024] = {0};
 int bytes = read(clientSocket, buffer, sizeof(buffer));
 if (bytes <= 0) break;
 string command(buffer);
 if (command == "LIST") sendFileList(clientSocket);
 else if (command == "EXIT") break;
 }
 close(clientSocket);
 close(serverSocket);
 return 0;
}
