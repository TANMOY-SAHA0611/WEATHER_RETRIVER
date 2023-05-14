#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define API_URL "https://api.openweathermap.org/data/2.5/weather?q=kolkata&appid=1ee4b410571f815e21ff5b4b544f704d"

double convert(double kelvin)
{
 return kelvin - 273.15;
}
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    cJSON *root = cJSON_Parse((char *)contents);
    cJSON *weather = cJSON_GetObjectItem(root,"weather");
    cJSON *first_item = cJSON_GetArrayItem(weather,0);
    cJSON *name = cJSON_GetObjectItem(root, "name");
    printf("NAME: %s\n", name->valuestring);
    cJSON *description = cJSON_GetObjectItem(first_item, "description");
    printf("Weather : %s\n", description->valuestring);
    cJSON *main = cJSON_GetObjectItem(root,"main");
    cJSON *temp = cJSON_GetObjectItem(main, "temp");
    printf("TEMPERATURE: %.2f C\n", convert(temp->valuedouble));
    cJSON *feels_like = cJSON_GetObjectItem(main, "feels_like");
    printf("FEELS LIKE: %.2f C\n", convert(feels_like->valuedouble));
    cJSON *temp_min = cJSON_GetObjectItem(main, "temp_min");
    printf("MINIMUM TEMPERATURE: %.2f C\n", convert(temp_min->valuedouble));
    cJSON *temp_max = cJSON_GetObjectItem(main, "temp_max");
    printf("MAXIMUM TEMPERATURE: %.2f C\n", convert(temp_max->valuedouble));
    cJSON *pressure = cJSON_GetObjectItem(main, "pressure");
    printf("PRESSURE: %.2f hPa \n", pressure->valuedouble);
    cJSON *humidity = cJSON_GetObjectItem(main, "humidity");
    printf("HUMIDITY: %.2f % \n", humidity->valuedouble);
    cJSON *visibility = cJSON_GetObjectItem(root, "visibility");
    printf("VISIBILITY: %d meters\n", visibility->valueint);
    cJSON *wind = cJSON_GetObjectItem(root, "wind");
    cJSON *speed = cJSON_GetObjectItem(wind, "speed");
    printf("SPEED: %.2f m/s \n", speed->valuedouble);
    cJSON *deg = cJSON_GetObjectItem(wind, "deg");
    printf("WIND DIRECTION: %d degree \n", deg->valueint);
    cJSON_Delete(root);
    printf("\n");
    return size * nmemb;
}
int main()
{
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "Failed to retrieve weather data: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}
