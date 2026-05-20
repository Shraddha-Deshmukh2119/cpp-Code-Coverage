#include "Admin.h"
#include "Goods.h"
#include "Cash.h"
#include "test_hooks.h"

void Admin::home()
{
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
            buy(adminFile);
            break;
        
        case 2:
            Employee::complain();
            break;
        
        case 3:
            profile(adminFile, this->lookup);
            break;

        case 4:
            add_employee();
            break;

        case 5:
            view(customerFile);
            break;

        case 6:
            view(empFile);
            break;

        case 7:
            stock();
            break;

        case 8:
            accounts();
            break;
        
        case 9:
            search(empFile);
            break;
        
        case 10:
            search(customerFile);
            break;
        
        case 11:
            Delete(empFile);
            break;
        
        case 12:
            Delete(customerFile);
            break;

        case 13:
            balance(customerFile);           
            break;

        case 14:
            balance(empFile);     
            break;

        // extra function;
        case 15:
        {
            string activity;
            activity = (*server).getActivity();
            (*server).Send(activity);
        }    
            break;

        case 16:
            go = false;
            (*server).endServer();
            break;

        case 17:
            go = false;
            break;
        }
    }
}

void Admin::add_employee() // Makes an employee's account
{
    if (input(empFile))
        (*server).Send("TRUE");
    else
        (*server).Send("FALSE");
}

void Admin::view(const string& fileName) // Displays records of customers or employees
{
    ifstream file(fileName); // Opening the file
    if (!file.is_open()) // Ensuring that file opens correctly
    {
        cout << " Sorry! An error occured while processing the data.\n";
        fflush(stdin);
        oshop_pause_for_tests();
        return ;
    }

    // Reading and displaying the file
    string line;
    ostringstream ss;
    while (!file.eof())
    {
        getline(file, line);
        ss << line << endl;
    }
    file.close(); // Closes the file   
    (*server).Send(ss.str());
}

void Admin::search(const string& fileName)
{
    int ans;
    bool go = true;
    string str;

    while(go)
    {   
        (*server).Rec(str);
        ans = stoi(str);
        
        switch(ans)
        {
            case 1:
                search("CNIC", fileName);
                break;

            case 2:
                search("Email", fileName);
                break;

            case 3:
                search("Phone number", fileName);
                break;

            case 4:
                search("Username", fileName);
                break;

            case 5:
                search("Password", fileName);
                break;

            case 6:
                go = false;
                break;                
        }
    }
}

void Admin::search(const string& key, const string& fileName) // Searches for the record that matches the provided detail
{
    string ans, lookup;
    bool found = false;
    (*server).Rec(ans);
    
    ifstream file(fileName); // Opening the files
    if (!file.is_open())
    {
        cout << " Sorry! An error occured while processing the data.\n";
        return;
    }

    string str;
    while (!file.eof())
    {
        getline(file, str);
        if (str.substr(0, 8) == "Username")
        {
            lookup = str;
            while(true)
            {
                if (str == (key + ": " + ans))
                {
                    found = true;
                    break;
                }
                if (str == "")
                    break;
                getline(file, str);
            }
            
            if (found)
                break;
        }
    }
    file.close();

    if (found)
    {
        (*server).Send("TRUE");
        profile(fileName, lookup);
    }
    else
        (*server).Send("FALSE");
}

void Admin::Delete(const string& fileName) // Deletes the record passed as argument
{
    string user;
    (*server).Rec(user);
    
    ifstream old_file(fileName);
    ofstream new_file("tmp.txt");
    if (!old_file.is_open() || !new_file.is_open()) // Ensuring that the files opened correctly
    {
        cout << endl << "Sorry! An error occured while processing the data.\n";
        oshop_pause_for_tests();
        return;
    }

    string line;
    user = "Username: " + user;
    while (!old_file.eof())
    {
        getline(old_file, line);
        if (line == user)
        {
            while(line != "")
                getline(old_file, line);
            continue;
        }

        new_file << line << endl;
    }
    old_file.close(); // closes the file
    new_file.close(); // closes the file
    remove(fileName.c_str()); // deletes the old file
    rename("tmp.txt", fileName.c_str()); // renames the new file to the name of the old file
}

