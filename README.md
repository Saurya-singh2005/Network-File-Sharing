# 🧱 Network File Sharing System (C++ Socket Programming)

### 👨‍💻 Author: *Saurya Singh*

---

## 📘 Overview
This project is a *Client–Server File Sharing System* built using *C++ socket programming*.  
It allows clients to connect to a central server to *list, **download, and **upload* files securely over a TCP connection.  
The final version also includes *user authentication* for secure access.

---

## 🎯 Features
| Feature | Description |
|----------|--------------|
| 🔌 *Connection Setup* | Establishes a TCP connection between client and server |
| 📂 *LIST* | Displays all files available on the server |
| ⬇ *GET* | Downloads selected file from server |
| ⬆ *PUT* | Uploads a file from client to server |
| 🔐 *Authentication* | Requires valid username/password before access |
| 🧹 *EXIT* | Gracefully disconnects the client and server |

---

## 🧩 Project Structure
network_file_sharing/
├── server.cpp # Server-side logic
├── client.cpp # Client-side logic
├── server_files/ # Folder storing server files
│ ├── a.txt
│ ├── b.txt
│ └── upload.txt
└── README.md
---

## ⚙ Tech Stack
- *Language:* C++  
- *Environment:* Ubuntu / WSL (Linux)  
- *Libraries:*  
  - <sys/socket.h> – Socket creation  
  - <netinet/in.h> – Address structure  
  - <arpa/inet.h> – IP address conversion  
  - <unistd.h> – Read/Write/Close functions  
  - <fstream> – File I/O  
  - <dirent.h> – Directory handling  
- *Communication:* TCP protocol

---

## 🚀 How to Run the Project

### 🖥 Step 1 – Compile
```bash
g++ server.cpp -o server
g++ client.cpp -o client
