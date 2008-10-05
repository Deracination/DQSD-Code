#pragma once

#include "Url.h"

class Form
{
public:
  Form(const std::string& name, const std::string& method, const std::string& action)
    : m_name(name)
    , m_method(method)
    , m_action(action)
  {
  }

  bool IsMethodValid() const
  {
    return AreEqual(m_method, "get") || AreEqual(m_method, "post");
  }

  std::string Name() const
  {
    return m_name;
  }

  void SetName(const std::string& name)
  {
    m_name = name;
  }

  std::string Method() const
  {
    return m_method;
  }

  std::string Action() const
  {
    return m_action;
  }
  
  std::string GetAbsoluteActionPath(const std::string& baseUrl) const
  {
    if (StartsWith(m_action, "http"))
      return m_action;

    Url url = Url::Parse(baseUrl);
    std::string actionPath = url.Scheme() + "://" + url.EndPoint();
    
    if (!StartsWith(m_action, "/"))
      actionPath += url.Directory();
    
    actionPath += m_action;

    return actionPath;
  }

private:
  bool AreEqual(const std::string& lhs, const std::string& rhs) const
  {
    return (_stricmp(lhs.c_str(), rhs.c_str()) == 0);
  }

  bool StartsWith(const std::string& s, const std::string& value) const
  {
    return (_strnicmp(s.c_str(), value.c_str(), value.length()) == 0);
  }

  std::string m_name;
  std::string m_method;
  std::string m_action;
};
