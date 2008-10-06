#pragma once

class FormElement
{
public:
  FormElement(const std::string& tag, const std::string& type, const std::string& name, const std::string& value, bool checked = false)
    : m_tag(tag)
    , m_type(type)
    , m_name(name)
    , m_value(value)
    , m_checked(checked)
  {
    ForceLower(m_tag);
    ForceLower(m_type);
  }

  std::string Tag() const
  {
    return m_tag;
  }

  std::string Type() const
  {
    return m_type;
  }

  std::string Name() const
  {
    return m_name;
  }

  std::string Value() const
  {
    return m_value;
  }

  bool Checked() const
  {
    return m_checked;
  }

private:
  void ForceLower(std::string& value)
  {
    std::transform(value.begin(), value.end(), value.begin(), tolower);
  }

  std::string m_tag;
  std::string m_type;
  std::string m_name;
  std::string m_value;
  bool m_checked;
};