#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <regex>

// Function to convert time from hh:mm:ss,ms to milliseconds
long timeToMilliseconds(const std::string& time) {
    std::regex timeRegex("(\\d+):(\\d+):(\\d+),(\\d+)");
    std::smatch matches;
    if (std::regex_search(time, matches, timeRegex)) {
        long hours = std::stol(matches[1]);
        long minutes = std::stol(matches[2]);
        long seconds = std::stol(matches[3]);
        long milliseconds = std::stol(matches[4]);
        return hours * 3600000 + minutes * 60000 + seconds * 1000 + milliseconds;
    }
    return 0;
}

// Function to convert milliseconds to time format hh:mm:ss,ms
std::string millisecondsToTime(long milliseconds) {
    long hours = milliseconds / 3600000;
    milliseconds %= 3600000;
    long minutes = milliseconds / 60000;
    milliseconds %= 60000;
    long seconds = milliseconds / 1000;
    milliseconds %= 1000;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ':'
        << std::setw(2) << minutes << ':' << std::setw(2) << seconds << ','
        << std::setw(3) << milliseconds;
    return oss.str();
}

// Function to shift the time in a subtitle line
std::string shiftSubtitleTime(const std::string& line, long shiftMilliseconds) {
    std::regex timeRegex("(\\d+:\\d+:\\d+,\\d+) --> (\\d+:\\d+:\\d+,\\d+)");
    std::smatch matches;
    if (std::regex_search(line, matches, timeRegex)) {
        std::string startTime = matches[1];
        std::string endTime = matches[2];
        long startMilliseconds = timeToMilliseconds(startTime);
        long endMilliseconds = timeToMilliseconds(endTime);
        startMilliseconds += shiftMilliseconds;
        endMilliseconds += shiftMilliseconds;
        return millisecondsToTime(startMilliseconds) + " --> " + millisecondsToTime(endMilliseconds);
    }
    return line;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input.srt> <output.srt> <shift_milliseconds>\n";
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];
    long shiftMilliseconds = std::stol(argv[3]);

    std::ifstream inputFile(inputFilePath);
    std::ofstream outputFile(outputFilePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file: " << inputFilePath << "\n";
        return 1;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFilePath << "\n";
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        outputFile << shiftSubtitleTime(line, shiftMilliseconds) << "\n";
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Subtitle time shift complete.\n";
    return 0;
}
