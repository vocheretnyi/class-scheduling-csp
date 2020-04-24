#include "CSP.h"

#include <random>
#include <algorithm>
#include <iostream>

#include "Schedule.h"

void CSPSolver::SetVariables(const vector<Group>& groups, const vector<Day>& days, int LESSONS_PER_DAY) {
    kLESSONS_PER_DAY = LESSONS_PER_DAY;
    size_t num = 0;
    for (const Day& day : days) {
        for (size_t lesson = 1; lesson <= LESSONS_PER_DAY; ++lesson) {
            for (const Group& group : groups) {
                variables.push_back(Variable{num, Time{day, lesson}, group});
                ++num;
            }
        }
    }
}

void CSPSolver::SetDomains(const vector<Room>& rooms) {
    for (const Variable& variable : variables) {
        vector<Domain> variableDomains;
        for (const Subject& subject : variable.group.subjects) {
            for (const Teacher& teacher : subject.teachers) {
                for (const Room& room : rooms) {
                    variableDomains.emplace_back(subject, teacher, room);
                }
            }
        }
        domains.insert(make_pair((Variable* const)&variable, move(variableDomains)));
    }
}

Schedule ConvertToSchedule(map<Variable*, Domain*>& cur) {
    Schedule schedule;

    for (const auto& [variable, domain] : cur) {
        size_t num = variable->time.number;
        size_t sz = schedule.schedule[variable->group][variable->time.day].size();
        schedule.schedule[variable->group][variable->time.day].resize(max(sz, num));
        schedule.schedule[variable->group][variable->time.day][num - 1] = Class(variable->group,
                domain->subject,
                domain->room,
                domain->teacher,
                variable->time);
    }

    return schedule;
}

Schedule CSPSolver::Solve() {
    map<Variable*, Domain*> cur, ans;
    Backtracking(cur, ans);

    return ConvertToSchedule(ans);
}

// TODO(Improvement): variables is a list of unassigned variables
Variable* SelectUnassignedVariable(map<Variable*, Domain*>& cur, vector<Variable>& variables) {
//    shuffle(variables.begin(), variables.end(), mt19937(random_device()()));
    for (Variable& variable : variables) {
        if (!cur.count(&variable)) {
            return &variable;
        }
    }
    return nullptr;
}

vector<Domain>& OrderDomainValues(vector<Domain>& domains) {
    shuffle(domains.begin(), domains.end(), mt19937(random_device()()));
    return domains;
}

bool check_constraints(map<Variable*, Domain*>& cur) {
    for (const auto& [variable1, domain1] : cur) {
        for (const auto& [variable2, domain2] : cur) { // TODO(Improvement)
            if (variable1->time == variable2->time && variable1 != variable2) {
                if (domain1->teacher == domain2->teacher) {
                    return false;
                }
                if (domain1->room == domain2->room) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool CSPSolver::Backtracking(map<Variable*, Domain*>& cur, map<Variable*, Domain*>& ans) {
//    cout << "here\n";
    if (cur.size() == variables.size()) {
        ans = cur;
        return true;
    }

    Variable* variable = SelectUnassignedVariable(cur, variables);

    for (Domain& domain : OrderDomainValues(domains[variable])) { // TODO: OrderDomainValues
        cur[variable] = &domain;
        if (check_constraints(cur)) {
            bool result = Backtracking(cur, ans);
            if (result) {
                return true;
            }
        }
        cur.erase(variable);
    }
    return false;
}
