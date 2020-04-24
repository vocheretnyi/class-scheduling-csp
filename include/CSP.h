#pragma once

#include <utility>
#include <vector>
#include <map>

#include "Classes.h"

using namespace std;

class Schedule;

struct Variable {
    size_t id;
    Time time;
    Group group;

    Variable(size_t id_, Time time_, Group group_)
            : id(id_), time(move(time_)), group(group_) {}

    bool operator<(const Variable& other) const {
        return id < other.id;
    }
};

//struct Domain {
//    vector<Subject> subjects;
//    vector<Teacher> teachers;
//    vector<Room> rooms;
//
//    size_t GetSize() const {
//        return subjects.size() * teachers.size() * rooms.size();
//    }
//};

struct Domain {
    Subject subject;
    Teacher teacher;
    Room room;

    Domain(Subject subject_, Teacher teacher_, Room room_) :
            subject(subject_), teacher(teacher_), room(room_) {}
};

class CSPSolver {
public:
    CSPSolver() = default;

    void SetVariables(const vector<Group>& groups, const vector<Day>& days, int LESSONS_PER_DAY);

    void SetDomains(const vector<Room>& rooms);

    Schedule Solve();

private:

    bool Backtracking(map<Variable*, Domain*>& cur, map<Variable*, Domain*>& ans);

    vector<Variable> variables;
    map<Variable*, vector<Domain>> domains;
    size_t kLESSONS_PER_DAY;
};