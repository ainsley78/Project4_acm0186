/* 
 * Project 3 - A SONG OF FILES & DATA
 * Author: Ainsley McLeod
 * Username: acm0186
 * How to compile code: g++ project1_acm0186_Ainsley_McLeod.cpp
 * How to run code: ./a.out
 * https://www.geeksforgeeks.org/cpp/file-handling-c-classes/ - file handling assitance 
 * https://www.geeksforgeeks.org/cpp/vector-in-cpp-stl/ - vector assitance
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <unordered_map>
using namespace std;

string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// check if line is number
bool parseLine(const string &line, double &out) {
    try {
        size_t idx;
        out = stod(trim(line), &idx);
        return idx == trim(line).size();
    } catch (...) {
        return false;
    }
}

// read file
bool readFile(const string &fname, vector<double> &nums, bool &nf, bool &bad) {
    nf = bad = false;
    nums.clear();
    ifstream fin(fname);
    if (!fin.is_open()) { nf = true; return false; }
    string line; double val; bool ok = false;
    while (getline(fin, line)) {
        if (parseLine(line, val)) {
            nums.push_back(val);
            ok = true;
        } else { bad = true; fin.close(); return false; }
    }
    fin.close();
    if (!ok) { bad = true; return false; }
    return true;
}

// stats
double mean(vector<double> &v) {
    double sum = 0;
    for (double x : v) sum += x;
    return sum / v.size();
}
double median(vector<double> &v) {
    int n = v.size();
    if (n % 2 == 1) return v[n/2];
    return (v[n/2 - 1] + v[n/2]) / 2.0;
}

// Fix mode: pick the smallest mode if multiple
double mode(const vector<double> &v) {
    unordered_map<double,int> freq;
    int maxCount = 0;
    for (double x : v) maxCount = max(maxCount, ++freq[x]);

    double smallestMode = 1e308;
    for (auto &p : freq) {
        if (p.second == maxCount && p.first < smallestMode) {
            smallestMode = p.first;
        }
    }
    return smallestMode;
}

int main() {
    cout << "*** Welcome to Ainsley's Data Analyzer ***\n\n";

    // CSV output prompt with robust error handling
    ofstream fout;
    string outname;
    while (true) {
        cout << "Enter the output filename to save: ";
        getline(cin, outname);
        fout.open(outname);
        if (fout.is_open()) break;
        cout << "Invalid file path. Please try again.\n\n";
    }
    cout << "*** File has been opened for writing ***\n\n";

    int n;
    while (true) {
        cout << "Enter the number of files to read: ";
        string tmp;
        getline(cin, tmp);
        try {
            n = stoi(trim(tmp));
            if (n > 0) break;
        } catch (...) {}
        cout << "Invalid input. Please enter a positive integer.\n\n";
    }
    cout << "\n";

    vector<double> all;
    for (int i = 1; i <= n; i++) {
        while (true) {
            cout << "Enter the filename for file " << i << ": ";
            string fname; getline(cin, fname); fname = trim(fname);
            vector<double> vals; bool nf, bad;
            if (!readFile(fname, vals, nf, bad)) {
                if (nf) cout << "File not found. Please try again.\n\n";
                else if (bad) cout << "Not an input file. Illegal content/structure detected. Please try again.\n\n";
                continue;
            }
            cout << "\nThe list of " << vals.size() << " values in file " << fname << " is:\n";
            for (double v : vals) cout << v << "\n";
            cout << "\n";
            all.insert(all.end(), vals.begin(), vals.end());
            break;
        }
    }

    sort(all.begin(), all.end());
    cout << "*** Summarized Statistics ***\n\n";
    cout << "The orderly sorted list of " << all.size() << " values is:\n";
    for (int i = 0; i < (int)all.size(); i++) {
        cout << all[i];
        if (i != (int)all.size() - 1) cout << " ";
        // write to CSV
        fout << all[i];
        if (i != (int)all.size() - 1) fout << ",";
    }
    fout << "\n\n";

    cout << "\n";

    cout << fixed << setprecision(4);
    double avg = mean(all);
    double med = median(all);
    double mod = mode(all);

    cout << "The mean is " << avg << "\n";
    cout << "The median is " << med << "\n";
    cout << "The mode is " << mod << "\n\n";

    // write stats to CSV
    fout << "mean," << avg << "\n";
    fout << "median," << med << "\n";
    fout << "mode," << mod << "\n";

    cout << "*** Goodbye. ***\n";

    fout.close();
}
