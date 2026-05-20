#include "Admin.h"
#include "Customer.h"
#include "Employee.h"
#include "Server.h"
using namespace std;

void PROGRAM(Thread&);

int main()
{
    Server server(PROGRAM);

    if (server.start())
    {
        system("cls");
        cout << " Server started\n";
        server.run();
    }

    cout << " Server closed\n";
    return 0;
}

void PROGRAM(Thread &server)
{
    string str;
    bool go = true;

    while(go)
    {        
        server.Rec(str);
        int ans = stoi(str);

        switch(ans)
        {
            case 1:
            {
                Admin admin(server);
                admin.login(adminFile);
                break;
            }
            case 2:
            {
                Employee employee(server);
                employee.login(empFile);
                break;
            }
            case 3:
            {
                Customer customer(server);
                customer.start();
                break;
            }
            case 4:
                go = false;
                break;
        }
    }
}