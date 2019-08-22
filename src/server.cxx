#include <attack_logger.hxx>

IServer::IServer(const std::string & oName)
    : lock_mutex()
    , name(oName)
{
    name += std::string(18 - name.size(), ' ');
}

IServer::~IServer()
{}

const std::string & IServer::Name() const
{
    return name;
}

/*
 *
 */

ServerWithMap::ServerWithMap()
    : IServer("std::map")
    , attack_map()
{}

ServerWithMap::~ServerWithMap()
{}

void ServerWithMap::GetTopN(size_t N, result_t & result)
{
    result.clear();
    {
        size_t                      count = 0;
        std::lock_guard<std::mutex> lock(lock_mutex);
        for (auto it = attack_map.rbegin(); it != attack_map.rend(); ++it)
        {
            result.push_back(it->second);

            if (N == ++count)
            {
                break;
            }
        }
    }
}

size_t ServerWithMap::Count()
{
    std::lock_guard<std::mutex> lock(lock_mutex);
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
    {
        std::lock_guard<std::mutex> lock(lock_mutex);
        if (!ExistAndIterate(nRank))
        {
            attack_map.emplace(nRank, Attack(nRank, oDescription));
        }
    }
}

/*
 *
 */

ServerWithUnorderedMap::ServerWithUnorderedMap()
    : IServer("std::unordered_map")
    , attack_map()
{}

ServerWithUnorderedMap::~ServerWithUnorderedMap()
{}

void ServerWithUnorderedMap::GetTopN(size_t N, result_t & result)
{
    std::vector<IServer::element_t> copy;

    {
        std::lock_guard<std::mutex> lock(lock_mutex);
        copy = std::vector<IServer::element_t>(attack_map.begin(), attack_map.end());
    }

    size_t middle = N;
    if (middle > copy.size())
    {
        middle = copy.size();
    }

    std::partial_sort(copy.begin(), copy.begin() + middle, copy.end(),
                      [](const IServer::element_t a, const IServer::element_t b)
                      { return a.second > b.second; });

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
    std::lock_guard<std::mutex> lock(lock_mutex);
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
    {
        std::lock_guard<std::mutex> lock(lock_mutex);
        if (!ExistAndIterate(nRank))
        {
            attack_map.emplace(nRank, Attack(nRank, oDescription));
        }
    }
}
