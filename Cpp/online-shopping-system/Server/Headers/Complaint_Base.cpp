#include "Complaint_Base.h"

bool Complaint_Base::write(const char text[500], const char client[500])
{
    fstream file;
    file.open(fileName, ios::in | ios::binary);
    if (!file.eof())
    {
        // creating a file if it doesnt exist
        fstream fcreate(fileName, ios::out | ios::binary | ios::app);
        fcreate.close();
        
        file.close();
        file.open(fileName, ios::in | ios::binary);
    }

    file.seekg(0, ios::end);
    const size_t size = file.tellg();
    file.seekg(0, ios::beg);

    bool unique = true;
    while(file.tellg() < size)
    {
        file.read(reinterpret_cast<char*> (&complaint), sizeof(complaint));

        if (string(complaint.client) == string(client))
        {
            unique = false;
            break;
        }
    }
    file.close();

    if (unique)
    {
        strcpy(complaint.text, text);
        strcpy(complaint.client, client);
        strcpy(complaint.answer, "*no answer*");
        complaint.answered = false;
        complaint.seen = false;

        file.open(fileName, ios::out | ios::binary | ios::app);
        file.seekp(0);
        file.write(reinterpret_cast<const char*> (&complaint), sizeof(complaint));
        file.close();        
    }
    return unique;
}

void Complaint_Base::view(const Complaint& complaint, ostream& os)
{
    os << endl;
    os << left << setw(25) << "\tName of client";
    os << " : " << complaint.client << endl;
    os << left << setw(25) << "\tComplaint";
    os << " : " << complaint.text << endl;
    os << left << setw(25) << "\tAnswer status";
    os << " : " << boolalpha << complaint.answered << endl;
    os << left << setw(25) << "\tSeen status";
    os << " : " << boolalpha << complaint.seen << endl;
    os << left << setw(25) << "\tResponse by Company" ;
    os << " : " << complaint.answer << endl;
}

bool Complaint_Base::see(Complaint& readIn, const char client[500], bool seenStatus)
{
    fstream file(fileName, ios::out | ios::in | ios::binary | ios::ate);
    
    file.seekg(0, ios::end);
    int size = file.tellg();

    file.seekg(0, ios::beg);
    file.seekp(0, ios::beg);

    bool found = false;
    size_t previous;

    while(file.tellg() < size)
    {
        previous = file.tellp();
        file.read(reinterpret_cast<char*> (&readIn), sizeof(readIn));

        if (string(readIn.client) == string(client))
        {
            if (seenStatus)
            {
                readIn.seen = true;
                file.seekp(previous, ios::beg);
                file.write(reinterpret_cast<const char*> (&readIn), sizeof(readIn));
            }

            found = true;
            break;
        }
    }

    file.close();
    return found;
}

bool Complaint_Base::update(const char newComplaint[500], const char client[500], bool& status)
{
    fstream file(fileName, ios::out | ios::in | ios::binary | ios::ate);
    
    file.seekg(0, ios::end);
    int size = file.tellg();

    file.seekg(0, ios::beg);
    file.seekp(0, ios::beg);

    bool found = false;
    status = false;
    size_t previous;

    while(file.tellg() < size)
    {
        previous = file.tellp();
        file.read(reinterpret_cast<char*> (&complaint), sizeof(complaint));

        if (string(complaint.client) == string(client))
        {   
            if (complaint.answered)
                status = true;
            else
            {
                strcpy(complaint.text, newComplaint);
                file.seekp(previous, ios::beg);
                file.write(reinterpret_cast<const char*> (&complaint), sizeof(complaint));
                found = true;
            }
            break;
        }
    }

    file.close();
    return found;
}
