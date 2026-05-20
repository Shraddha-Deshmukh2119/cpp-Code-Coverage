#include "Admin.h"
#include "Goods.h"
#include "Cash.h"

void Admin::home()
{
    bool go = true;
    int ans;

    while(go)
    {
        system("cls");
        heading("HOME");
        cout<< " You want to:\n"
            << "\t1) Buy goods\n"
            << "\t2) Answer complaints\n"
            << "\t3) View profile\n"
            << "\t4) Add employees\n"
            << "\t5) View customer records\n"
            << "\t6) View employee records\n"
            << "\t7) Stock\n"
            << "\t8) Accounts\n"
            << "\t9) Search in employees\n"
            << "\t10) Search in customers\n"
            << "\t11) Fire employees\n"
            << "\t12) Ban customers\n"
            << "\t13) Change Balance of Customers\n"
            << "\t14) Change Balance of Employess\n"
            << "\t15) View clients' activity\n"
            << "\t16) Close the server\n"
            << "\t17) Back\n";
        
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
            Employee::complain();
            break;
        
        case 3:
            (*client).Send(3);
            profile();
            break;

        case 4:
            (*client).Send(4);
            add_employee();
            break;

        case 5:
            (*client).Send(5);
            view("CUSTOMERS");
            break;

        case 6:
            (*client).Send(6);
            view("EMPLOYEES");
            break;

        case 7:
            (*client).Send(7);
            stock();
            break;

        case 8:
            (*client).Send(8);
            accounts();
            break;
        
        case 9:
            (*client).Send(9);
            search();
            break;
        
        case 10:
            (*client).Send(10);
            search();
            break;
        
        case 11:
            (*client).Send(11);
            Delete();
            break;
        
        case 12:
            (*client).Send(12);
            Delete();
            break;

        case 13:
            (*client).Send(13);
            balance();
            break;

        case 14:
            (*client).Send(14);
            balance();
            break;

        // extra function;
        case 15:
            (*client).Send(15);
            {
                string activity;
                system("cls");
                heading("ACTIVITY");
                (*client).Rec(activity);
                cout << activity;
            }
            fflush(stdin);
            getch();
            break;

        case 16:
            (*client).Send(16);
            go = false;
            break;

        case 17:
            (*client).Send(17);
            go = false;
            break;
    
        default:
            cout << " Invalid choice!";
            cout << "\n Press any key to go enter again...\n";
            getch();
            break;
        }
    }
}

void Admin::add_employee() // Makes an employee's account
{
    system("cls");
    heading("EMPLOYEE ACCOUNT");

    string err;
    input();
    (*client).Rec(err);

    if (err == "TRUE")
        cout << endl << "\n Employee account has been successfuly created.\n";
    else
        cerr << endl << "\n Sorry! An error occured while processing the data.\n";
    fflush(stdin);
    getch();
}

void Admin::view(const string &head) // Displays records of customers or employees
{
    system("cls");
    heading(head);
    string content;
    (*client).Rec(content);
    cout << content << endl;
    fflush(stdin);
    getch();
}

void Admin::search()
{
    int ans;
    bool go = true;

    while(go)
    {
        system("cls");
        heading("SEARCH");

        cout << endl 
            << "\t1) Search in CNIC\n"
            << "\t2) Search in email\n"
            << "\t3) Search in phone number\n" 
            << "\t4) Search in username\n" 
            << "\t5) Search in password\n" 
            << "\t6) Go Back\n" 
            << "\nYour choice > ";
        cin >> ans;
        fflush(stdin);

        switch(ans)
        {
            case 1:
                (*client).Send(1);
                search("CNIC");
                break;

            case 2:
                (*client).Send(2);
                search("Email");
                break;

            case 3:
                (*client).Send(3);
                search("Phone number");
                break;

            case 4:
                (*client).Send(4);
                search("Username");
                break;

            case 5:
                (*client).Send(5);
                search("Password");
                break;

            case 6:
                (*client).Send(6);
                go = false;
                break;
        
            default:
                cout << " Invalid choice!";
                cout << "\n Press any key to go enter again...\n";
                getch();
                break;
                
        }
    }
}

