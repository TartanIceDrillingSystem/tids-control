/*
    Copyright 2018 CMU Tartan Ice Drilling System (TIDS) Controls
    https://github.com/TartanIceDrillingSystem/tids-control

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "readwrite.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace tids {

string read(string filepath, string filename) {
    ifstream readStream;
    readStream.open((filepath + filename).c_str());
    if (!readStream.is_open()) {
        perror("READWRITE: Read failed to open file.");
    }
    string value;
    getline(readStream, value);
    readStream.close();
    return value;
}

int write(string filepath, string filename, string value) {
    ofstream writeStream;
    writeStream.open((filepath + filename).c_str());
    if (!writeStream.is_open()) {
       perror("READWRITE: Write failed to open file.");
       return -1;
    }
    writeStream << value;
    writeStream.close();
    return 0;
}

int write(string filepath, string filename, int value) {
    stringstream valueStream;
    valueStream << value;
    return write(filepath, filename, valueStream.str());
}

} /* namespace tids */
