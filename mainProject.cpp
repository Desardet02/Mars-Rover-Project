//
// Created by Bill Wright on 10/28/26.
//

#include <iostream>
#include <fstream>
using namespace std;

#include "Location.h"
#include "World.h"
#include "Direction.h"
#include "Mission.h"
#include "Rover.h"

// split helper function
void split(string line, char delimiter, string fields[], int capacity) {
    if (capacity <= 0) {
        return;
    }

    int count = 0;
    string field;
    for (char character : line) {
        if (character == delimiter) {
            if (count >= capacity) {
                return;
            }
            fields[count++] = field;
            field = "";
        } else {
            field += character;
        }
    }

    if (count >= capacity) {
        return;
    }
    fields[count++] = field;
    return;
}

bool isValidMissionLine(const string& line) {
    int commaCount = 0;
    for (char character : line) {
        if (character == ',') {
            commaCount++;
        }
    }
    if (commaCount != 2) {
        return false;
    }

    string fields[3];
    split(line, ',', fields, 3);

    if (fields[0].empty()) {
        return false;
    }
    for (char command : fields[0]) {
        if (command != 'F' && command != 'L' && command != 'R'
            && command != 'S' && command != 'C') {
            return false;
        }
    }

    for (int i = 1; i <= 2; i++) {
        if (fields[i].length() != 1) {
            return false;
        }
        if (fields[i][0] < '0' || fields[i][0] > '9') {
            return false;
        }
    }
    return true;
}

int main() {
    cout << "Enter input filename: ";
    string inputFilename;
    cin >> inputFilename;
    ifstream inFile;
    inFile.open(inputFilename);
    if (inFile.fail()) {
        cout << "Error: could not open input file " << inputFilename << endl;
        return 1;
    }

    cout << "Enter output filename: ";
    string outputFilename;
    cin >> outputFilename;
    ofstream outFile;
    outFile.open(outputFilename);
    if (outFile.fail()) {
        cout << "Error: could not open output file " << outputFilename << endl;
        return 1;
    }

    int WORLD_SIZE = 10;
    Location chargerLocation{0, 0};
    World mars(WORLD_SIZE, chargerLocation);

    Location startingLocation{0, 0};
    Rover rover(startingLocation, Direction::EAST, 40, outFile);
    rover.putOnWorld(mars);

    string line;
    while (getline(inFile, line)) {
        if (!isValidMissionLine(line)) {
            continue;
        }
        string fields[3];
        split(line, ',', fields, 3);
        Mission mission(fields[0]);
        Location goal(stoi(fields[1]), stoi(fields[2]));
        mission.setGoal(goal);
        rover.executeMission(mission);
        cout << mission.getStatus();
        cout << "Rover ended at " << rover.toString() << endl;
    }
    return 0;
}
