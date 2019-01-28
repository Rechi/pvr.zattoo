#include "Utils.h"

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <sstream>

#include <iostream>

#include "p8-platform/os.h"

#include "client.h"

using namespace ADDON;

std::string Utils::GetFilePath(const std::string &strPath, bool bUserPath)
{
  return (bUserPath ? g_strUserPath : g_strClientPath) + PATH_SEPARATOR_CHAR
      + strPath;
}

// http://stackoverflow.com/a/17708801
std::string Utils::UrlEncode(const std::string &value)
{
  std::ostringstream escaped;
  escaped.fill('0');
  escaped << std::hex;

  for (char c : value) {
      // Keep alphanumeric and other accepted characters intact
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
    {
      escaped << c;
      continue;
    }

    // Any other characters are percent-encoded
    escaped << '%' << std::setw(2) << int((unsigned char) c);
  }

  return escaped.str();
}

double Utils::StringToDouble(const std::string &value)
{
  std::istringstream iss(value);
  double result;

  iss >> result;

  return result;
}

int Utils::StringToInt(const std::string &value)
{
  return (int) StringToDouble(value);
}

std::vector<std::string> Utils::SplitString(const std::string &str,
    const char &delim, int maxParts)
{
  typedef std::string::const_iterator iter;
  iter beg = str.begin();
  std::vector < std::string > tokens;

  while (beg != str.end())
  {
    if (maxParts == 1)
    {
      tokens.emplace_back(beg, str.end());
      break;
    }
    maxParts--;
    iter temp = find(beg, str.end(), delim);
    if (beg != str.end())
      tokens.emplace_back(beg, temp);
    beg = temp;
    while ((beg != str.end()) && (*beg == delim))
      beg++;
  }

  return tokens;
}

std::string Utils::ReadFile(const std::string& path)
{
  void* file;
  file = XBMC->CURLCreate(path.c_str());
  if (!file || !XBMC->CURLOpen(file, 0))
  {
    XBMC->Log(LOG_ERROR, "Failed to open file [%s].", path.c_str());
    return "";
  }

  char buf[1025];
  ssize_t nbRead;
  std::string content;
  while ((nbRead = XBMC->ReadFile(file, buf, 1024)) > 0)
  {
    buf[nbRead] = 0;
    content.append(buf);
  }
  XBMC->CloseFile(file);
  
  return content;

}

time_t Utils::StringToTime(const std::string &timeString)
{
  struct tm tm{};

  int year, month, day, h, m, s, tzh, tzm;
  if (sscanf(timeString.c_str(), "%d-%d-%dT%d:%d:%d%d", &year, &month, &day, &h,
      &m, &s, &tzh) < 7)
  {
    tzh = 0;
  }
  tzm = tzh % 100;
  tzh = tzh / 100;

  tm.tm_year = year - 1900;
  tm.tm_mon = month - 1;
  tm.tm_mday = day;
  tm.tm_hour = h - tzh;
  tm.tm_min = m - tzm;
  tm.tm_sec = s;

  time_t ret = timegm(&tm);
  return ret;
}
