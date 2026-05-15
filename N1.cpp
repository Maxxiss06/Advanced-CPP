#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <sstream>

class Group;

class User {
    int id;
    std::string name;
    std::string email;
    std::weak_ptr<Group> group;
public:
    User(int uid, const std::string& uname, const std::string& uemail)
        : id(uid), name(uname), email(uemail) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }

    void setGroup(std::shared_ptr<Group> g);
    std::shared_ptr<Group> getGroup() const;
    void detachGroup();
    void print() const;
};

class Group : public std::enable_shared_from_this<Group> {
    int id;
    std::vector<std::shared_ptr<User>> users;
public:
    Group(int gid) : id(gid) {}
    int getId() const { return id; }
    void addUser(std::shared_ptr<User> user);
    void removeUser(int userId);
    void print() const;
    const std::vector<std::shared_ptr<User>>& getUsers() const { return users; }
};

void User::setGroup(std::shared_ptr<Group> g) { group = g; }
std::shared_ptr<Group> User::getGroup() const { return group.lock(); }
void User::detachGroup() { group.reset(); }
void User::print() const {
    std::cout << "User: id=" << id << ", name=" << name << ", email=" << email;
    auto g = group.lock();
    if (g) std::cout << ", groupId=" << g->getId();
    else std::cout << ", groupId=none";
    std::cout << std::endl;
}

void Group::addUser(std::shared_ptr<User> user) {
    if (!user) return;
    auto old = user->getGroup();
    if (old && old.get() != this) old->removeUser(user->getId());
    user->setGroup(shared_from_this());
    users.push_back(user);
}
void Group::removeUser(int uid) {
    auto it = std::remove_if(users.begin(), users.end(),
        [uid](auto& u) { return u && u->getId() == uid; });
    if (it != users.end()) {
        (*it)->detachGroup();
        users.erase(it, users.end());
    }
}
void Group::print() const {
    std::cout << "Group: id=" << id << ", members: ";
    if (users.empty()) std::cout << "none";
    else for (size_t i = 0; i < users.size(); ++i)
        std::cout << users[i]->getName() << "(" << users[i]->getId() << ")"
                  << (i+1 < users.size() ? ", " : "");
    std::cout << std::endl;
}

std::unordered_map<int, std::shared_ptr<User>> usersMap;
std::unordered_map<int, std::shared_ptr<Group>> groupsMap;

void createUser(int id, const std::string& name, const std::string& email) {
    if (usersMap.count(id)) {
        std::cout << "Error: user with id " << id << " already exists" << std::endl;
        return;
    }
    usersMap[id] = std::make_shared<User>(id, name, email);
    std::cout << "User created: " << name << " (id=" << id << ")" << std::endl;
}

void deleteUser(int id) {
    if (!usersMap.count(id)) {
        std::cout << "Error: user " << id << " not found" << std::endl;
        return;
    }
    if (auto g = usersMap[id]->getGroup()) g->removeUser(id);
    usersMap.erase(id);
    std::cout << "User " << id << " deleted" << std::endl;
}

void allUsers() {
    if (usersMap.empty()) std::cout << "No users" << std::endl;
    else for (auto& p : usersMap) p.second->print();
}

void getUser(int id) {
    if (!usersMap.count(id)) std::cout << "User " << id << " not found" << std::endl;
    else usersMap[id]->print();
}

void createGroup(int id) {
    if (groupsMap.count(id)) {
        std::cout << "Error: group with id " << id << " already exists" << std::endl;
        return;
    }
    groupsMap[id] = std::make_shared<Group>(id);
    std::cout << "Group created: id=" << id << std::endl;
}

void deleteGroup(int id) {
    if (!groupsMap.count(id)) {
        std::cout << "Error: group " << id << " not found" << std::endl;
        return;
    }
    for (auto& u : groupsMap[id]->getUsers()) if (u) u->detachGroup();
    groupsMap.erase(id);
    std::cout << "Group " << id << " deleted" << std::endl;
}

void allGroups() {
    if (groupsMap.empty()) std::cout << "No groups" << std::endl;
    else for (auto& p : groupsMap) p.second->print();
}

void getGroup(int id) {
    if (!groupsMap.count(id)) std::cout << "Group " << id << " not found" << std::endl;
    else groupsMap[id]->print();
}

void addUserToGroup(int uid, int gid) {
    if (!usersMap.count(uid)) { std::cout << "User not found\n"; return; }
    if (!groupsMap.count(gid)) { std::cout << "Group not found\n"; return; }
    groupsMap[gid]->addUser(usersMap[uid]);
    std::cout << "User " << uid << " added to group " << gid << std::endl;
}

int main() {
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "exit") break;
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "createUser") {
            int id; std::string name, email;
            if (iss >> id >> name >> email) createUser(id, name, email);
            else std::cout << "Usage: createUser {id} {name} {email}" << std::endl;
        }
        else if (cmd == "deleteUser") {
            int id; if (iss >> id) deleteUser(id);
            else std::cout << "Usage: deleteUser {id}" << std::endl;
        }
        else if (cmd == "allUsers") allUsers();
        else if (cmd == "getUser") {
            int id; if (iss >> id) getUser(id);
            else std::cout << "Usage: getUser {id}" << std::endl;
        }
        else if (cmd == "createGroup") {
            int id; if (iss >> id) createGroup(id);
            else std::cout << "Usage: createGroup {id}" << std::endl;
        }
        else if (cmd == "deleteGroup") {
            int id; if (iss >> id) deleteGroup(id);
            else std::cout << "Usage: deleteGroup {id}" << std::endl;
        }
        else if (cmd == "allGroups") allGroups();
        else if (cmd == "getGroup") {
            int id; if (iss >> id) getGroup(id);
            else std::cout << "Usage: getGroup {id}" << std::endl;
        }
        else if (cmd == "addUserToGroup") {
            int uid, gid;
            if (iss >> uid >> gid) addUserToGroup(uid, gid);
            else std::cout << "Usage: addUserToGroup {userId} {groupId}" << std::endl;
        }
        else if (cmd.empty() || cmd == "\n") continue;
        else std::cout << "Unknown command: " << cmd << std::endl;
    }
    return 0;
}