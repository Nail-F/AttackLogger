#pragma once

class IServer
{
protected:
    std::mutex  lock_mutex;
    std::string name;

public:
    typedef std::pair<size_t, Attack> element_t;
    typedef std::vector<Attack> result_t;

protected:
    IServer(const std::string & oName);
    virtual ~IServer();

public:
    const std::string & Name() const;

public:
    virtual void   AttackMe(size_t nRank, const std::string & oDescription) = 0;
    virtual void   GetTopN(size_t N, result_t & result) = 0;
    virtual size_t Count() = 0;

protected:
    virtual bool ExistAndIterate(size_t nRank) = 0;
}; // class IServer

/*
 * std::map
 */

class ServerWithMap : public IServer
{
public:
    ServerWithMap();
    ~ServerWithMap();

private:
    typedef std::map<size_t, Attack> container_t;
    container_t attack_map;

public:
    void   AttackMe(size_t nRank, const std::string & oDescription) override;
    void   GetTopN(size_t N, result_t & result) override;
    size_t Count() override;

protected:
    bool ExistAndIterate(size_t nRank) override;
}; // class ServerWithMap

/*
 * std::unordered_map
 */

class ServerWithUnorderedMap : public IServer
{
public:
    ServerWithUnorderedMap();
    ~ServerWithUnorderedMap();

private:
    typedef std::unordered_map<size_t, Attack> container_t;
    container_t attack_map;

public:
    void   AttackMe(size_t nRank, const std::string & oDescription) override;
    void   GetTopN(size_t N, result_t & result) override;
    size_t Count() override;

protected:
    bool ExistAndIterate(size_t nRank) override;
}; // class ServerWithUnorderedMap