void Admin::balance(const string& fileName) // Updates the balance of the record passed as argument
{
    string user, balance;
    bool error = false;
    (*server).Rec(user);
    (*server).Rec(balance);

    ifstream old_file(fileName);
    ofstream new_file("tmp.txt");
    if (!old_file.is_open() || !new_file.is_open()) // Ensuring that the files opened correctly
    {
        cout << endl << "Sorry! An error occured while processing the data.\nPress any key to exit.";
        return;
    }
    
    string line;
    while (!old_file.eof()) // Updating balance and copying the rest of the contents
    {
        getline(old_file, line);
        if (line == ("Username: " + user))
        {
            while(true)
            {   
                if (line == "")
                    break;
                if (line.substr(0, 7) == "Balance")
                {
                    new_file << "Balance: " << balance << endl << endl;
                    error = true;
                }
                else
                    new_file  << line << endl;

                getline(old_file, line);
            }
            continue;
        }
        
        if (line.substr(0, 7) == "Balance")
            new_file << line << endl << endl;
        else if (line != "")
            new_file << line << endl;
    }

    old_file.close(); // closes the file
    new_file.close(); // closes the file
    remove(fileName.c_str()); // deletes the old file
    rename("tmp.txt", fileName.c_str()); // renames the new file to the name of the old file
    
    if (error)
        (*server).Send("TRUE");
    else
        (*server).Send("FALSE");
}

void Admin::stock(void) // Displays details of goods and allows modification and re-ordering
{
    Goods g[GOODS_COUNT];
    ostringstream ss;
    string str;
    int ans;

    if (!initialize_goods(g))
    {
        cout << "Sorry! An error occured while processing the data.\n";
        return;
    }

    for (int i = 0; i < GOODS_COUNT; i++)
    {
        ss << "Name: " << g[i].name << endl << "Stock: " << g[i].stock << endl << "Cost: " << g[i].cost
        << endl << "Price: " << g[i].price << endl << "Reorder Level: " << g[i].reorder_level << endl <<
        "Reorder Amount: " << g[i].reorder_amount << endl << endl;
    }
    (*server).Send(ss.str());
    ss.str("");

    (*server).Rec(str);
    ans = stoi(str);

    switch (ans)
    {
        case 1:
        {
            (*server).Rec(str);
            bool b = false; // false if entered name is incorrect
            int index;

            for (int i = 0; i < GOODS_COUNT; i++)
            {
                if (g[i].name.compare(str) == 0)
                {
                    b = true;
                    index = i;
                    break;
                }
            }

            if (!b)
            {
                (*server).Send("FALSE");
                break;
            }
            else
                (*server).Send("TRUE");

            Cash csh;
            if (!initialize_cash(csh))
            {
                cout << "Sorry! An error occured while processing the data.\n";
                break;
            }
            (*server).Rec(str);
            double quantity = stod(str);

            if (csh.get_final_cash() < (quantity * g[index].cost)) // Validation
            {
                (*server).Send("FALSE");
                break;
            }
            else
                (*server).Send("TRUE");

            g[index].stock += quantity; // Increasing stock
            csh - (quantity * g[index].cost); // Decreasing cash
            if (!update_goods(g) || !update_cash(csh))
            {
                cout << "Sorry! An error occured while processing the data.\n";
                return;
            }
        }
        break;
        
        case 2:
        {
            (*server).Rec(str);
            bool b = false; // false if entered name is incorrect
            int index;

            for (int i = 0; i < GOODS_COUNT; i++)
            {
                if (g[i].name.compare(str) == 0)
                {
                    b = true;
                    index = i;
                    break;
                }
            }

            if (!b)
            {
                (*server).Send("FALSE");
                break;
            }
            else
                (*server).Send("TRUE");
            
            (*server).Rec(str);
            g[index].price = stod(str);
            (*server).Rec(str);
            g[index].reorder_level = stoi(str);
            (*server).Rec(str);
            g[index].reorder_amount = stod(str);

            if (!update_goods(g))
            {
                cout << "Sorry! An error occured while processing the data.\n";
                break;
            }
        }
        break;

        default:
            break;
    }
}

void Admin::accounts() // Displays financial details
{
    ifstream file("cash.txt"); // Opening the file
    if (!file.is_open()) // Ensuring that the file opened correctly
    {
        cout << "Sorry! An error occured while processing the data.\n";
        return;
    }

    std::string line;
    std::ostringstream ss;
    while (getline(file, line))
        ss << line << std::endl;

    (*server).Send(ss.str());
    file.close(); // Closes the file
}
