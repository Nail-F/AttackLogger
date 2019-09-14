#include <attack_logger.hxx>

Test::Test(IServer & server)
    : server_ref(server)
{}

Test::~Test()
{}

void Test::Run()
{
    std::thread attacker(&Test::Attacker, this);
    std::thread logger(&Test::Logger, this);
    attacker.join();
    logger.join();
}

void Test::Attacker()
{
    auto start_time = std::chrono::system_clock::now();
    for (size_t it = 0; it < ATTACK_MAX; ++it)
    {
        size_t rank = std::rand();
        server_ref.AttackMe(rank, std::string("attack with rank #") + std::to_string(rank));
    }

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>
                     (std::chrono::system_clock::now() - start_time).count();
    std::cout << "server: " << server_ref.Name() << " "
              << "attacker: execution time: " << delta << " ms, "
              << "attacks: " << server_ref.Count()
              << " of " << ATTACK_MAX
              << std::endl;
}

void Test::Print5FromTop100() const
{
    result_t res;
    server_ref.GetTopN(100, res);
    size_t count = 0;
    for (auto & it : res)
    {
        (void) it;
        std::cout << "rank: " << it.Rank()
                  << ", decription: " << it.Description()
                  << ", attacks count: " << it.Count()
                  << std::endl;
        if (++count >= PRINT_ROWS_LIMIT)
        {
            break;
        }
    }
    std::cout << std::endl;
}

void Test::Logger()
{
    auto start_time = std::chrono::system_clock::now();

    for (size_t i = 0; i < LOG_COUNT; ++i)
    {
        // std::cout << "logger: #" << i
        //           << std::endl;

        Print5FromTop100();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>
                     (std::chrono::system_clock::now() - start_time).count();
    std::cout << "server: " << server_ref.Name() << " "
              << "logger:   execution time: " <<  delta << " ms"
              << std::endl;
}
