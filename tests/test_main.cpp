#include "test_framework.h"

int main()
{
    int failed = 0;

    for (const geox::test::TestCase& test : geox::test::registry())
    {
        try
        {
            test.function();
            std::cout << "[PASS] " << test.name << '\n';
        }
        catch (const std::exception& exception)
        {
            ++failed;
            std::cerr << "[FAIL] " << test.name << ": " << exception.what() << '\n';
        }
    }

    std::cout << geox::test::registry().size() - failed << '/'
              << geox::test::registry().size() << " tests passed.\n";
    return failed == 0 ? 0 : 1;
}
