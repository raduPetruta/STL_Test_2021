#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

std::ifstream fi("input.txt");
std::ofstream fo("output.txt");

int nrDays, nrEventsPlaned, nrEventsToBePlaned;

struct eventsA {
    std::string id;
    int day, startHour, nrHours;
};
struct eventsB {
    std::string id;
    int allocatedHours, priority;
    bool flag = true;
};
struct available {
    int dayIndex, startIndex, endIndex;     
    bool usable = true;
};
struct rez {
    std::string id;
    int day, start;
};

std::vector<eventsA> events;
std::vector<eventsB> eventsToBe;
std::vector<rez> Result;

void parseInput() {
     
    //get input
    fi >> nrDays; fi >> nrEventsPlaned;
    for (int i = 0; i < nrEventsPlaned; i++) {
        eventsA EVENTS;
        fi >> EVENTS.id >> EVENTS.day >> EVENTS.startHour >> EVENTS.nrHours;
        events.push_back(EVENTS);
    }
    //add unused events to bound the days
    for (int dayIt = 1; dayIt <= nrDays; dayIt++) {
        events.push_back({ "", dayIt, 9, 0 }); events.push_back({ "", dayIt, 17, 0 });
    }
    
    //sort the events by day and hours, with the first "fake event" and the last "fake event" (starting at 9 && 17)
    sort(events.begin(), events.end(), [](eventsA A, eventsA B) {
        if (A.day < B.day)
            return true;
        else if (A.day > B.day)
            return false;

        if (A.startHour < B.startHour)
            return true;
        else if (A.startHour > B.startHour)
            return false;

        return A.nrHours < B.nrHours;
    });

    fi >> nrEventsToBePlaned;
    //get the upcoming events
    for (int i = 1; i <= nrEventsToBePlaned; i++) {
        eventsB EVENTS;
        fi >> EVENTS.id >> EVENTS.allocatedHours >> EVENTS.priority;
        eventsToBe.push_back(EVENTS);
    }
  
}
void solveA() {

    int firstDuration = eventsToBe[0].allocatedHours;
    eventsA current = events[0];

    for (int i = 1; i < events.size(); i++) {

        //calculate available time between two events
        int  freeTime = events[i].startHour - (events[i - 1].startHour + events[i - 1].nrHours);

        //this condition is used for the "empty cases" because some of them give a negative result and we don't need it
        if (freeTime > 0) {
            if (freeTime == firstDuration) {
                //return the curent event day and available time because the first event will be placed before the current one
                std::cout << events[i].day << " ";
                std::cout << events[i].startHour - freeTime;
                break;
            }
        }
    }
    std::cout << std::endl;
}

void solveB() {

    int count = 0;

    //sort the upcoming events by their priority and by their duration
    sort(eventsToBe.begin(), eventsToBe.end(), [](eventsB A, eventsB B) {
        if (A.priority > B.priority)
            return true;
        else if(A.priority < B.priority)
            return false;
        return A.allocatedHours < B.allocatedHours;
    });

    //a vector for available intervals of hours that we can place an event in
    std::vector<available> Available;

    for (int i = 1; i < events.size(); i++) {
        int  freeTime = events[i].startHour - (events[i - 1].startHour + events[i - 1].nrHours);
        if (freeTime > 0) 
            Available.push_back({ events[i].day, events[i].startHour - freeTime, events[i].startHour});
    }
    
    for (int i = 0; i < Available.size(); i++) {
        
        int freeTime = Available[i].endIndex - Available[i].startIndex;
        for (int j = 0; j < eventsToBe.size(); j++) {
            
            // if the freeTime == allocated hours for the upcoming event && the event has not already been used and the Available spot has not already been used
            if (freeTime == eventsToBe[j].allocatedHours && eventsToBe[j].flag == true && Available[i].usable == true) {
                Available[i].usable = false;
                eventsToBe[j].flag = false;
                Result.push_back({ eventsToBe[j].id , Available[i].dayIndex, Available[i].startIndex });

                count++;
            }
        }
    }
    
    std::cout << count << std::endl;
    for (int i = 0; i < Result.size(); i++)
        std::cout << Result[i].id << " " << Result[i].day << " " << Result[i].start << '\n';
}

int main(){

    parseInput(); 
    fi.close();

    solveA(); 
    solveB();
    fo.close();
    
    return 0;
}
