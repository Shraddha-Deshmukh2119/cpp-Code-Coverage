#include "Person.h"

bool Person::input()
{
    input_username(username);
    input_password(password);
    input_name(name);
    input_age(age);
    input_sex(sex);
    input_DOB(DOB, age);
    input_CNIC(CNIC);
    input_email(email);
    input_phone_num(phone_num);
    transfer();

    string error;
    (*client).Rec(error);
    if (error == "true")
        return true;
    else
        return false;
}

void Person::input_name(string& name)
{
    cout << "\n Name: ";
    getline(cin, name);
    fflush(stdin);

    if (check_name(name))
        return;

    input_name(name);
}

bool Person::check_name(const string& name)
{
    // Ensuring only letters and spaces are allowed in name
	for (const char& c : name) 
	{
		if ((c < 'A' && c != ' ') || (c > 'Z' && c < 'a') || c > 'z')
		{
			cout << " Name must only contain letters and spaces.\n";
			return false;
		}
	}
    return true;
}

void Person::input_age(int& age)
{
    cout << " Age: ";
	cin >> age;
    fflush(stdin);

    if (check_age(age))
        return;
    
    input_age(age);
}

bool Person::check_age (const int& age)
{
    if (age < 10 || age > 99)
    {	
        if (age < 0)
            cout << " Age cannot be negative.\n";
        else if (age >= 0 && age < 10)
            cout << " You must be at least 10 years old.\n";
        else
            cout << " You must be between 10 and 99 inclusive.\n";
        return false;
    }
    else
        return true;
}

void Person::input_sex(char& sex)
{
    cout << " Sex (M or F or O): ";
	cin >> sex;
    fflush(stdin);
    
    if (check_sex(sex))
        return;
    
    input_sex(sex);
}

bool Person::check_sex (const char& sex)
{
	if (sex == 'M' || sex == 'F' || sex == 'O')
        return true;
    
    cout << " Invalid input\n";
    return false;
}

void Person::input_DOB(Date& DOB, const int& myAge)
{
    cout << " Date of birth (mm/dd/yyyy): ";
	scanf("%d/%d/%d", &DOB.month, &DOB.day, &DOB.year);
    fflush(stdin);

    if (check_DOB(DOB))
        if (consistency(DOB, myAge))
            return;

	input_DOB(DOB, myAge);
}

bool Person::check_DOB (const Date& DOB)
{
     // Validation for year
    if (DOB.year > 2011 || DOB.year < 1921)
	{
		cout << " Year must be between 1921 and 2011 inclusive.\n";
        return false;
	}

     // Validation for month
	if (DOB.month > 12 || DOB.month < 1)
	{
		cout << " Month must be between 1 and 12 inclusive.\n";
		return false;
	}

     // Validation for day
	switch(DOB.month)
	{
	    case 1:
	    case 3:
	    case 5:
	    case 7:
	    case 8:
	    case 10:
	    case 12:
	    	if (DOB.day < 1 || DOB.day > 31)
	    	{
	    		cout << " Day must be between 1 and 31.\n";
		        return false;
			}
            break;

		case 4:
		case 6:
		case 9:
		case 11:
			if (DOB.day < 1 || DOB.day > 30)
	    	{
	    		cout << " Day must be between 1 and 30.\n";
                return false;
			}
            break;

		default:
             // Assuming no lunar year
			if (DOB.day < 1 || DOB.day > 28)
			{
				cout << " Day must be between 1 and 28.\n";
                return false;
			}
            break;
	}
    return true;    
}

bool Person::consistency(const Date& DOB, const int& myAge)
{
    // Determining current date/time based on current system
    // 'now' now stores the number of seconds elapsed since Jan 1, 1970
    time_t now = time(0);

    // Returns a pointer to the tm structure (defined in ctime)  
    // representing local time and date

    tm* t = localtime(&now); 
    // t->tm_mday stores day, t->tm_mon stores month (from 0 to 11) and t->tm_year stores years since 1900
   
    // Calculating age
    int age;
    if (DOB.month < (t->tm_mon + 1))
        age = (t->tm_year + 1900) - DOB.year;
    else if (DOB.month > (t->tm_mon + 1))
        age = (t->tm_year + 1900) - DOB.year - 1; // Person's birthday this year is yet to arrive
    else
    {
        if (DOB.day > t->tm_mday)
            age = (t->tm_year + 1900) - DOB.year - 1; // Person's birthday this year is yet to arrive
        else
            age = (t->tm_year + 1900) - DOB.year;
    }

    // Comparing age
    if (age == myAge)
        return true;

    cout << "Date of birth is not consistent with entered age.\n";
    return false;
}

void Person::input_CNIC(string& CNIC)
{
    cout << " CNIC: ";
    getline(cin, CNIC);
    fflush(stdin);

    if (check_CNIC(CNIC))
        return;

    input_CNIC(CNIC);
}

bool Person::check_CNIC(const string& CNIC)
{
    // Ensuring CNIC contains 15 characters
    if (CNIC.length() != 15)
    {
        cout << " CNIC must be contain 15 characters.\n";
        return false;
    }

    // Ensuring only numbers and - allowed in CNIC
	for (const char& c : CNIC) 
	{
		if ((c < '0' && c != '-') || c > '9')
		{
			cout << " CNIC must contain only numbers and -\n";
			return false;
		}
	}
    
    // Ensuring correct pattern
    for (int i = 0, l = CNIC.length(); i < l; i++)
    {
        if (i == 5 || i == 13)
        {
            if (CNIC[i] != '-')
            {
                cout << "Incorrect pattern.\n";
			    return false;
            }
        }
        else if (CNIC[i] == '-')
        {
            cout << "Incorrect pattern.\n";
            return false;
        }
    }

    return true;
}

