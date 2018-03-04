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

#ifndef LIBRARY_READWRITE_H_
#define LIBRARY_READWRITE_H_

#include <string>

using std::string;

namespace tids {

string read(string filepath, string filename);
int write(string filepath, string filename, string value);
int write(string filepath, string filename, int value);

} /* namespace tids */

#endif /* LIBRARY_READWRITE_H_ */
