#define MAX_SOURCE_SIZE (0x100000)
#define NUM_SONGS 1000

#ifdef __APPLE__
#define I_DIR "../wav"
#define O_DIR "./fpid"
#define CSV_DIR "./report"
#else
#define I_DIR "../../_wav"
#define O_DIR "../fpid"
#define CSV_DIR "../report"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include "CL/opencl.h"
#endif

#include "AOCLUtils/aocl_utils.h"
#include "hifp/hifp.h"
#include "utils/utils.h"
#include "utils/csv.h"

using namespace std;
using namespace aocl_utils;
using namespace hifp;
using namespace my_utils;

const int NUMWAVE = NUM_WAVE;
const int NUMDWTECO = NUM_DWT_ECO;
const int NUMFRAME = NUM_FRAME;
const int num_songs = NUM_SONGS;

// OpenCL runtime configuration
string binary_file = "hifp.aocx";
cl_platform_id platform = NULL;
char platform_name[50];
unsigned num_devices = 0;
cl_device_id device = NULL;
cl_context context = NULL;
cl_command_queue queue = NULL;
cl_command_queue queue_2 = NULL;
cl_program program = NULL;
cl_kernel kernel[2];

cl_mem wave_buf = NULL;
cl_mem fpid_buf = NULL;

const cl_uint work_dim[2] = {1, 1};
const cl_uint num_events_in_wait_list[2] = {1, 1};
const size_t global_work_offset[2] = {0, 0};
const size_t global_work_size[2] = {4096};
const size_t local_work_size[2] = {0, 0};


// Problem data
const char *IDIR = I_DIR;
const char *ODIR = O_DIR;
const char *CSVDIR = CSV_DIR;

short int wave[NUMWAVE];
short int fpid[NUMDWTECO];

unsigned int c_fpid[128];

int song_id = 0;
vector<string> song_names;
vector<double> total_time;
vector<double> write_transfer_time;
vector<double> read_transfer_time;
vector<double> dwt_kernel_time;
vector<double> genfpid_kernel_time;

// Function prototypes
void init_opencl();
int init_problem(FILE *ifp, FILE *ofp);
void run();
void cleanup();
void print_executed_time();
void save_csv(string csvpath);



int main(int argc, char ** argv)
{
    Options options(argc, argv);

    if (options.has("kernel_bin"))
    {
        binary_file = options.get<string>("kernel_bin");
    }

    init_opencl();
    
    DIR *dir = NULL;
    struct dirent *ep;
    char ifpath[256];
    char ofpath[256];
    char csvpath[256];
    char current_datetime[50];
    FILE *ifp = NULL;
    FILE *ofp = NULL;
    FILE *csvfp = NULL;

    dir = opendir(IDIR);
    ASSERT(dir != NULL);
    

    while ((ep = readdir(dir)) != NULL && song_id < num_songs)
    {
        if (ep->d_type == DT_REG)
        {
            sprintf(ifpath, "%s/%s",     IDIR, ep->d_name);
            sprintf(ofpath, "%s/%s.raw", ODIR, ep->d_name);

            song_names.push_back(ep->d_name);

            ifp = fopen(ifpath, "rb+");
            ASSERT(ifp != NULL);

            ofp = fopen(ofpath, "wb");
            ASSERT(ofp != NULL);

            init_problem(ifp, ofp);
            run();

            // compress fpid: 4096 -> 128
            // memset(c_fpid, 0, sizeof(c_fpid));

            // for (int i=0; i<128; i++) {
            //     int fpid_offset = i * 32;
            //     c_fpid[i] = 0;
            //     for (int j=0; j<32; j++) {
            //         c_fpid[i] <<= 1;
                        
            //         if (fpid[fpid_offset+j] == 1) {
            //             c_fpid[i] |= 1;
            //         }
            //     }
            // }
            
            // print FPID for verification
            // for (int i=0; i<NUMDWTECO; i++) {
            //     printf("%hu ", fpid[i]);
            // }
            // printf("\n\n");

            // for (int i=0; i<128; i++) {
            //     printf("%u ", c_fpid[i]);
            // }

            printf("\n\n");


            // save_fp_to_disk(ofp, fpid);
            
            if (ifp != NULL) {
                fclose(ifp);
                ifp = NULL;
            }

            if (ofp != NULL) {
                fclose(ofp);
                ofp = NULL;
            }
            
            song_id++;
        }
    }

    print_executed_time();
    
    closedir(dir);

    get_date_time((char *) current_datetime);
    sprintf(csvpath, "%s/%s.%s.%s_%u.csv", CSVDIR, "opencl", platform_name, current_datetime, (int) round(getCurrentTimestamp()));
    save_csv(csvpath);

    cleanup();

    return 0;

err:
    // closedir(dir);
    return -1;
}



