#include <attack_logger.hxx>

template <class T>
TServer<T>::TServer(const std::string & oName)
    : IServer()
    , name(oName)
    , locker()
    , container()
{
    name += std::string(6 - name.size(), ' ');
}

template <class T>
TServer<T>::~TServer()
{}

template <class T>
const std::string & TServer<T>::Name() const
{
    return name;
}

template <class T>
bool TServer<T>::CheckExistenceAndIcreaseCount(size_t nRank)
{
    class T::iterator it = container.find(nRank);
    if (it != container.end())
    {
        ++(it->second);
        return true;
    }
    return false;
}

template <class T>
void TServer<T>::AttackMe(size_t nRank, const std::string & oDescription)
{
    std::lock_guard<std::mutex> lock(locker);
    if (!CheckExistenceAndIcreaseCount(nRank))
    {
        container.emplace(nRank, Attack(nRank, oDescription));
    }
}

template <class T>
size_t TServer<T>::Count()
{
    std::lock_guard<std::mutex> lock(locker);
    return container.size();
}

template <>
void TServer<map_container_t>::GetTopN(size_t N, result_t & result)
{
    result.clear();
    size_t                      count = 0;
    std::lock_guard<std::mutex> lock(locker);
    for (auto it = container.crbegin(); it != container.crend(); ++it)
    {
        result.push_back(it->second);

        if (N == ++count)
        {
            break;
        }
    }
}

template <>
void TServer<unordered_map_container_t>::GetTopN(size_t N, result_t & result)
{
    std::vector<element_t> copy;

    {
        std::lock_guard<std::mutex> lock(locker);
        copy = std::vector<element_t>(container.cbegin(), container.cend());
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

TServer<map_container_t>           tempMapServer("map");
TServer<unordered_map_container_t> tempUOMapServer("uomap");
