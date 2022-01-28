#include <bits/stdc++.h>
using namespace std;

float MAX_DISTANCE_FOR_SEARCH = 5;
// maximum distance bitween user and driver to look for

struct Location
{
    int x;
    int y;
    Location()
    {
        x = -1;
        y = -1;
    }
    Location(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
};

class User
{

private:
    // considering name as primary key(There Is No 2 or more users with the same name) other wise we can use unique usernames/mobile/email
    string name;
    char gender;
    int age;
    // Location location;

public:
    Location location;
    User(string _name, char _gender, int _age)
    {
        this->name = _name;
        this->gender = _gender;
        this->age = _age;
    }

    User(string _name, char _gender, int _age, int _x, int _y)
    {
        this->name = _name;
        this->gender = _gender;
        this->age = _age;
        this->location = Location(_x, _y);
    }

    void update_Location(int _x, int _y)
    {
        location.x = _x;
        location.y = _y;
    }

    void update_Location(Location _location)
    {
        location = _location;
    }

    string get_name()
    {
        return name;
    }

    Location get_location()
    {
        return location;
    }
};

class Driver : public User
{
private:
    string vehicale_name;
    bool availablity_status;

public:
    Driver(string _name, char _gender, int _age, int _x, int _y, string _vehicale_name) : User(_name, _gender, _age, _x, _y)
    {
        this->vehicale_name = _vehicale_name;
        this->availablity_status = true;
    }

    bool is_available()
    {
        return this->availablity_status;
    }

    float get_distance(Location _location)
    {
        float x1 = _location.x;
        float y1 = _location.y;
        float x2 = this->location.x;
        float y2 = this->location.y;
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }

