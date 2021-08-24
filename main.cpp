#include <iostream>
#include <cassert>
#include <type_traits>

#include <cstring>
#include <string>
#include <unordered_map>

enum class AvailableEnumValues
{
    VALUE_ONE,
    VALUE_TWO
};

template <typename EnumClassToBeWrapped>
struct GenericEnumWrapper
{
    using EnumMap = std::unordered_map<EnumClassToBeWrapped, std::string>;

    GenericEnumWrapper() noexcept
        : current(static_cast<AvailableEnumValues>(-1))
    {
    }
    explicit GenericEnumWrapper(EnumClassToBeWrapped v) noexcept
        : current(v)
    {
    }

    std::string stringify() const noexcept
    {
        if (!m_map.count(current))
            return "?";
        return m_map.at(current);
    }

    void parse(std::string str) noexcept
    {
        auto it = std::find_if(m_map.cbegin(), m_map.cend(), [str = std::move(str)](auto v)
                               { return v.second == str; });
        if (it != m_map.cend())
        {
            current = it->first;
        }
        else
        {
            current = static_cast<EnumClassToBeWrapped>(-1);
        }
    }

private:
    std::size_t count(EnumClassToBeWrapped v) const noexcept
    {
        return m_map.count(v);
    }
    EnumClassToBeWrapped current;
    static EnumMap m_map;
};

using EnumWrapper = GenericEnumWrapper<AvailableEnumValues>;
EnumWrapper::EnumMap EnumWrapper::m_map = {
    {AvailableEnumValues::VALUE_ONE, "VALUE_ONE"},
    {AvailableEnumValues::VALUE_TWO, "VALUE_TWO"}};

int main(int, char **)
{
    // from enum to string
    auto myEnum = EnumWrapper(AvailableEnumValues::VALUE_ONE);
    assert("VALUE_ONE" == myEnum.stringify());
    myEnum = EnumWrapper(AvailableEnumValues::VALUE_TWO);
    assert("VALUE_TWO" == myEnum.stringify());
    myEnum = EnumWrapper();
    assert("?" == myEnum.stringify());
    myEnum = EnumWrapper(static_cast<AvailableEnumValues>(-1));
    assert("?" == myEnum.stringify());

    // from string to enum
    myEnum.parse("VALUE_ONE");
    assert("VALUE_ONE" == myEnum.stringify());
    myEnum.parse("VALUE_TWO");
    assert("VALUE_TWO" == myEnum.stringify());
    myEnum.parse("?");
    assert("?" == myEnum.stringify());
    myEnum.parse("NOT_A_VALID_VALUE");
    assert("?" == myEnum.stringify());
    return EXIT_SUCCESS;
}
