# Keylogger Project

## Instructions for Use
1. Compile the C++ program using the following command: g++ keylogger.cpp -o keylogger

2. Run the compiled program with: ./keylogger

3. The keystrokes will be logged in the `Keylogger.txt` file located in the same directory.

4. The Keylogger.txt file is cleaned with the following command: truncate -s 0 /home/kali/keylogger_project/keylogger.txt
 

# System Assumptions
The program assumes the following:
- The operating system is a Linux distribution (Kali Linux is recommended).
- The user has sudo privileges.
- C++ development tools are installed.

# Use Cases
- The keylogger can be used for:
- Monitoring your own system for educational purposes.
- Testing the security of your own devices.

This keylogger should be used only on systems where you have explicit permission.