void init_opencl()
{
    cl_int status;
    int choose_device = 0;

    printf("Initializing OpenCL \n");

    // Get the OpenCL platform
#ifdef __APPLE__
    platform = findPlatform("Apple");
    sprintf(platform_name, "%s", "apple");
    choose_device = 1;
#else
    if (!setCwdToExeDir())
    {
        checkError(-1, "Failed to perform setCwdToExeDir()");
    }

    platform = findPlatform("Intel");
    sprintf(platform_name, "%s", "intel_pac_a10");
#endif
    if (platform == NULL)
    {
        checkError(-1, "Unable to find platform");
    }

    // Query the available OpenCL device.
    cl_device_id *devices = getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices);

    printf("\n");
    printf("Platform: %s\n", getPlatformName(platform).c_str());

    printf("Found %d device(s):\n", num_devices);
    for (unsigned i = 0; i < num_devices; ++i)
    {
        printf("- %s (id: %d)\n", getDeviceName(devices[i]).c_str(), devices[i]);
    }

    // Choose 1st device
    device = devices[choose_device];

    printf("\n");
    printf("Choose device:\n");
    printf("- %s (id: %d)\n", getDeviceName(device).c_str(), device);

    // Create the context.
    context = clCreateContext(NULL, 1, &device, &oclContextCallback, NULL, &status);
    checkError(status, "Failed to create context");

    // Create the program for all device. Use the first device as the
    // representative device (assuming all device are of the same type).
#ifdef __APPLE__
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("device/hifp.cl", "r");
    if (!fp)
    {
        checkError(-1, "Failed to load kernel");
    }
    source_str = (char *)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &status);
#else
    printf("Using kernel binary: %s\n", binary_file.c_str());
    program = createProgramFromBinary(context, binary_file.c_str(), &device, 1);
#endif

    // Build the program that was just created.
    status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
    checkError(status, "Failed to build program");

    // Command queue.
    queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
    checkError(status, "Failed to create command queue");

    queue_2 = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
    checkError(status, "Failed to create command queue_2");

    // Kernel.
    kernel[0] = clCreateKernel(program, "generate_fpid", &status);
    checkError(status, "Failed to create kernel");

    /* Print kernel's configuration */
    printf("\n");
    printf("Launching for device:      %d  \n", device);
    // printf("- DWT kernel:      \n");
    // printf("+ work_dim:                %u  \n", work_dim[0]);
    // printf("+ num_events_in_wait_list: %u  \n", num_events_in_wait_list[0]);
    // printf("+ global_work_offset:      %lu \n", global_work_offset[0]);
    // printf("+ global_work_size:        %lu \n", global_work_size[0]);
    // printf("+ local_work_size:         %lu \n", local_work_size[0]);
    
}



