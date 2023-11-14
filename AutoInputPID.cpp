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
    // Untuk Windows
    HWND hwnd = FindWindowA(NULL, "Audition"); // Sesuaikan dengan judul jendela Audition
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
#else
    // Untuk sistem Unix-based (Linux, macOS, dll.)
    // Dapatkan PID menggunakan perintah ps
    string cmd = "pidof Audition"; // Sesuaikan dengan nama proses Audition
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        cerr << "Gagal mendapatkan PID Audition." << endl;
        return -1;
    }

    char buffer[128];
    string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);

    // Hapus karakter newline jika ada
    size_t pos = result.find('\n');
    if (pos != string::npos) {
        result.erase(pos);
    }

    return stoi(result);
#endif
}

// Fungsi untuk menulis PID ke dalam file
void writePIDToFile(const string& fileName, int pid) {
    ifstream inFile(fileName);

    if (!inFile) {
        cerr << "Gagal membuka file " << fileName << endl;
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
        cerr << "Gagal membuka file " << fileName << " untuk ditulis" << endl;
        return;
    }

    outFile << newContent;

    outFile.close();
}

int main() {
    // Nama file konfigurasi
    const string fileName = "app.conf";

    // Dapatkan PID Audition
    int auditionPID = getAuditionPID();
    if (auditionPID == -1) {
        cerr << "Gagal mendapatkan PID Audition. Program berhenti." << endl;
        return 1;
    }

    // Tulis PID ke dalam file
    writePIDToFile(fileName, auditionPID);

    // Menjalankan perintah "app.py"
    cout << "Menjalankan app.py..." << endl;
    string command = "python app.py"; // Sesuaikan dengan perintah yang sesuai untuk menjalankan app.py
    int result = system(command.c_str());

    if (result == 0) {
        cout << "app.py telah berhasil dijalankan." << endl;
    }
    else {
        cerr << "Gagal menjalankan app.py." << endl;
    }

    return 0;
}