    void change_driver_status()
    {
        availablity_status = !availablity_status;
    }
};

User *add_user(string _name, char _gender, int _age)
{
    User *user = new User(_name, _gender, _age);
    return user;
}

Driver *add_driver(string _name, char _gender, int _age, int _x, int _y, string _vehicale_name)
{
    Driver *driver = new Driver(_name, _gender, _age, _x, _y, _vehicale_name);
    return driver;
}

vector<Driver *> Find_Cabs_Near_Me(Location location, map<string, Driver *> &drivers)
{
    vector<Driver *> drivers_near_user;

    for (auto it = drivers.begin(); it != drivers.end(); it++)
    {
        Driver *driver = it->second;
        if (driver->is_available())
        {
            float distance = driver->get_distance(location);
            if (distance <= MAX_DISTANCE_FOR_SEARCH)
            {
                drivers_near_user.push_back(driver);
            }
        }
    }
    return drivers_near_user;
}

void choose_ride(string user, string driver, map<string, string> &In_Process_Ride)
{
    In_Process_Ride[driver] = user;
}


float calculatecost(Location destination,Location user_location, Location driver_location)
{
    float x1 = destination.x;
    float x2 = user_location.x;
    float x3 = driver_location.x;
    float y1 = destination.y;
    float y2 = user_location.y;
    float y3 = driver_location.y;

    float cost_of_reching_user_location = sqrt((x2-x3)*(x2-x3) + (y2-y3)*(y2-y3));
    float cost_of_reching_destination = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    return cost_of_reching_user_location + cost_of_reching_destination;
    // considering cost as 1km/rs
}

int main()
{
    // considering map as database and their name as primary key
    map<string, User *> users;
    map<string, Driver *> drivers;
    map<string, string> In_Process_Ride;
    // key : driver value: user
    // this will be used for concurrent ride searches
    // ans key value pairs is string because we can use it in feture data history
    map<string,map<string,float> > earning;
    // {user name { date : earning of that day }}
    // here i will store everdays earning so i have used key as year_month_date 

    time_t now = time(0);
    tm *ltm = localtime(&now);
    string key = to_string(ltm->tm_year) + to_string(ltm->tm_mon) + to_string(ltm->tm_mday);

    while (true)
    {
        cout << "Enter 1 for add user" << endl;
        cout << "Enter 2 for add driver" << endl;
        cout << "Enter 3 for find ride" << endl;
        cout << "Enter 4 for change status of driver" << endl;
        cout << "Enter 5 for update Location" << endl;
        cout << "Enter 6 for Showing Earnings For Perticular Driver" << endl;
        cout << "Enter 7 for Showing All Drivers Earnings" << endl;
        string str;
        cin >> str;
        if(str.size()>1 or str[0]<'0' or str[0]>'9')
        {
            cout << "Select Valid Number" << endl;
            continue;
        }
        int input=stoi(str);
        if (input == 1)
        {
            string name;
            int age;
            char gender;
            cout << "Enter name age and gender" << endl;
            cin >> name >> age >> gender;
            if (users.find(name) != users.end())
            {
                cout << "user already exists" << endl;
                continue;
            }
            User *user = add_user(name, age, gender);
            users[name] = user;
        }
        else if (input == 2)
        {
            string name;
            int age;
            char gender;
            string vehicale_name;
            int x, y;
            cout << "Enter name age gender vehiclename(carname_numberplate)(use this format) and location" << endl;
            cin >> name >> age >> gender >> vehicale_name >> x >> y;
            if (drivers.find(name) != drivers.end())
            {
                cout << "Driver with This Name already exist" << endl;
                continue;
            }
            Driver *driver = add_driver(name, gender, age, x, y, vehicale_name);
            drivers[name] = driver;
        }
        else if (input == 3)
        {
            // here i have considered that user will get ride from their location so not taking source as input
            cout << "Enter Your Name" << endl;
            string name;
            cin >> name;
            if (users.find(name) == users.end())
            {
                cout << "Please register" << endl;
                continue;
            }
            User *user = users[name];

            vector<Driver *> cabs = Find_Cabs_Near_Me(user->location, drivers);

            if (cabs.size() == 0)
            {
                cout << "Soory No driver is available" << endl;
            }
            else
            {
                cout << "There Are Total : " << cabs.size() << " drivers availabel" << endl;
                cout << "choose any one" << endl;
                for (int i = 0; i < cabs.size(); i++)
                {
                    Location location = cabs[i]->location;
                    cout << "option : " << i << " " << cabs[i]->get_name() << " At Location : " << location.x << " " << location.y << endl;
                }
                cout << "select Option : ";
                int option;
                cin >> option;
                if (option < 0 or option > cabs.size() - 1)
                {
                    cout << "invalid option" << endl;
                    continue;
                }
                Location location;
                cout << "Enter Your Location" << endl;
                cin >> location.x >> location.y;
                User *user = users[name];
                Driver *driver = cabs[option];
                choose_ride(name, driver->get_name(), In_Process_Ride);
                // cout << "press 1 if ride is canceled" << endl;
                // we can use above statement to cancel ride and then we cann call funcation cancel(rider,user) to delete entry from in progress ride list.
                // and we can use another database to log that data
                cout << "ride is started" << endl;
                
                float cost = calculatecost(location, user->get_location(), driver->get_location());
                cout << "Total cost is : " << cost << endl;
                In_Process_Ride.erase(driver->get_name());
                user->update_Location(location);
                driver->update_Location(location);
                earning[driver->get_name()][key] += cost;
                // add money on current days earnings 
                cout << "Thanks !!! Ride Is Ended" << endl;
            }

            // here i have
        }
        else if (input == 4)
        {
            string driver_name;
            cout << "Enter driver name" << endl;
            cin >> driver_name;
            if(drivers.find(driver_name)==drivers.end()){
                cout << "No Driver Found" << endl;
            }
            else
            {
                drivers[driver_name]->change_driver_status();
            }

        }
        else if (input == 5)
        {
            cout << "choose 1 for user and 2 for driver";
            int choise;
            cin >> choise;
            string name;
            cout << "Enter name" << endl;
            cin >> name;
            if(choise==1)
            {
                if(users.find(name) == users.end())
                {
                    cout << "Name Not Found" << endl;
                }
                else
                {
                    Location location;
                    cout << "Enter Location " << endl;
                    cin >> location.x >> location.y ;
                    users[name]->update_Location(location);
                }
            }
            else
            {
                if (drivers.find(name) == drivers.end())
                {
                    cout << "Name Not Found" << endl;
                }
                else
                {
                    Location location;
                    cout << "Enter Location " << endl;
                    cin >> location.x >> location.y;
                    drivers[name]->update_Location(location);
                }
            }
        }
        else if(input == 6)
        {
            string name;
            cout << "Enter name" << endl;
            cin >> name;
            if(drivers.find(name) == drivers.end())
            {
                cout << "No Driver With This Name Found" << endl;
                continue;
            }
            else
            {
                cout << name << " " << key << " " << earning[name][key] << endl;
                cout << earning[name][key] << endl;
            }
        }
        else if(input == 7)
        {
            for(auto it = drivers.begin(); it != drivers.end();it++)
            {
                string name=it->first;
                cout << "Driver " << name << "Have Earned : " << earning[name][key] << "$" << endl;

            }
        }
        else
        {
            cout << "invalid option" << endl;
        }
    }
}