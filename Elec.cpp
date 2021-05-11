#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <filesystem>
#include <Windows.h>

using namespace std;

string preprocess_data(string File_Path)
{
    fstream reader;
    string buffer;
    string line_buffer;
    string dataset;
    int comma_iteraiton = 0;

    reader.open(File_Path);

    while (getline(reader, buffer))
    {
        for (char c : buffer)
        {
            if (c == ',')
            {
                comma_iteraiton += 1;
            }
            else
            {
                comma_iteraiton = 0;
            }

            if (comma_iteraiton > 1)
            {
                continue;
            }
            else
            {
                line_buffer += c;
            }
        }

        dataset += line_buffer;
        line_buffer = "";
        dataset += "\n";

        comma_iteraiton = 0;
    }
    reader.close();

    return dataset;
}

void save_dataset(string File_Path, string Text)
{
    ofstream writer(File_Path);
    writer << Text;
    writer.close();
}

float get_value(string file_path, int x, int y)
{
    string buffer;
    fstream reader;
    reader.open(file_path);

    for (int row = 1; row < x; row++)
    {
        getline(reader, buffer);
    }

    getline(reader, buffer, ',');

    for (int column = 1; column <= y; column++)
    {
        getline(reader, buffer, ',');
    }

    return stof(buffer);
}

int main()
{
    string file_name;
    char working_directory[MAX_PATH + 1];
    GetCurrentDirectoryA(sizeof(working_directory), working_directory);
    string current_path = working_directory;

    cout << "Copy file on this location (CSV Type): " << current_path << endl;

    cout << "Enter file name: ";
    cin >> file_name;

    string data = preprocess_data(current_path + "\\" + file_name);
    save_dataset(current_path + "\\" + "New_" + file_name, data);

    string file_path;
    file_path = current_path + "\\" + "New_" + file_name;

    cout << get_value(file_path, 1, 1);

    string output_file = current_path + "\\" + "Output.csv";
    ofstream writer(output_file);

    float ro, a, s, R, Rw, n, F, Rsr, L, Rr1, Rr2, Rn, Pi;

    ro = get_value(file_path, 1, 1); //Ohm.M
    a = get_value(file_path, 2, 1); //Meter
    s = get_value(file_path, 3, 1); //Meter
    R = 0; //Initialization
    Pi = get_value(file_path, 4, 1);

    const float e = get_value(file_path, 5, 1);

    //METHOD OF CALCULATION EARTH WELL RESISTANCE
    R = (ro / (8 * a)) + (ro / (4 * Pi * s)) * (1 - ((7 / 24) * (pow(a, 2) / pow(s, 2)) + (99 / 30) * (pow(a, 4) / pow(s, 4))));

    cout << "EARTH WELL RESISTANCE: ";
    cout << R;
    cout << " Ohm\n";
    
    writer << "R";
    writer << ',';
    writer << R;
    writer << "\n";

    n = get_value(file_path, 7, 1);
    F = get_value(file_path, 8, 1);
    //Rw CALCULATION
    Rw = (R / n) * F;

    cout << "Rw: ";
    cout << Rw;
    cout << " Ohm\n";

    writer << "Rw";
    writer << ',';
    writer << Rw;
    writer << "\n";

    L = get_value(file_path, 10, 1); //Meter
    ro = get_value(file_path, 11, 1); //Ohm.M
    a = get_value(file_path, 12, 1); //Meter
    F = get_value(file_path, 13, 1);
    n = get_value(file_path, 14, 1);

    //CALCULATION EARTH ROD RESISTANCE
    Rsr = (ro / (2 * Pi * L)) * (log((4 * L) / a) - 1);

    cout << "EARTH ROD RESISTANCE: ";
    cout << Rsr;
    cout << " Ohm\n";

    writer << "Rsr";
    writer << ',';
    writer << Rsr;
    writer << "\n";

    //Rr1 CALCULATION
    Rr1 = (Rsr / n) * F;

    cout << "Rr1: ";
    cout << Rr1;
    cout << " Ohm\n";

    writer << "Rr1";
    writer << ',';
    writer << Rr1;
    writer << "\n";

    ro = get_value(file_path, 17, 1); //Ohm.M

    //CALCULATION EARTH ROD RESISTANCE
    Rsr = (ro / (2 * Pi * L)) * (log((4 * L) / a) - 1);

    cout << "EARTH ROD RESISTANCE: ";
    cout << Rsr;
    cout << " Ohm\n";

    writer << "Rsr";
    writer << ',';
    writer << Rsr;
    writer << "\n";

    //Rr2 CALCULATION
    Rr2 = (Rsr / n) * F;

    cout << "Rr2: ";
    cout << Rr2;
    cout << " Ohm\n";

    writer << "Rr2";
    writer << ',';
    writer << Rr2;
    writer << "\n";

    // instrument earth resistant for FBS NO.2/FFPS NO.2/MAIN GAS STATION
    Rn = pow((1 / Rw) + (1 / Rr1) + (1 / Rr2), -1);

    cout << "Rn: ";
    cout << Rn;
    cout << " Ohm\n";

    writer << "Rn";
    writer << ',';
    writer << Rn;
    writer << "\n";

    writer.close();

    cout << "File saved on " + current_path << endl << endl;

    return 0;
}

