#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define MAX_COLLUMNS 100
#define MAX_ROWS     300

typedef struct
{
    vector<string> header;
    vector<vector<string> > rows;
} csv_data;

int parse_csv_from_matrix(int num_of_collumns, int num_of_rows, int *matrix, csv_data *csv) {
    vector<vector<string> > rows;

    for (int i=0; i<num_of_collumns; i++) {
        vector<string> r;
        for (int j=0; j<num_of_rows; j++) {
            int v = *((matrix + i*num_of_rows) + j);
            printf("%d ", v);
            r.push_back(to_string(v));
        }
        rows.push_back(r);
    }

    csv->rows = rows;

    return 1;
}

int write_csv_file(csv_data *data, string filename)
{
    ofstream output_file;

    // create and open the .csv file
    output_file.open(filename, ios::out | ios::trunc);

    // write the file headers
    for (int i = 0; i < data->header.size(); i++)
    {
        output_file << data->header[i];

        if (i < data->header.size() - 1)
        {
            output_file << ",";
        }
    }
    output_file << endl;

    // write data to the file
    for (int i = 0; i < data->rows.size(); i++)
    {
        for (int j=0; j < data->rows[i].size(); j++) {
            output_file << data->rows[i][j];

            if (j < data->rows[i].size() - 1) {
                output_file << ",";
            }
        }
        output_file << endl;
    }

    // close the output file
    output_file.close();

    return 1;
}

int main(int argc, char **argv)
{
    csv_data c_data;
    c_data.header.push_back("okmen1");
    c_data.header.push_back("okmen2");

    vector<string> header;
    header.push_back("okmen1");
    header.push_back("okmen2");

    // vector<string> row1, row2, row3;
    // row1.push_back("1");
    // row1.push_back("2");
    // row2.push_back("1");
    // row2.push_back("2");
    // row3.push_back("1");
    // row3.push_back("2");

    // vector<vector<string> > rows;
    // rows.push_back(row1);
    // rows.push_back(row2);
    // rows.push_back(row3);

    // c_data.rows = rows;

    int matrix[2][3] = {{1,2,3}, {2,4,6}};

    parse_csv_from_matrix(2, 3, (int *)matrix, &c_data);

    write_csv_file(&c_data, "okmen2.csv");

    return 0;
}