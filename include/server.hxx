#pragma once

typedef std::pair<size_t, Attack> element_t;
typedef std::vector<Attack> result_t;

typedef std::map<size_t, Attack> map_container_t;
typedef std::unordered_map<size_t, Attack> unordered_map_container_t;
typedef std::set<Attack> set_container_t;

class IServer
{
protected:
    IServer()
    {}
    virtual ~IServer()
    {}

public:
    virtual const std::string & Name() const = 0;

public:
    virtual void   AttackMe(size_t nRank, const std::string & oDescription) = 0;
    virtual void   GetTopN(size_t N, result_t & result) = 0;
    virtual size_t Count() = 0;
}; // class IServer

template <class T>
class TServer : public IServer
{
private:
    std::string name;
    std::mutex  locker;
    T           container;

public:
    TServer(const std::string & oName);
    virtual ~TServer();

protected:
    virtual bool CheckExistenceAndIcreaseCount(size_t nRank);

public:
    void                AttackMe(size_t nRank, const std::string & oDescription) override;
    void                GetTopN(size_t N, result_t & result) override;
    size_t              Count() override;
    const std::string & Name() const;
}; // class TServer

class ServerWithMap : public TServer<map_container_t>
{
public:
    ServerWithMap()
    : TServer<map_container_t>("map")
    {}
    ~ServerWithMap() {}
};

class ServerWithUnorderedMap : public TServer<unordered_map_container_t>
{
public:
    ServerWithUnorderedMap()
    : TServer<unordered_map_container_t>("unordered_map")
    {}
    ~ServerWithUnorderedMap() {}
};

class ServerWithSet : public TServer<set_container_t>
{
public:
    ServerWithSet()
    : TServer<set_container_t>("set")
    {}
    ~ServerWithSet() {}
};
