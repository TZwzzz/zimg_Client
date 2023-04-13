#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


int main(int argc, char **argv)
{
    if(argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *image_file = fopen(argv[1], "rb");
    if(!image_file) {
        printf("Failed to open image file %s\n", argv[1]);
        return 1;
    }

    CURL *curl_handle = curl_easy_init();
    if(!curl_handle) {
        printf("Failed to initialze curl\n");
        fclose(image_file);
        return 1;
    }

    // 打开 json 文件用于写入
    FILE *json_file = fopen("response.json", "wb");
    if(!json_file) {
        printf("Failed to create json file\n");
        return 1;
    }

    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    struct curl_slist *headerlist = NULL;
    static const char buf[] = "Expect:";

    curl_global_init(CURL_GLOBAL_ALL);

    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "uploadfile",
                 CURLFORM_FILE, argv[1],
                 CURLFORM_CONTENTTYPE, "image/jpeg",
                 CURLFORM_END);

    headerlist = curl_slist_append(headerlist, buf);

    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://192.168.156.222:4869/upload");
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, json_file);

    CURLcode res = curl_easy_perform(curl_handle);
    if(res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        printf("Upload succeeded\n");
        // 读取服务器返回的 JSON 数据
        fseek(json_file, 0, SEEK_END);
        long size = ftell(json_file);
        fseek(json_file, 0, SEEK_SET);
        char *json_data = (char *)malloc(size + 1);
        fread(json_data, 1, size, json_file);
        json_data[size] = '\0';

        printf("Received JSON data:\n%s\n", json_data);

        free(json_data);
    }

    curl_formfree(formpost);
    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl_handle);
    fclose(image_file);
    fclose(json_file);

    curl_global_cleanup();

    return 0;
}