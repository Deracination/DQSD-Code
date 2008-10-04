#pragma once

class Url
{
public:
  static Url Parse(const std::string& url)
  {
    URL_COMPONENTSA uc = {0};
    uc.dwStructSize = sizeof(uc);
    uc.dwUrlPathLength = uc.dwHostNameLength = uc.dwSchemeLength = uc.dwUrlPathLength = 1;

    InternetCrackUrlA(url.c_str(), 0, 0, &uc);

    std::string scheme(uc.lpszScheme, uc.lpszScheme + uc.dwSchemeLength);
    std::string path(uc.lpszUrlPath, uc.lpszUrlPath + uc.dwUrlPathLength);
    std::string hostname(uc.lpszHostName, uc.lpszHostName + uc.dwHostNameLength);

    // Zero out the port if the URL doesn't contain a port spec (the only ':' should 
    // be part of the protocol scheme)
    if (url.find_last_of(':') == scheme.length())
      uc.nPort = 0;

    return Url(scheme, hostname, path, uc.nPort);
  }

  std::string Scheme() const
  {
    return m_scheme;
  }

  std::string Host() const
  {
    return m_host;
  }

  std::string Path() const
  {
    return m_path;
  }

  std::string Directory() const
  {
    std::string directory = m_path;
    if (directory.empty())
      directory = "/";

    size_t last_slash = directory.find_last_of('/');
    if (last_slash != std::string::npos)
      directory = directory.substr(0, last_slash + 1);

    return directory;
  }

  unsigned Port() const
  {
    return m_port;
  }

  std::string EndPoint() const
  {
    std::string endpoint = m_host;

    if (m_port != 0)
    {
      char buffer[64] = {0};
      wsprintfA(buffer, "%u", m_port);

      endpoint += ":";
      endpoint += buffer;
    }

    return endpoint;
  }

private:
  Url(const std::string& scheme, const std::string& host, const std::string& path, unsigned port)
    : m_scheme(scheme)
    , m_host(host)
    , m_path(path)
    , m_port(port)
  {
  }

  std::string m_scheme;
  std::string m_host;
  std::string m_path;
  unsigned m_port;  
};