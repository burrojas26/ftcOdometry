#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

struct pos2d{
    int x;
    int y;
    int h;
};

struct gamePad {
    float ly;
    float lx;
    float rx;
};

struct timePos{
    time_t time;
    pos2d position;
    gamePad gamePad;
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

    // Daylight Savings must be specified
    // -1 uses the computer's timezone setting
    datetime.tm_isdst = -1;
    time_t time = mktime(&datetime);
    timePos timePos;
    timePos.time = time;
    timePos.position = position;
    timePos.gamePad = game;
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

double getAvgDistance(vector<timePos> coordinates1, vector<timePos> coordinates2) {
    double avgDistance;
    if (size(coordinates1) <= size(coordinates2)) {
        double distances = 0;
        int len = size(coordinates1);
        for (int i = 0; i < size(coordinates1); i++) {
            double distance = sqrt(pow((coordinates2[i].position.y-coordinates1[i].position.y), 2) + pow((coordinates2[i].position.x-coordinates1[i].position.x), 2));
            distances += distance;
        }
        avgDistance = distances/len;
    }
    else {
        double distances = 0;
        int len = size(coordinates2);
        for (int i = 0; i < size(coordinates2); i++) {
            double distance = sqrt(pow((coordinates2[i].position.y-coordinates1[i].position.y), 2) + pow((coordinates2[i].position.x-coordinates1[i].position.x), 2));
            distances += distance;
        }
        avgDistance = distances/len;
    }
    return avgDistance;
}



int main() {
    vector<string> strData = getData("data/05-02-2025-10-33.txt");
    vector<string> strData2 = getData("data/05-02-2025-10-34.txt");
    const char* format = "dd, MM, yyyy, HH, mm, ss";
    vector<timePos> coordinates;
    vector<timePos> coordinates2;
    for (string str : strData) {
        coordinates.push_back(parseDateTime(str));
    }
    for (string str : strData2) {
        coordinates2.push_back(parseDateTime(str));
    }
    
    // for (timePos coord : coordinates) {
    //     cout << ctime(&coord.time) << "\n";
    // }
    double avg = getAvgDistance(coordinates, coordinates2);
    cout << avg;
}