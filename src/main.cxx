#include <main.hxx>

int main(int argc, char const *argv[])
{
    {
        ServerWithMap server;
        Test test(server);
        test.Run();
    }

    {
        ServerWithUnorderedMap server;
        Test test(server);
        test.Run();
    }

    return 0;
}
