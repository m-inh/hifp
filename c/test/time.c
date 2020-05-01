#include <stdio.h>
#include <time.h>

int get_date_time(char *buffer) {
    time_t time_raw_format;
    struct tm *ptr_time;
    // char buffer[50];

    time(&time_raw_format);
    ptr_time = localtime(&time_raw_format);
    if (strftime(buffer, 50, "%Y_%m_%d", ptr_time) == 0)
    {
        perror("Couldn't prepare formatted string");
    }
    else
    {
        // printf("Current local time and date: %s", buffer);
    }

    return 1;
}

int main()
{
    char date_time[50];
    get_date_time((char *)date_time);

    printf("Current local time and date: %s", date_time);

    return 0;
}