void Admin::search(const string& key) // Searches for the record that matches the provided detail
{
    system("cls");
    heading("SEARCHING");

    cout << " Enter the " << key << " of customer/employee > ";
    string ans;
    getline(cin, ans);
    (*client).Send(ans);

    (*client).Rec(ans);
    if (ans == "TRUE")
        profile();
    else
    {
        cout << " No results found\n";
        fflush(stdin);
        getch();
    }

}

void Admin::Delete() // Deletes the record passed as argument
{
    cout << " Enter the username of customer/employee > ";
    string user;
    getline(cin, user);
    (*client).Send(user);
    cout << " Account removed\n";
    fflush(stdin);
    getch();
}

void Admin::balance() // Updates the balance of the record passed as argument
{
    string user;
    double balance;

    cout << " Enter username: ";
    getline(cin, user);
    cout << " Enter new balance: $";
    cin >> balance;

    if (balance < 0) // Validation
    {
        do
        {
            cout << "Balance cannot be negative.\n Please re-enter: $";
            cin >> balance;
            fflush(stdin);
        }
        while (balance < 0);
    }

    (*client).Send(user);
    (*client).Send(balance);

    (*client).Rec(user);
    if (user == "TRUE")
        cout << " Balance updated successfully\n";
    else
        cout << " Unable to update balance\n";
    fflush(stdin);
    getch();
}

void Admin::stock(void) // Displays details of goods and allows modification and re-ordering
{
    system("cls");
    heading("STOCK REPORT");
    
    string str;
    (*client).Rec(str);
    cout << str;
        
    int ans;
    int quantity, index;
    bool go = true;
    while (go)
    {
        cout << endl
            << "\t1) Order\n"
            << "\t2) Modify\n"
            << "\t3) Exit\n"
            << " Your choice > ";
        fflush(stdin);
        cin >> ans;
        
        switch(ans)
        {
            case 1:
            {
                (*client).Send(1);
                cout << " Name of Good : ";
                fflush(stdin);
                getline(cin, str);
                (*client).Send(str);

                (*client).Rec(str);
                if (str == "FALSE")
                    cout << endl << "No such good in inventory!";
                else
                {
                    cout << endl << " Quantity to order: ";
                    cin >> quantity;
                    if (quantity <= 0)
                    {
                        do
                        {
                            cout << " Quantity must be positive"
                            << "\n Please re-enter: ";
                            fflush(stdin);
                            cin >> quantity;

                        } while(quantity <= 0);
                    }
                    (*client).Send(quantity);
                    
                    (*client).Rec(str);
                    if (str == "FALSE")
                        cout << "Not enough cash!\n";
                    else
                        cout << endl << "Order has been successfully placed!\n";
                }
            }
            go = false;
            break;
                
            case 2:
            {
                (*client).Send(2);
                cout << " Name of Good : ";
                fflush(stdin);
                getline(cin, str);
                (*client).Send(str);

                (*client).Rec(str);
                if (str == "FALSE")
                {
                    cout << endl << "No such good in inventory!";
                }
                else
                {
                    double price, amount;
                    int level;

                    cout << endl << " New price: ";
                    cin >> price;
                    if (price <= 0)
                    {
                        do
                        {
                            cout << " Price must be positive"
                            << "\n Please re-enter: ";
                            fflush(stdin);
                            cin >> price;

                        } while(price <= 0);
                    }
                    (*client).Send(price);

                    cout << endl << " New reorder level: ";
                    cin >> level;
                    if (level <= 0)
                    {
                        do
                        {
                            cout << " Price must be positive"
                            << "\n Please re-enter: ";
                            fflush(stdin);
                            cin >> level;

                        } while(level <= 0);
                    }
                    (*client).Send(level);

                    cout << endl << " New reorder amount: ";
                    cin >> amount;
                    if (amount <= 0)
                    {
                        do
                        {
                            cout << " Price must be positive"
                            << "\n Please re-enter: ";
                            fflush(stdin);
                            cin >> amount;

                        } while(amount <= 0);
                    }
                    (*client).Send(amount);

                    cout << endl << "Changes successfully made!";
                }
            }
            go = false;
            break;

            case 3:
                go = false;
                break;
                
            default:
                cout << "\n Invalid choice!\n";
                getch();
        }
        fflush(stdin);
        getch();
    }
}

void Admin::accounts() // Displays financial details
{
    system("cls");
    heading("FINANCIAL REPORT");

    string contents;
    (*client).Rec(contents);
    cout << contents << endl;
    fflush(stdin);
    getch();
}
