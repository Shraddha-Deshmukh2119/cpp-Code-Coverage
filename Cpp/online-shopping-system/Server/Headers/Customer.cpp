#include "Customer.h"
#include "Complaint_C.h"
#include "test_hooks.h"

// Customers are asked to choose between signup and login
void Customer::start()
{
    string str;
    bool go = true;

    while(go)
    {
        (*server).Rec(str);
        int ans = stoi(str);

        switch(ans)
        {
            case 1:
                signup();
                break;
            case 2:
                login(customerFile);
                break;
            case 3:
                go = false;
                break;
        }
    }
}

void Customer::signup()
{
    if (!input(customerFile))
        cout << endl << "\n An error occured";
    oshop_pause_for_tests();
}

void Customer::home()
{
    string str;
    bool go = true;

    while(go)
    {
        (*server).Rec(str);
        int ans = stoi(str);

        switch(ans)
        {
            case 1:
                buy(customerFile);
                break;
            case 2:
                complain();
                break;
            case 3:
                profile(customerFile, this->lookup);
                break;
            case 4:
                go = false;
                break;
        }
    }
}

void Customer::complain()
{
    Complaint_C complaint(username.c_str(), "complaint.dat");
    bool go = true;
    int ans;
    string str;
    
    while(go)
    {
        (*server).Rec(str);
        ans = stoi(str);

        switch (ans)
        {
        case 1:
            {
                (*server).Rec(str);
                char text[500];
                strcpy(text, str.c_str());

                if (complaint.write(text))
                    (*server).Send("TRUE");
                else
                    (*server).Send("FALSE");                 
            }
            break;
        
        case 2:
        {
            ostringstream ss;
            complaint.reView(ss);
            (*server).Send(ss.str());
            break;
        }

        case 3:
            {
                char newText[500];
                bool AnsStatus; 

                (*server).Rec(str);
                strcpy(newText, str.c_str());

                if (complaint.update(newText, AnsStatus))
                    (*server).Send("TRUE");
                else
                {
                    if (AnsStatus)
                        (*server).Send("FAILED");
                    else
                        (*server).Send("FALSE");
                }
            }
            break;
        
        case 4:
            go = false;
            break;
        }
    }
}
