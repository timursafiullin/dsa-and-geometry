#pragma once

#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace geox::test
{
    struct TestCase
    {
        const char* name;
        void (*function)();
    };

    inline std::vector<TestCase>& registry()
    {
        static std::vector<TestCase> tests;
        return tests;
    }

    class Registration
    {
    public:
        Registration(const char* name, void (*function)())
        {
            registry().push_back({name, function});
        }
    };

    inline void require(bool condition, const char* expression, const char* file, int line)
    {
        if (condition)
            return;

        std::ostringstream message;
        message << file << ':' << line << ": requirement failed: " << expression;
        throw std::runtime_error(message.str());
    }

    inline void requireClose(
        double actual,
        double expected,
        const char* actualExpression,
        const char* expectedExpression,
        const char* file,
        int line,
        double epsilon = 1e-12
    )
    {
        if (std::abs(actual - expected) <= epsilon)
            return;

        std::ostringstream message;
        message << file << ':' << line << ": expected " << actualExpression << " (" << actual
                << ") to equal " << expectedExpression << " (" << expected << ')';
        throw std::runtime_error(message.str());
    }

    template <typename Exception, typename Callable>
    void requireThrows(Callable&& callable, const char* expression, const char* file, int line)
    {
        try
        {
            callable();
        }
        catch (const Exception&)
        {
            return;
        }
        catch (const std::exception& exception)
        {
            throw std::runtime_error(
                std::string(file) + ':' + std::to_string(line) + ": " + expression +
                " threw an unexpected exception: " + exception.what()
            );
        }

        throw std::runtime_error(
            std::string(file) + ':' + std::to_string(line) + ": " + expression +
            " did not throw"
        );
    }
} // namespace geox::test

#define GEOX_TEST(name) \
    static void name(); \
    static ::geox::test::Registration name##_registration(#name, &name); \
    static void name()

#define GEOX_REQUIRE(...) \
    ::geox::test::require((__VA_ARGS__), #__VA_ARGS__, __FILE__, __LINE__)

#define GEOX_REQUIRE_CLOSE(actual, expected) \
    ::geox::test::requireClose((actual), (expected), #actual, #expected, __FILE__, __LINE__)

#define GEOX_REQUIRE_THROWS_AS(expression, exception_type) \
    ::geox::test::requireThrows<exception_type>([&] { expression; }, #expression, __FILE__, __LINE__)
