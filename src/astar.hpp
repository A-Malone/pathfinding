#include "common.hpp"

#include "solver.hpp"

class AStar : Solver
{
public:
    std::vector<Node*> get_path
    (
        const Terrain& terrain,
        std::pair<int,int> p1,
        std::pair<int,int> p2
    );

protected:
    int heuristic
    (
        std::pair<int,int> p1,
        std::pair<int,int> p2
    );

    std::vector<Node*> reconstruct_path
    (
        std::unordered_map<Node*,Node*> came_from,
        Node* end
    );
};

template <class A, class B, class Compare>
class CompareMap : public std::unordered_map<A,B>
{
private:
    Compare c;

public:
    explicit CompareMap(Compare c_ = Compare()) 
        :std::unordered_map<A,B>()
        ,c(c_)
        {};

    bool operator()(A a, A b)
    {
        return c(this->at(a), this->at(b));
    };
};