void Person::input_email(string& email)
{
    cout << " Email: ";
    getline(cin, email);
    fflush(stdin);

    if (check_email(email))
        return;
    
    input_email(email);
}

bool Person::check_email(const string& email)
{
    bool found = false;

    // Ensuring there is an '@' in the email
    for (const char& c : email)
    {
        if (c == '@')
        {
            found = true;
            break;
        }
    }

    if(!found)
    {
        cout << " Email address must contain a '@'\n";
        return false;
    }

    return true;
}

void Person::input_phone_num(string& phone_num)
{
    cout << " Phone Number: ";
    getline(cin, phone_num);
    fflush(stdin);
    
    if (check_phone_num(phone_num))
        return;

    input_phone_num(phone_num);
}

bool Person::check_phone_num(const string& phone_num)
{
    // Ensuring only numbers, *, #, and - are allowed in phone number
	for (const char& c : phone_num) 
    {
        if ((c < '0' && c != '-' && c != '*' && c != '#') || c > '9')
		{
			cout << " Phone number can only contain numbers, *, #, and -\n";
            return false;
		}
    }
    return true;
}

inline void Person::input_username(string& username)
{
    cout << " Username: ";
    getline(cin, username);
    fflush(stdin);
}

void Person::input_password(string& password)
{
	cout << " Password: ";
    
    char c;
    password.clear();
	while (1)
	{    
    	c = getch();

        if (c == 13)     // 13 is ASCII code of Enter
            break;

		cout << "*";
		password.push_back(c);
	}

    if (check_password(password))
        return;
    
    input_password(password);
}

bool Person::check_password(const string& password)
{
	// Confirming password
	cout << "\n Confirm Password: ";

	string confirm;
	char c;
	while (1)
	{    
    	c = getch();

        if (c == 13)     // 13 is ASCII code of Enter
            break;

		cout << "*";
		confirm.push_back(c);
	}

	// Comparing the two passwords
	if (password != confirm)
	{
		cout << "\n Password must be same.\n";
        return false;
	}

    return true;
}

void Person::transfer()
{
    (*client).Send(name);
    (*client).Send(age);
    (*client).Send(sex);
    (*client).Send(DOB.month);
    (*client).Send(DOB.day);
    (*client).Send(DOB.year);
    (*client).Send(CNIC);
    (*client).Send(email);
    (*client).Send(phone_num);
    (*client).Send(username);
    (*client).Send(password);
}

void Person::login()
{
    system("cls");
    heading("LOGIN");
    cout << " Username : ";
    getline(cin, username);
    fflush(stdin);
	
	cout << " Password : ";
    char c;
    password.clear();
    while (1)
	{    
    	c = getch();

        if (c == 13)     // 13 is ASCII code of Enter
            break;

		cout << "*";
		password.push_back(c);
	}

    (*client).Send(username);
    (*client).Send(password);

    string error;
    (*client).Rec(error);

    if (error == "Correct")
    {
        home();
        return;
    }

    if (error == "Incorrect-User")
        cout << "\n Incorrect username" << endl;
    if (error == "Incorrect-Pass")
        cout << "\n Incorrect password" << endl;
    
    fflush(stdin);
    getch();
    login();
}

void Person::profile()
{
    system("cls");
    heading("PROFILE");

    string record;
    (*client).Rec(record);
    cout << record << endl;

    fflush(stdin);
    getch();
}

void Person::buy() // Displays menu and allows customers to make purchases
{
    char c;
    bool go = true;
    int itemNo, quantity, ans;
    while (go)
    {
        system("cls");
        heading("MENU");
        
        string menu, str;
        (*client).Rec(menu);
        cout << menu;
        
        cout << "\nYou want to:\n"
            << "\t1) Add items to order" << endl
            << "\t2) Confirm order" << endl
            << "\t3) Go Back" << endl;
        cout << " Your choice > ";
        cin >> ans;
        fflush(stdin);

        switch(ans)
        {
            case 1:
                (*client).Send(1);
                cout << endl << " Item number: ";
                cin >> itemNo;

                if (itemNo < 1 || itemNo > 10)
                {
                    do
                    {
                        cout << " Number must be between 1 and 10 > ";
                        cin >> itemNo;
                        fflush(stdin);
                    } while (itemNo < 1 || itemNo > 10);
                    
                }
                
                cout << " Quantity: ";
                cin >> quantity;
    
                while (true)
                {
                    if (quantity > 0)
                        break;
                    
                    cout << "Number must be positive > ";
                    cin >> quantity;
                    fflush(stdin);
                }

                (*client).Send(itemNo);
                (*client).Send(quantity);
                
                (*client).Rec(str);
                if (str == "NOT-ENOUGH")
                    cout << " Not enough quantity in stock!\n";
                getch();             
                break;

            case 2:
            {    
                (*client).Send(2);        
                (*client).Rec(str);
                if (str == "NO-ORDER") // Display error if no order has been placed
                {
                    cout << endl << "Sorry! No order has been placed yet!" << endl;
                    getch();
                    break;
                }
                else if (str.substr(0, 4) == "Your")
                {
                    cout << endl << "Sorry! You do not have enough balance!" << endl ;
                    cout << str;
                    getch();
                    break;
                }
                else
                {
                    cout << endl << "Order successfully placed!\n";
                    cout << str;
                }

                fflush(stdin);
                getch();
                break;
            }
            
            case 3:
                (*client).Send(3);
                go = false;
                break;

            default :
                cout << " Invalid choice!";
                cout << "\n Press any key to go enter again...\n";
                getch();
                break;
        }
    }
}

