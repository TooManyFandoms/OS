#ifndef LAB6_TOPOLOGY_H
#define LAB6_TOPOLOGY_H

#include <list>
#include <stdexcept>

class Topology { 
private:
    std::list<std::list<int>> container;
     
public:
    void Insert(int id, int parent_id)
    { 
        if (parent_id == -1) { 
            std::list<int> new_list; 
            new_list.push_back(id); 
            container.push_back(new_list); 
        } else {
            int list_id = Find(parent_id); 
            if (list_id == -1) {
                throw std::runtime_error("Wrong parent id"); 
            }
            auto it1 = container.begin();
            std::advance(it1, list_id);
            for (auto it2 = it1->begin(); it2 != it1->end(); ++it2) {
                if (*it2 == parent_id) { 
                    it1->insert(++it2, id); 
                    return;
                }
            }
        }
    }

    int Find(int id)
    { 
        int cur_list_id = 0;
        for (auto it1 = container.begin(); it1 != container.end(); ++it1) {
            for (auto it2 = it1->begin(); it2 != it1->end(); ++it2) {
                if (*it2 == id) { 
                    return cur_list_id; 
                }
            }
            ++cur_list_id;
        }
        return -1; 
    }

    void Erase(int id)
    { 
        int list_id = Find(id); 
        if (list_id == -1) {
            throw std::runtime_error("Wrong id"); 
        }
        auto it1 = container.begin();
        std::advance(it1, list_id);
        for (auto it2 = it1->begin(); it2 != it1->end(); ++it2) {
            if (*it2 == id) { 
                it1->erase(it2, it1->end()); 
                if (it1->empty()) { 
                    container.erase(it1); 
                }
                return;
            }
        }
    }

    int GetFirstId(int list_id)
    { 
        auto it1 = container.begin(); 
        std::advance(it1, list_id); 
        if (it1->begin() == it1->end()) { 
            return -1; 
        }
        return *(it1->begin()); 
    }
};

#endif 
