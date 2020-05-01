#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>

#include "hifp/hifp.h"
#include "utils/utils.h"
#include "utils/csv.h"

#ifdef __APPLE__
#define I_DIR "../wav"
#define O_DIR "./fpid"
#define CSV_DIR "./report"
#define PLATFORM_NAME "Apple"
#else
#define I_DIR "../_wav"
#define O_DIR "./fpid"
#define CSV_DIR "./report"
#define PLATFORM_NAME "Linux"
#endif

using namespace std;
using namespace hifp;
using namespace my_utils;

const char *IDIR = I_DIR;
const char *ODIR = O_DIR;
const char *CSVDIR = CSV_DIR;
const char *platform = PLATFORM_NAME;

vector<string> song_names;
vector<double> total_time;


void save_csv(string csvpath);


/* Entry point */
int main(int argc, char **argv)
{
    DIR *dir = NULL;
    struct dirent *ep;
    char ifpath[256];
    char ofpath[256];
    char csvpath[256];
    char current_datetime[50];
    FILE *ifp = NULL;
    FILE *ofp = NULL;
    int r;

    dir = opendir(IDIR);

    ASSERT(dir != NULL);

    while ((ep = readdir(dir)) != NULL)
    {
        if (ep->d_type == DT_REG)
        {
            sprintf(ifpath, "%s/%s", IDIR, ep->d_name);
            sprintf(ofpath, "%s/%s.raw", ODIR, ep->d_name);

            song_names.push_back(ep->d_name);

            ifp = fopen(ifpath, "rb+");
            ASSERT(ifp != NULL);

            ofp = fopen(ofpath, "wb");
            ASSERT(ofp != NULL);

            const double start_time = getCurrentTimestamp();

            r = run_all(ifp, ofp);
            ASSERT(r == 0);

            const double end_time = getCurrentTimestamp();
            const double total_time_tmp = (end_time-start_time) * 1e3;
            total_time.push_back(total_time_tmp);
            printf("%s : %lf \n", ep->d_name, total_time_tmp);


            fclose(ifp);
            ifp = NULL;

            fclose(ofp);
            ofp = NULL;
        }
    }

    get_date_time((char *) current_datetime);
    sprintf(csvpath, "%s/%s.%s_%u.csv", CSVDIR, platform, current_datetime, (int) round(getCurrentTimestamp()));
    save_csv(csvpath);

    closedir(dir);

    return 0;

err:
    if (ifp != NULL)
    {
        fclose(ifp);
    }
    if (ofp != NULL)
    {
        fclose(ofp);
    }
    if (dir != NULL)
    {
        closedir(dir);
    }
    return 0;
}


void save_csv(string csvpath)
{
    printf("Report (csv): %s \n", csvpath.c_str());
    
    csv_data c_data;
    add_collumn(&c_data, "song_names", song_names);
    add_collumn(&c_data, "total_time", total_time);

    write_csv_file_v(&c_data, csvpath);
}
