#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <string.h>
using namespace std;

struct Complaint 
{
    char text[500] = "\0";
    char answer[500] = "\0";
    char client[500] = "\0";
    bool seen = false;
    bool answered = false;
};

class Complaint_Base
{
protected:
    Complaint complaint;
    const string fileName;

public:    
    Complaint_Base(const string& FileName) : fileName(FileName) {}

    bool write(const char text[500], const char client[500]);
    void view(const Complaint& complaint, ostream& os);
    bool see(Complaint& readIn, const char client[500], bool seenStatus);
    bool update(const char newComplaint[500], const char client[500], bool& status);
};