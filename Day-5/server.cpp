#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <fstream>
using namespace std;
void sendFileList(int clientSocket) {
 DIR* dir = opendir("server_files");
 struct dirent* entry;
 string fileList = "";
 while ((entry = readdir(dir)) != NULL) {
 if (entry->d_type == DT_REG)
 fileList += string(entry->d_name) + "\n";
 }
 closedir(dir);
 if (fileList.empty()) fileList = "No files found\n";
 send(clientSocket, fileList.c_str(), fileList.size(), 0);
}
void sendFile(int clientSocket, string filename) {
 ifstream file("server_files/" + filename, ios::binary);
 if (!file.is_open()) {
 string msg = "File not found\n";
 send(clientSocket, msg.c_str(), msg.size(), 0);
 return;
 }
 char buffer[1024];
 while (!file.eof()) {
 file.read(buffer, sizeof(buffer));
 send(clientSocket, buffer, file.gcount(), 0);
 }
 file.close();
 cout << "Sent file: " << filename << endl;
}
void receiveFile(int clientSocket, string filename) {
 ofstream file("server_files/" + filename, ios::binary);
 if (!file.is_open()) {
 cerr << "Error creating file: " << filename << endl;
 return;
 }
 char buffer[1024];
 int bytes;
 while ((bytes = read(clientSocket, buffer, sizeof(buffer))) > 0) {
 file.write(buffer, bytes);
 if (bytes < 1024) break;
 }
 file.close();
 cout << "Received and saved: " << filename << endl;
}
bool authenticateUser(int clientSocket) {
 char buffer[1024] = {0};
 int bytes = read(clientSocket, buffer, sizeof(buffer));
 if (bytes <= 0) return false;
 string credentials(buffer);
 string validUser = "admin";
 string validPass = "1234";
 size_t spacePos = credentials.find(' ');
 if (spacePos == string::npos) return false;
 string user = credentials.substr(0, spacePos);
 string pass = credentials.substr(spacePos + 1);
 if (user == validUser && pass == validPass) {
 string msg = "Access Granted\n";
 send(clientSocket, msg.c_str(), msg.size(), 0);
 return true;
 } else {
 string msg = "Access Denied\n";
 send(clientSocket, msg.c_str(), msg.size(), 0);
 return false;
 }
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
 // Authentication
 if (!authenticateUser(clientSocket)) {
 cout << "Authentication failed. Closing connection." << endl;
 close(clientSocket);
 close(serverSocket);
 return 0;
 }
 cout << "Client authenticated successfully!" << endl;
 // Commands
 while (true) {
 char command[1024] = {0};
 int bytes = read(clientSocket, command, sizeof(command));
 if (bytes <= 0) break;
 string cmd(command);
 if (cmd == "LIST") sendFileList(clientSocket);
 else if (cmd.rfind("GET", 0) == 0) {
 string filename = cmd.substr(4);
 sendFile(clientSocket, filename);
 }
 else if (cmd.rfind("PUT", 0) == 0) {
 string filename = cmd.substr(4);
 receiveFile(clientSocket, filename);
 }
 else if (cmd == "EXIT") {
 cout << "Client disconnected.\n";
 break;
 }
 else {
 string msg = "Invalid command\n";
 send(clientSocket, msg.c_str(), msg.size(), 0);
 }
 }
 close(clientSocket);
 close(serverSocket);
 return 0;
}
