#include <string>
#include <map>

class Curl
{
public:
  Curl();
  virtual ~Curl();
  virtual std::string Delete(const std::string& url, int &statusCode);
  virtual std::string Get(const std::string& url, int &statusCode);
  virtual std::string Post(const std::string& url, const std::string& postData,
      int &statusCode);
  virtual void AddHeader(const std::string& name, const std::string& value);
  virtual void AddOption(const std::string& name, const std::string& value);
  virtual void ResetHeaders();
  virtual std::string GetCookie(const std::string& name);
  virtual std::string GetLocation() {
    return m_location;
  }

private:
  virtual std::string Request(const std::string& action, const std::string& url,
                              const std::string& postData, int &statusCode);
  std::string Base64Encode(unsigned char const* in, unsigned int in_len,
      bool urlEncode);
  std::map<std::string, std::string> m_headers;
  std::map<std::string, std::string> m_options;
  std::map<std::string, std::string> m_cookies;
  std::string m_location;
};
