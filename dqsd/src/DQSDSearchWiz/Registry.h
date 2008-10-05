#pragma once

static LONG QueryRegValue(CRegKey& key, const char* name, std::string& value)
{
    DWORD length = 0;
    key.QueryStringValue(name, NULL, &length);

    std::string buffer(length, 0);
    LONG result = key.QueryStringValue(name, &buffer[0], &length);
    if (result == ERROR_SUCCESS)
        value = buffer;

    return result;
}

static LONG QueryRegValue(CRegKey& key, const char* name, DWORD& value)
{
    return key.QueryDWORDValue(name, value);
}

static LONG QueryRegValue(CRegKey& key, const char* name, bool& value)
{
    DWORD state = 0;
    LONG result = QueryRegValue(key, name, state);
    if (result == ERROR_SUCCESS)
        value = (state != 0);

    return result;
}

static LONG SetRegValue(CRegKey& key, const char* name, const std::string& value)
{
    return key.SetStringValue(name, value.c_str());
}

static LONG SetRegValue(CRegKey& key, const char* name, bool value)
{
    return key.SetDWORDValue(name, value ? 1UL : 0UL);
}
