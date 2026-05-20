#include "Employee.h"

void Employee::home()
{
    bool go = true;
    int ans;

    while(go)
    {
        system("cls");
        heading("HOME");
        cout<< " You want to:\n"
            << "\t1) Buy goods\n"
            << "\t2) Answer a complaint\n"
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
            break;
        
        case 2:
            (*client).Send(2);
            complain();
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
            cout << " Press any key to go enter again...\n";
            getch();
            break;
        }
    }
}

void Employee::complain()
{
    bool Go = true;
    int ans;
    string str;

    while(Go)
    {
        system("cls");
        
        cout << "\t1) See a complaint\n";
        cout << "\t2) See a complaint anonymously\n";
        cout << "\t3) Answer a complaint\n";
        cout << "\t4) View all complaintees\n";
        cout << "\t5) View all complaints\n";
        cout << "\t6) Clear all complaints\n";
        cout << "\t7) Go back\n";
        cout << " Choose > ";
        fflush(stdin);
        cin >> ans;

        switch (ans)
        {
        case 1:
            {
                string name;
                (*client).Send(1);
                cout << " Enter the name of customer > ";
                fflush(stdin);
                getline(cin, name);
                (*client).Send(name);

                (*client).Rec(str);
                if (str == "TRUE")
                {
                    (*client).Rec(str);
                    cout << str << endl;
                }
                else
                    cerr << " \"" << name << "\" has no complaint" << endl;
                fflush(stdin);
                cin.get();
            }
            break;
        
        case 2:
            {
                string name;
                (*client).Send(2);
                cout << " Enter the name of customer > ";
                fflush(stdin);
                getline(cin, name);
                (*client).Send(name);

                (*client).Rec(str);
                if (str == "TRUE")
                {
                    (*client).Rec(str);
                    cout << str << endl;
                }
                else
                    cerr << " \"" << name << "\" has no complaint" << endl;

                fflush(stdin);
                cin.get();
            }
            break;
        
        case 3:
            {
                string name, ans;
                (*client).Send(3);
                cout << " Enter the name of customer > ";
                fflush(stdin);
                getline(cin, name);
                (*client).Send(name);

                cout << " Enter the response\n > ";
                fflush(stdin);
                getline(cin, ans);
                (*client).Send(ans);

                (*client).Rec(str);
                if (str == "TRUE")
                    cout << " The complaint has been answered\n";
                else
                    cerr << " \"" << name << "\" has no complaint" << endl;
                fflush(stdin);
                cin.get();
            }
            break;
        
        case 4:
            (*client).Send(4);
            (*client).Rec(str);
            cout << str << endl;
            fflush(stdin);
            cin.get();
            break;
        
        case 5:
            (*client).Send(5);
            (*client).Rec(str);
            cout << str << endl;
            fflush(stdin);
            cin.get();
            break;
        
        case 6:
            (*client).Send(6);
            cout << " All records are cleared\n";
            fflush(stdin);
            cin.get();
            break;
        
        case 7:
            (*client).Send(7);
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
