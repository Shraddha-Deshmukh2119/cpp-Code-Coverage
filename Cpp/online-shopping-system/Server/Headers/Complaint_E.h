#pragma once
#include "Complaint_Base.h"

class Complaint_E final : public Complaint_Base
{
public:
    Complaint_E(const string& FileName) : Complaint_Base(FileName) {}
   
    bool answer(const char ans[500], const char client[500]);
    void view(bool showDetails, ostream& os);
    void clear();
};