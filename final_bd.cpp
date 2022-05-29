#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <iomanip>
#include <exception>
#include <system_error>
#include <numeric>

using namespace std;

/*class exception {
    exception(string new_exc) {
        exc = new_exc;
    }
    string exc;
};*/

class Date {
public:
    Date() {
        year = 0;
        month = 0;
        day = 0;
    }
    Date(int yr, int mth, int dy) {
        year = yr;
        month = mth;
        day = dy;
    }
    int GetYear() const {
        return year;
    };
    int GetMonth() const {
        return month;
    };
    int GetDay() const {
        return day;
    };
private:
    int year;
    int month;
    int day;
};

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() != rhs.GetYear()) {
        return lhs.GetYear() < rhs.GetYear();
    }
    else if (lhs.GetMonth() == rhs.GetMonth()) {
        return lhs.GetDay() < rhs.GetDay();
    }
    return lhs.GetMonth() < rhs.GetMonth();
};

void ChekForDate(int& m, int& d) {
    if (m < 1 || m > 12) {
        //string error = "Month value is invalid: " + to_string(m);
        throw invalid_argument("Month value is invalid: " + to_string(m));
    }
    else if (d < 1 || d > 31) {
        throw invalid_argument("Day value is invalid: " + to_string(d));
    }
}

istream& operator >>(istream& stream, Date& date) {
    int new_year = 0, new_month = 0, new_day = 0;
    stream >> new_year;
    if (stream.peek() == '-') { stream.ignore(); }
    stream >> new_month;
    if (stream.peek() == '-') { stream.ignore(); }
    stream >> new_day;
    if (!stream >> new_year || !stream >> new_month || !stream >> new_day || !stream.eof()) {
        throw logic_error("Wrong date format: ");
    }
    try {
        ChekForDate(new_month, new_day);
    }
    catch (invalid_argument& ex) {
        cout << ex.what();
        exit(1);
    }
    date = { new_year, new_month, new_day };
    return stream;
}

void FindEv(const map<Date, set<string>>& dtbs, const Date& date) {
    try {
        for (const string& ev : dtbs.at(date)) {
            cout << ev << endl;
        }
    }
    catch (out_of_range const&) {

    }
}

void PrintAllEv(const map<Date, set<string>>& dtbs) {
    for (const auto& all : dtbs) {
        if (!all.second.empty()) {
            for (const string& ev : all.second) {
            cout << setfill('0') << setw(4) << all.first.GetYear()
                << "-" << setw(2) << all.first.GetMonth()
                << "-" << setw(2) << all.first.GetDay() << " ";
            
                cout << ev << endl;
            } //cout << endl;
        }

    }
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        dtbs[date].insert(event);
    };
    bool DeleteEvent(const Date& date, const string& event) {
        //for (const string& ev : dtbs[date]) {
        try {
            if (!dtbs.at(date).empty()) {
                if (dtbs[date].count(event) != 0) {
                    dtbs[date].erase(event);
                    cout << "Deleted successfully" << endl;
                    return true;
                }
            }
        }
        catch (out_of_range const&) {

        }
        cout << "Event not found" << endl;
        return false;
    };
    int  DeleteDate(const Date& date) {
        int count = 0;
        for (unsigned long i = 0; i < dtbs[date].size(); ++i) {
            count++;
        }
        dtbs[date].clear();
        cout << "Deleted " + to_string(count) + " events" << endl;
        return count;
    };

    /* ??? */ void Find(const Date& date) const {
        //if(dtbs.at(date).empty())
        FindEv(dtbs, date);
    };

    void Print() const {
        PrintAllEv(dtbs);
    };
private:
    map< Date, set<string>> dtbs;
};

void CheckCommand(const string& cmd) {
    if (cmd != "Del" && cmd != "" && cmd != "Find" && cmd != "Add" && cmd != "Print") {
        cout << "Unknown command: " + cmd;
        exit(1);
    }
}

int main()
{
    Database db;

    string command;
    while (getline(cin, command)) {
        stringstream ss;
        ss << command;
        string cmd;
        Date dt;
        string event;
        ss >> cmd;
        ss.ignore(1);
        CheckCommand(cmd);
        if (cmd != "") {
            if (cmd != "Print") {
                stringstream ss2;
                string entered_date;
                ss >> entered_date;
                ss.ignore(1);
                ss2.str(entered_date);
                try {
                    ss2 >> dt;
                }
                catch (logic_error& ex) {
                    cout << ex.what() << ss2.str();
                    exit(1);
                }
                ss >> event;
            }
            if (cmd == "Add") {
                db.AddEvent(dt, event);
            }
            else if ((cmd == "Del") && (!event.empty())) {
                db.DeleteEvent(dt, event);
            }
            else if (cmd == "Del") {
                db.DeleteDate(dt);
            }
            else if (cmd == "Find") {
                db.Find(dt);
            }
            else if (cmd == "Print") {
                db.Print();
            }
            // —читайте команды с потока ввода и обработайте каждую
        }
    }


    return 0;
}

