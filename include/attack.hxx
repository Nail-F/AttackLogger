#pragma once

class Attack
{
private:
    size_t      rank;
    std::string description;
    size_t      count;

public:
    Attack();

public:
    explicit Attack(size_t nRank, const std::string & oDescription);
    virtual ~Attack();

    bool operator<(const Attack & other) const;
    bool operator>(const Attack & other) const;
    void operator++();

    size_t              Rank() const;
    size_t              Count() const;
    const std::string & Description() const;
}; // class Attack
