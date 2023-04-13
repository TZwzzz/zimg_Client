#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>

// 回调函数，用于接收数据
static size_t receive_data(char *ptr, size_t size, size_t nmemb, void *userdata) 
{
    // 打印接收到的数据
    printf("%.*s\n", (int)(size * nmemb), ptr);
    return size * nmemb;
}


int delUrlPics(const char *MD5filename) {
    CURL *curl;
    CURLcode res;
    const char *url = "http://zimg_server_address/admin?md5=your_md5_code&t=1";
    FILE *fp;
    std::ifstream md5_file(MD5filename); // 打开存储MD5值的文件
    std::string line;

    curl = curl_easy_init();
    if(curl) {

        while (std::getline(md5_file, line))
        {
            std::string md5_value = line.substr(0, 32); // 分离MD5值
            std::string url = "http://192.168.156.222:4869/admin?md5=" + md5_value + "&t=1";     // 构造图片链接
            std::cout << url << std::endl;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            // 设置回调函数，用于接收数据
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
            /* 执行请求 */
            res = curl_easy_perform(curl);

            /* 检查请求是否成功 */
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
        }
        /* 清理curl句柄 */
        curl_easy_cleanup(curl);
    }
    return 0;
}

int main(int argc,const char *argv[])
{
    if(argc!= 2)
    {
        std::cout << "Usage: " << argv[0] << " <MD5 filename>" << std::endl;
        return -1;
    }
    delUrlPics(argv[1]);
    return 0;
}