/* Load problem data here */
int init_problem(FILE *ifp, FILE *ofp)
{
    if (num_devices == 0)
    {
        checkError(-1, "No devices");
    }

    /* Load 1 wav */
    WAVEHEADER wave_header;

    /* initialize all array elements to zero */
    memset(wave, 0, sizeof(wave));
    memset(fpid, 0, sizeof(fpid));

    /* Load data */
    wave_header = read_wave_header(ifp);
    read_wav_data(ifp, wave, wave_header);

    return 0;
}



void run()
{
    cl_int status;
    cl_event write_event[1];
    cl_event read_event[1];
    cl_event kernel_event[2];


    /* Create buffer */
    wave_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, NUMWAVE * sizeof(short int), NULL, &status);
    checkError(status, "Failed to create buffer for input");
    fpid_buf   = clCreateBuffer(context, CL_MEM_READ_WRITE, NUMDWTECO * sizeof(short int), NULL, &status);
    checkError(status, "Failed to create buffer for output 1 - fpid");

    
    // Set kernel arguments.
    /* kernel 0 */
    unsigned argi = 0;
    status = clSetKernelArg(kernel[0], argi++, sizeof(cl_mem), &wave_buf);
    checkError(status, "Failed to set argument %d", argi - 1);
    status = clSetKernelArg(kernel[0], argi++, sizeof(cl_mem), &fpid_buf);
    checkError(status, "Failed to set argument %d", argi - 1);    

    /* start counting execution-time */
    const double start_time = getCurrentTimestamp();

    /* Transfer data to device */
    status = clEnqueueWriteBuffer(queue, wave_buf, CL_FALSE, 0, NUMWAVE * sizeof(short int), wave, 0, NULL, &write_event[0]);
    checkError(status, "Failed to transfer input wav");


    /* Run kernel */
    /* kernel 0 */

    status = clEnqueueTask( queue, 
                            kernel[0],
                            1,
                            &write_event[0],
                            &kernel_event[0]);
    checkError(status, "Failed to launch gen_fpid kernel");


    /* Read result from device */
    status = clEnqueueReadBuffer(queue, fpid_buf, CL_FALSE, 0, NUMDWTECO * sizeof(short int), fpid, 1, &kernel_event[0], &read_event[0]);
    clWaitForEvents(1, read_event);


    /* finish counting execution-time */
    const double end_time = getCurrentTimestamp();

    total_time.push_back((end_time - start_time) * 1e3);
    write_transfer_time.push_back((double)(getStartEndTime(write_event[0]) * 1e-6));
    read_transfer_time.push_back((double)(getStartEndTime(read_event[0]) * 1e-6));
    genfpid_kernel_time.push_back((double)(getStartEndTime(kernel_event[0]) * 1e-6));


    /* Release all events */
    clReleaseEvent(kernel_event[0]);
    clReleaseEvent(read_event[0]);
    clReleaseEvent(write_event[0]);

    /* Verify result */
    // verify_fpid(fpid, NULL, NULL);
}

void print_executed_time() 
{
    for (int i=0; i<song_id; i++) {
        printf("\n");
        printf("Total time:           %0.3f ms \n", total_time[i] );
        printf("Transfer write time:  %0.3f ms \n", write_transfer_time[i]);
        printf("Transfer read time:   %0.3f ms \n", read_transfer_time[i]);
        printf("Kernel time gen_fpid: %0.3f ms \n", genfpid_kernel_time[i]);
    }
}


void save_csv(string csvpath)
{
    printf("\n");
    printf("Report (csv): %s \n", csvpath.c_str());
    
    csv_data c_data;
    add_collumn(&c_data, "song", song_names);
    add_collumn(&c_data, "total", total_time);
    add_collumn(&c_data, "write", write_transfer_time);
    add_collumn(&c_data, "read", read_transfer_time);
    add_collumn(&c_data, "gen_fpid", genfpid_kernel_time);

    write_csv_file_v(&c_data, csvpath);
}


void cleanup()
{
    clReleaseKernel(kernel[0]);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseMemObject(wave_buf);
    clReleaseMemObject(fpid_buf);
}
