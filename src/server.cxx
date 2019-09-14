#include <attack_logger.hxx>

Server::Server(const std::string & oName)
    : IServer()
    , name(oName)
    , locker()
{
    name += std::string(18 - name.size(), ' ');
}

Server::~Server()
{}

const std::string & Server::Name() const
{
    return name;
}

/*
 *
 */

ServerWithMap::ServerWithMap()
    : Server("std::map")
    , attack_map()
{}

ServerWithMap::~ServerWithMap()
{}

void ServerWithMap::GetTopN(size_t N, result_t & result)
{
    result.clear();
    size_t                      count = 0;
    std::lock_guard<std::mutex> lock(locker);
    for (auto it = attack_map.crbegin(); it != attack_map.crend(); ++it)
    {
        result.push_back(it->second);

        if (N == ++count)
        {
            break;
        }
    }
}

size_t ServerWithMap::Count()
{
    std::lock_guard<std::mutex> lock(locker);
    return attack_map.size();
}

bool ServerWithMap::ExistAndIterate(size_t nRank)
{
    container_t::iterator it = attack_map.find(nRank);
    if (it != attack_map.end())
    {
        ++(it->second);
        return true;
    }
    return false;
}

void ServerWithMap::AttackMe(size_t nRank, const std::string & oDescription)
{
    std::lock_guard<std::mutex> lock(locker);
    if (!ExistAndIterate(nRank))
    {
        attack_map.emplace(nRank, Attack(nRank, oDescription));
    }
}

/*
 *
 */

ServerWithUnorderedMap::ServerWithUnorderedMap()
    : Server("std::unordered_map")
    , attack_map()
{}

ServerWithUnorderedMap::~ServerWithUnorderedMap()
{}

void ServerWithUnorderedMap::GetTopN(size_t N, result_t & result)
{
    std::vector<element_t> copy;

    {
        std::lock_guard<std::mutex> lock(locker);
        copy = std::vector<element_t>(attack_map.cbegin(), attack_map.cend());
    }

    size_t middle = N;
    if (middle > copy.size())
    {
        middle = copy.size();
    }

    std::partial_sort(copy.begin(), copy.begin() + middle, copy.end(),
                      [](const element_t a, const element_t b)
    {
        return a.second > b.second;
    });

    result.clear();
    size_t count = 0;
    for (const auto & it : copy)
    {
        result.push_back(it.second);

        if (N == ++count)
        {
            break;
        }
    }
}

size_t ServerWithUnorderedMap::Count()
{
    std::lock_guard<std::mutex> lock(locker);
    return attack_map.size();
}

bool ServerWithUnorderedMap::ExistAndIterate(size_t nRank)
{
    container_t::iterator it = attack_map.find(nRank);
    if (it != attack_map.end())
    {
        ++(it->second);
        return true;
    }
    return false;
}

void ServerWithUnorderedMap::AttackMe(size_t nRank, const std::string & oDescription)
{
    std::lock_guard<std::mutex> lock(locker);
    if (!ExistAndIterate(nRank))
    {
        attack_map.emplace(nRank, Attack(nRank, oDescription));
    }
}
