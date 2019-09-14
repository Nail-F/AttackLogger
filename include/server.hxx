#pragma once

typedef std::pair<size_t, Attack> element_t;
typedef std::vector<Attack> result_t;

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

class Server : public IServer
{
protected:
    std::string name;
    std::mutex  locker;

protected:
    Server(const std::string & oName);
    virtual ~Server();

protected:
    virtual bool ExistAndIterate(size_t nRank) = 0;

public:
    const std::string & Name() const;
}; // class Server

/*
 * std::map
 */
class ServerWithMap : public Server
{
public:
    ServerWithMap();
    ~ServerWithMap();

private:
    typedef std::map<size_t, Attack> container_t;
    container_t attack_map;

protected:
    bool ExistAndIterate(size_t nRank) override;

public:
    void   AttackMe(size_t nRank, const std::string & oDescription) override;
    void   GetTopN(size_t N, result_t & result) override;
    size_t Count() override;
}; // class ServerWithMap

/*
 * std::unordered_map
 */

class ServerWithUnorderedMap : public Server
{
public:
    ServerWithUnorderedMap();
    ~ServerWithUnorderedMap();

private:
    typedef std::unordered_map<size_t, Attack> container_t;
    container_t attack_map;

protected:
    bool ExistAndIterate(size_t nRank) override;

public:
    void   AttackMe(size_t nRank, const std::string & oDescription) override;
    void   GetTopN(size_t N, result_t & result) override;
    size_t Count() override;
}; // class ServerWithUnorderedMap
