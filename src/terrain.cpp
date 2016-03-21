#include "terrain.hpp"

Terrain::Terrain(int w, int h)
{
    m_width = w;
    m_height = h;

    for (int x = 0; x < w; ++x)
    {
        std::vector<Node*> col;
        col.reserve(h);
        for (int y = 0; y < h; ++y)
        {
            col.push_back(new Node(x,y));
        }
        m_map.push_back(col);
    }

	auto blobs_gen = std::bind(
				std::normal_distribution<float>(10.0,3.0),
				std::knuth_b(std::knuth_b(std::chrono::system_clock::now().time_since_epoch().count()))
	);

    auto rand_x = std::bind(
    		std::uniform_int_distribution<int>(0,w-1),
			std::knuth_b(std::knuth_b(std::chrono::system_clock::now().time_since_epoch().count()))
	);

    auto rand_y = std::bind(
    		std::uniform_int_distribution<int>(0,h-1),
    		std::knuth_b(std::knuth_b(std::chrono::system_clock::now().time_since_epoch().count()))
	);

    auto rand_r = std::bind(
    		std::normal_distribution<float>(4.0,2.0),
			std::knuth_b(std::knuth_b(std::chrono::system_clock::now().time_since_epoch().count()))
	);

    int num_blobs = blobs_gen();

    for(int i = 0; i < num_blobs; ++i)
    {
    	create_obstacle(rand_x(), rand_y(), rand_r());
    }
}

Terrain::~Terrain()
{
    m_map.clear();
}

bool Terrain::is_valid(int x, int y) const
{
	if (x >= m_width || x < 0) {return false;}
	if (y >= m_height || y < 0) {return false;}
	return true;
}

Node* Terrain::at(int x, int y) const
{
    return m_map.at(x).at(y);
}

Node* Terrain::at(std::pair<int,int> p) const
{
    return m_map.at(p.first).at(p.second);
}

std::vector<Node*> Terrain::neighbours(Node* node) const
{
    std::vector<Node*> nodes;
    for (int x = -1; x <= 1; ++x)
    {
    	for (int y = -1; y <= 1; ++y)
    	{
    		if (is_valid(node->x + x, node->y + y) && !(x == y && x == 0))
    		{
    			Node* new_node = at(node->x + x,node->y + y);
    			if (!new_node->wall)
    			{
    				nodes.push_back(new_node);
    			}
    		}
    	}
    }
    return nodes;
}


void Terrain::create_obstacle(int px, int py, float r)
{
	for (int x = px-r; x <= px+r; ++x)
	{
		for (int y = py-r; y <= py+r; ++y)
		{
			if (is_valid(x,y) && sqrt(std::pow(px-x,2) + std::pow(py-y,2)) < r)
			{
				at(x,y)->wall = true;
			}
		}
	}

}

