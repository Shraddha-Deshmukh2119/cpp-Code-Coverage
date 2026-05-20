#pragma once
#include "Complaint_Base.h"

class Complaint_C final : public Complaint_Base
{
protected:
    char client[500] = "\0";

public:
    Complaint_C(const char Client[500], const string& FileName) : Complaint_Base(FileName) 
    {
        strcpy(this->client, Client);
    }

    bool write(const char text[500])
    {
        return Complaint_Base::write(text, this->client);
    }

    void reView(ostream& os)
    {
        if (Complaint_Base::see(complaint, this->client, false))
            Complaint_Base::view(complaint, os);
        else
            cerr << " You have not registered any complaint so far" << endl;
    }

    bool update(const char newComplaint[500], bool& status)
    {
        return Complaint_Base::update(newComplaint, this->client, status);
    }
};
