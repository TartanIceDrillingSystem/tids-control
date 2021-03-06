/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "TIDSControl.h"

#include <iostream>

using namespace tids;

int main () {
    std::cout << "Tartan Ice Drilling System (TIDS) Control" << std::endl;

    std::cout << "Starting." << std::endl;
    TIDSControl *tidsControl = new TIDSControl();

    std::cout << "Testing." << std::endl;
    tidsControl->testLoadCell();

    std::cout << "Stopping." << std::endl;
    delete tidsControl;
}
