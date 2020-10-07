#define NUM_SONGS 1

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
#define PLATFORM_NAME "apple"
#else
#define I_DIR "../_wav"
#define O_DIR "./fpid"
#define CSV_DIR "./report"
#define PLATFORM_NAME "linux"
#endif

using namespace std;
using namespace hifp;
using namespace my_utils;

const int NUMWAVE   = NUM_WAVE;
const int NUMDWTECO = NUM_DWT_ECO;
const int NUMFRAME  = NUM_FRAME;
const int num_songs = NUM_SONGS;

const char *IDIR = I_DIR;
const char *ODIR = O_DIR;
const char *CSVDIR = CSV_DIR;
const char *platform = PLATFORM_NAME;

vector<string> song_names;
vector<double> preprocessing_time;
vector<double> execution_time;


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
    int song_id = 0;

    WAVEHEADER wave_header;
    short int wave16[131072];
    short int fpid[4096];
    unsigned int c_fpid[128];
    unsigned int plain_fpid[NUMFRAME];
    short int dwt_eco[NUMDWTECO];

    /* initialize all array elements to zero */
    memset(wave16, 0, sizeof(wave16));
    memset(fpid, 0, sizeof(fpid));
    memset(plain_fpid, 0, sizeof(plain_fpid));
    memset(dwt_eco, 0, sizeof(dwt_eco));

    dir = opendir(IDIR);

    ASSERT(dir != NULL);

    while ((ep = readdir(dir)) != NULL && song_id < num_songs)
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

            const double start_time_preprocessing = getCurrentTimestamp();

            /* Prepare wave data */
            wave_header = read_wave_header(ifp);
            read_wav_data(ifp, wave16, wave_header);

            const double end_time_preprocessing = getCurrentTimestamp();

            const double preprocessing_time_tmp = (end_time_preprocessing - start_time_preprocessing) * 1e3;

            const double start_time_execution = getCurrentTimestamp();

            /* Run */
            gen_fpid_2(wave16, fpid, dwt_eco);

            const double end_time_execution = getCurrentTimestamp();

            const double execution_time_tmp = (end_time_execution-start_time_execution) * 1e3;
            execution_time.push_back(execution_time_tmp);
            preprocessing_time.push_back(preprocessing_time_tmp);

            printf("preprocessing_time: %s : %lf \n", ep->d_name, preprocessing_time_tmp);
            printf("execution_time: %s : %lf \n", ep->d_name, execution_time_tmp);

            for (int i=0; i<128; i++) {
                int fpid_offset = i * 32;
                c_fpid[i] = 0;
                for (int j=0; j<32; j++) {
                    c_fpid[i] <<= 1;
                        
                    if (fpid[fpid_offset+j] == 1) {
                        c_fpid[i] |= 1;
                    }
                }
            }
            
            // print FPID for verification
            // for (int i=0; i<NUMDWTECO; i++) {
            //     printf("%hu ", fpid[i]);
            // }
            // printf("\n\n");

            // for (int i=0; i<128; i++) {
            //     printf("%u ", c_fpid[i]);
            // }

            // printf("\n\n");


            // verify_fpid(fpid, NULL, NULL);

            // save_fp_to_disk(ofp, fpid);

            song_id++;

            fclose(ifp);
            ifp = NULL;

            fclose(ofp);
            ofp = NULL;
        }
    }

    get_date_time((char *) current_datetime);
    sprintf(csvpath, "%s/%s.%s.%s_%u.csv", CSVDIR, "c", platform, current_datetime, (int) round(getCurrentTimestamp()));
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
    add_collumn(&c_data, "preprocessing_time", preprocessing_time);
    add_collumn(&c_data, "execution_time", execution_time);

    write_csv_file_v(&c_data, csvpath);
}
