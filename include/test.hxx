#pragma once

class Test
{
private:
    const size_t ATTACK_MAX = 1000 * 1000;
    const size_t LOG_COUNT = 5;
    const size_t PRINT_ROWS_LIMIT = 5;

private:
    IServer & server_ref;

public:
    explicit Test(IServer & server_ref);
    ~Test();

    void Print5FromTop100() const;
    void Run();

private:
    void Attacker();
    void Logger();
};