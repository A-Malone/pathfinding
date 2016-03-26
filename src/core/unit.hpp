#ifndef SRC_CORE_UNIT_HPP_
#define SRC_CORE_UNIT_HPP_

class Unit
{
    // Allow the world to call the getter and setters for path
    friend class World;

public:
    std::pair<int, int> pos()
    {
        return std::make_pair(m_current->x, m_current->y);
    };

    bool is_idle()
    {
        return m_path.size() == 0;
    };

    void set_path(const std::vector<Node*>& path)
    {
        m_path = path;
    };

    const std::vector<Node*>& get_path() const
    {
        return m_path;
    };

protected:
    Unit(Node* start) :
            m_current(start)
    {};

    void move(int moves)
    {
        if (!is_idle())
        {
            int num_moves = std::min(moves + 1, static_cast<int>(m_path.size()));
            for (int i = 0; i < num_moves; ++i)
            {
                Node* next = m_path[m_path.size() - i - 1];
                if (next->is_wall())
                {
                    m_path.clear();
                    break;
                }
                else
                {
                    m_current = next;
                }
            }

            if (!is_idle())
            {
                m_path.erase(m_path.end() - num_moves, m_path.end());
            }
        }
    };

private:
    Node* m_current;
    std::vector<Node*> m_path;
};

#endif /* SRC_CORE_UNIT_HPP_ */
