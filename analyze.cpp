#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

// Struct to represnt a 2d coordinate with heading
struct pos2d{
    int x;
    int y;
    int h;
};

// Struct to represent a game controller
struct gamePad {
    float ly;
    float lx;
    float rx;
};

// Struct to represent a coordinate in time
struct timePos{
    time_t time;
    pos2d position;
    //gamePad gamePad;
};

// function to parse a date or time string.
// Geeks for Geeks
timePos parseDateTime(string datetimeString)
{
    struct tm datetime;
    pos2d position;
    gamePad game;
    vector<string> dateInfo;
    // Fills the vector with the date data
    string currStr = "";
    for (int i = 0; i <= datetimeString.length(); i++) {
        
        if (i == datetimeString.length()-1) {
            currStr+=datetimeString[i];
            dateInfo.push_back(currStr);
            
        }
        else {
            if (datetimeString[i] == ',' && datetimeString[i+1] == ' ') {
                dateInfo.push_back(currStr);
                currStr = "";
            }
            else {
                currStr+=datetimeString[i];
            }
        }
    }
    datetime.tm_year = stoi(dateInfo[2]) - 1900; // Number of years since 1900
    datetime.tm_mon = stoi(dateInfo[1]) - 1; // Number of months since January
    datetime.tm_mday = stoi(dateInfo[0]);
    datetime.tm_hour = stoi(dateInfo[3]);
    datetime.tm_min = stoi(dateInfo[4]);
    datetime.tm_sec = stoi(dateInfo[5]);
    position.x = stoi(dateInfo[6]);
    position.y = stoi(dateInfo[7]);
    position.h = stoi(dateInfo[8]);
    game.ly = stof(dateInfo[9]);
    game.lx = stof(dateInfo[10]);
    game.rx = stof(dateInfo[11]);

    // -1 corresponds to the local time zone setting
    datetime.tm_isdst = -1;
    time_t time = mktime(&datetime);
    timePos timePos;
    timePos.time = time;
    timePos.position = position;
    //timePos.gamePad = game;
    return(timePos);
}

vector<string> getData(string theFile) {
    string text;
    vector<string> allText;
    ifstream file(theFile);
    while (getline (file, text)) {
        // Output the text from the file
        allText.push_back(text);
    }
    file.close();
    return allText;
}

// Calculates the similarity between the 2 paths
// Includes time
double getAvgDistance(vector<timePos> coordinates1, vector<timePos> coordinates2) {
    double avgDistance;
    if (coordinates1.size() <= coordinates2.size()) {
        double distances = 0;
        int len = coordinates1.size();
        for (int i = 0; i < coordinates1.size(); i++) {
            double distance = sqrt(pow((coordinates2[i].position.y-coordinates1[i].position.y), 2) + pow((coordinates2[i].position.x-coordinates1[i].position.x), 2));
            distances += distance;
        }
        avgDistance = distances/len;
    }
    else {
        double distances = 0;
        int len = coordinates2.size();
        for (int i = 0; i < coordinates2.size(); i++) {
            double distance = sqrt(pow((coordinates2[i].position.y-coordinates1[i].position.y), 2) + pow((coordinates2[i].position.x-coordinates1[i].position.x), 2));
            distances += distance;
        }
        avgDistance = distances/len;
    }
    return avgDistance;
}

// Converts a coordinate into a string
string toStr(pos2d pos) {
    string str = "(" + pos.x;
    str += ", " + pos.y;
    str += ")";
    return str;
}

// Returns a vector of the key points in the list of coordinates
vector<pos2d> getKeyPts(timePos coordinates[], int size) {
    vector<pos2d> allPts;
    // added X and Y represent the most recent x or y values added to the key points list
    int addedX = coordinates[0].position.x;
    int addedY = coordinates[0].position.y;
    // last represents the point that was analyzed prior to the current point
    pos2d lastPt = coordinates[0].position;
    // Represents whether the current line is increasing, decreasing, or remaining flat
    // 1, -1, 0
    int xDir = 0;
    int yDir = 0;
    allPts.push_back(coordinates[0].position);
    // Checks whether each point is increasing, decreasing, or remaining the same
    for (int i = 0; i < size; i++) {
        int x = coordinates[i].position.x;
        int y = coordinates[i].position.y;
        // Checking if the previous point was a local min or max in the x axis
        if (lastPt.x != x) {
            if (x < lastPt.x) {
                if (xDir==1) {
                    allPts.push_back(lastPt);
                    addedX = lastPt.x;
                    xDir = 0;
                }
                else {
                    xDir = -1;
                }
            }
            if (x > lastPt.x) {
                if (xDir==-1) {
                    
                    allPts.push_back(lastPt);
                    addedX = lastPt.x;
                    xDir = 0;
                }
                else {
                    xDir = 1;
                }
            }
        }
        else if (lastPt.x != addedX) {
            allPts.push_back(lastPt);
            addedX = lastPt.x;
            xDir = 0;
        }

        // Checking if the previous point was a local min or max in the y axis
        if (lastPt.y != y) {
            if (y < lastPt.y) {
                if (yDir==1) {
                    allPts.push_back(lastPt);
                    addedY = lastPt.y;
                    yDir = 0;
                }
                else {
                    yDir = -1;
                }
            }
            if (y > lastPt.y) {
                if (yDir==-1) {
                    
                    allPts.push_back(lastPt);
                    addedY = lastPt.y;
                    yDir = 0;
                }
                else {
                    yDir = 1;
                }
            }
        }
        else if (lastPt.y != addedY) {
            allPts.push_back(lastPt);
            addedY = lastPt.y;
            yDir = 0;
        }
        
        lastPt = coordinates[i].position;

        pos2d last = allPts[allPts.size()-1];
        pos2d secondLast = allPts[allPts.size()-2];
        if (last.x == secondLast.x && last.y == secondLast.y) {
            allPts.pop_back();
        }


    }
    
    allPts.push_back(coordinates[size-1].position);
    return allPts;
}



// Main function that runs the code
int main() {
    vector<string> strData = getData("data/05-02-2025-10-33.txt");
    int numLines = strData.size();
    const char* format = "dd, MM, yyyy, HH, mm, ss";

    timePos coordinates[numLines];

    // Parsing string data
    for (int i = 0; i < numLines; i++) {
        coordinates[i] = parseDateTime(strData[i]);
    }
    //double avg = getAvgDistance(coordinates, coordinates2);
    //cout << "Average:" << avg << "\n";
    vector<pos2d> pts = getKeyPts(coordinates, numLines);
    cout << "Points:" << "\n";
    for (pos2d pt : pts) {
        cout << "(" << pt.x << ", " << pt.y << ")" << "\n";
    }
    cout << "\n";

}