#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>

int getUrlPics(const char *filename)
{
  CURL *curl;
  FILE *fp;
  CURLcode res;
  std::ifstream md5_file(filename); // 打开存储MD5值的文件
  std::string line;
  int count = 0; // 初始化图片序号

  curl = curl_easy_init();
  if (curl)
  {
    while (std::getline(md5_file, line))
    {
      std::string md5_value = line.substr(0, 32); // 分离MD5值
      
      std::string outfilename = std::to_string(count) + "-" + "person" + ".jpg"; // 构造文件名
      std::string url = "http://192.168.156.222:4869/" + md5_value;     // 构造图片链接
      std::cout << url << std::endl;
      fp = fopen(outfilename.c_str(), "wb");    // 打开图片文件
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // 设置curl的URL
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
      res = curl_easy_perform(curl);
      fclose(fp);

      count++; // 更新图片序号
    }
    curl_easy_cleanup(curl);
  }
  md5_file.close(); // 关闭文件

  return 0;
}

int getUrlPic(const char *MD5,const char *SavePicName)
{
  CURL *curl;
  FILE *fp;
  CURLcode res;
  char url[128] = "http://192.168.156.222:4869/";
  strncat(url,MD5,32);

  curl = curl_easy_init();
  if (curl)
  {
    fp = fopen(SavePicName, "wb");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
    
  }
  return 0;
}
