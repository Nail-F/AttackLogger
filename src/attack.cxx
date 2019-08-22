#include <attack_logger.hxx>

Attack::Attack()
    : rank(0)
    , description()
    , count(0)
{}

Attack::Attack(size_t nRank, const std::string & oDescription)
    : rank(nRank)
    , description(oDescription)
    , count(1)
{}

Attack::~Attack()
{}

size_t Attack::Rank() const
{
    return rank;
}

size_t Attack::Count() const
{
    return count;
}

const std::string & Attack::Description() const
{
    return description;
}

bool Attack::operator<(const Attack & other) const
{
    return Rank() < other.Rank()
           || (Rank() == other.Rank() && Count() < other.Count());
}
bool Attack::operator>(const Attack & other) const
{
    return Rank() > other.Rank()
           || (Rank() == other.Rank() && Count() > other.Count());
}


void Attack::operator++()
{
    ++count;
}
