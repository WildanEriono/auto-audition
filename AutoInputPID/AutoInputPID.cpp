#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> 
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

int getAuditionPID() {
#ifdef _WIN32
    HWND hwnd = FindWindowA(NULL, "Audition"); 
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
#else
    string cmd = "pidof Audition"; 
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        cerr << "Failed to get Audition's PID." << endl;
        return -1;
    }

    char buffer[128];
    string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);

    size_t pos = result.find('\n');
    if (pos != string::npos) {
        result.erase(pos);
    }

    return stoi(result);
#endif
}

void writePIDToFile(const string& fileName, int pid) {
    ifstream inFile(fileName);

    if (!inFile) {
        cerr << "Failed to access app.conf." << fileName << endl;
        return;
    }

    string line;
    string newContent;

    while (getline(inFile, line)) {
        size_t pos = line.find("pid");
        if (pos != string::npos) {
            line = "pid = " + to_string(pid);
        }
        newContent += line + "\n";
    }

    inFile.close();

    ofstream outFile(fileName);

    if (!outFile) {
        cerr << "Failed to access the file " << fileName << " to write" << endl;
        return;
    }

    outFile << newContent;

    outFile.close();
}

int main() {
    const string fileName = "app.conf";

    int auditionPID = getAuditionPID();
    if (auditionPID == -1) {
        cerr << "Auto Input PID Failed. Program will closed." << endl;
        return 1;
    }

    writePIDToFile(fileName, auditionPID);

    cout << "Run app.py..." << endl;
    string command = "python app.py";
    int result = system(command.c_str());

    if (result == 0) {
        cout << "app.py is running." << endl;
    }
    else {
        cerr << "failed to run app.py." << endl;
    }

    return 0;
}