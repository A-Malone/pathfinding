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
        return m_path.size() > 0;
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
            for (int i = 0; i < moves; ++i)
            {
                if (m_path[i]->is_wall())
                {
                    m_path.clear();
                    break;
                }
                else
                {
                    m_current = m_path[i];
                }
            }

            if (!is_idle())
            {
                m_path.erase(m_path.begin(), m_path.begin() + moves);
            }
        }
    };

private:
    Node* m_current;
    std::vector<Node*> m_path;
};

#endif /* SRC_CORE_UNIT_HPP_ */
