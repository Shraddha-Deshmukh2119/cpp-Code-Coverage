#include "Customer.h"

// Customers are asked to choose between signup and login
void Customer::start()
{
    bool go = true;
    int ans;

    while(go)
    {
        system("cls");

        welcome();
        cout<< " You want to:\n"
            << "\t1) Sign up\n"
            << "\t2) Login\n"
            << "\t3) Back\n";
        
        cout << "\n Your choice > ";
        cin >> ans;
        fflush(stdin);

        switch (ans)
        {
        case 1:
            (*client).Send(1);
            signup();
            break;
        
        case 2:
            (*client).Send(2);
            login();
            break;
        
        case 3:
            (*client).Send(3);
            go = false;
            break;
    
        default:
            cout << " Invalid choice!";
            cout << "\n Press any key to go enter again...\n";
            getch();
            break;
        }
        fflush(stdin);
    }
}

void Customer::signup()
{
    system("cls");
    heading("SIGN-UP");

    if (input())
        cout << endl << "\n Account successfully made.";
    else
        cout << endl << "\n Sorry! An error occured while processing the data.\n";
    getch();
}

void Customer::home()
{
    bool go = true;
    int ans;

    while(go)
    {
        system("cls");

        heading("HOME");
        cout<< " You want to:\n"
            << "\t1) Buy goods\n"
            << "\t2) Write a complaint\n"
            << "\t3) View profile\n"
            << "\t4) Log out\n";
        
        cout << "\n Your choice > ";
        cin >> ans;
        fflush(stdin);

        switch (ans)
        {
        case 1:
            (*client).Send(1);
            buy();
            getch();
            break;
        
        case 2:
            (*client).Send(2);
            complain();
            getch();
            break;
        
        case 3:
            (*client).Send(3);
            profile();
            break;
            
        case 4:
            (*client).Send(4);
            go = false;
            break;
    
        default:
            cout << " Invalid choice!";
            cout << "\n Press any key to go enter again...\n";
            getch();
            break;
        }
        fflush(stdin);
    }
}

void Customer::complain()
{
    bool Go = true;
    int ans;
    string str;
    
    while(Go)
    {
        system("cls");
        cout << endl;
        cout << "\t1) Write a complaint\n";
        cout << "\t2) Re-view your complaint\n";
        cout << "\t3) Change a complaint\n";
        cout << "\t4) Go Back\n";
        cout << " Choose > ";

        fflush(stdin);
        cin >> ans;
        switch (ans)
        {
        case 1:
            {
                (*client).Send(1);
                cout << " Enter the complaint\n > ";
                fflush(stdin);
                getline(cin, str);
                (*client).Send(str);

                (*client).Rec(str);
                if (str == "TRUE")
                    cout << " Your complain has been written\n";
                else
                    cerr << " You have already registered a complaint\n";

                fflush(stdin);
                cin.get();
            }
            break;
        
        case 2:
            (*client).Send(2);
            (*client).Rec(str);
            cout << str << endl;
            
            fflush(stdin);
            cin.get();
            break;
        
        case 3:
            {
                (*client).Send(3);
                cout << " Enter the new complaint\n > ";
                fflush(stdin);
                getline(cin, str);
                (*client).Send(str);

                (*client).Rec(str);
                if (str == "TRUE")
                    cout << " The complaint has been changed\n";
                else
                {
                    if (str == "FAILED")
                        cerr << " Our employee has already answered your complaint";
                    else
                        cerr << " Complaint not found" << endl;
                }
                
                fflush(stdin);
                cin.get();
            }
            break;
        
        case 4:
            (*client).Send(4);
            Go = false;
            break;
        
        default:
            cerr << " Invalid input!\n";
            fflush(stdin);
            cin.get();
            break;
        }
    }
}
