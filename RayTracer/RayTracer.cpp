// RayTracer.cpp 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>

// Output in P6 format, a binary file containing:
// P6
// ncolumns nrows
// Max colour value
// colours in binary format thus unreadable
void save_imageP6(int Width, int Height, char* fname, unsigned char* pixels) {
    FILE* fp;
    const int maxVal = 255;

    printf("Saving image %s: %d x %d\n", fname, Width, Height);
    fp = fopen(fname, "wb");
    if (!fp) {
        printf("Unable to open file '%s'\n", fname);
        return;
    }
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", Width, Height);
    fprintf(fp, "%d\n", maxVal);

    for (int j = 0; j < Height; j++) {
        fwrite(&pixels[j * Width * 3], 3, Width, fp);
    }

    fclose(fp);
}

// Output in P3 format, a text file containing:
// P3
// ncolumns nrows
// Max colour value (for us, and usually 255)
// r1 g1 b1 r2 g2 b2 .....
void save_imageP3(int Width, int Height, char* fname, unsigned char* pixels) {
    FILE* fp;
    const int maxVal = 255;

    printf("Saving image %s: %d x %d\n", fname, Width, Height);
    fp = fopen(fname, "w");
    if (!fp) {
        printf("Unable to open file '%s'\n", fname);
        return;
    }
    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", Width, Height);
    fprintf(fp, "%d\n", maxVal);

    int k = 0;
    for (int j = 0; j < Height; j++) {

        for (int i = 0; i < Width; i++)
        {
            fprintf(fp, " %d %d %d", pixels[k], pixels[k + 1], pixels[k + 2]);
            k = k + 3;
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// Sphere class
class Sphere
{
public:
    std::string name;
    std::vector<double> pos;
    std::vector<double> scl;
    std::vector<double> color;
    double Ka, Kd, Ks, Kr;
    int spec;
    Sphere() {};
    ~Sphere() {};
private:
};

// Light class
class Light
{
public:
    std::string name;
    std::vector<double> pos;
    std::vector<double> color;
    Light() {};
    ~Light() {};
private:
};


std::vector<double> raytrace(std::vector<double> ray_S, std::vector<double> ray_c, int depth_budget) {
    // Check recursive depth
    if (depth_budget == 0) {
        std::vector<double> c(3,0);
        return c;
    }

}

int main(int argc, char* argv[])
{
    // Parameter checking
    if (argc != 2) {
        std::cout << "Invalid parameters. Usage: RayTracer.exe <config-file-name>\n";
        return 1;
    }

    // Input file data
    double near, left, right, top, bottom;
    int nColumns, nRows;
    std::vector<Sphere> sphereList;
    std::vector<Light> lightList;
    std::vector<double> bg_color;
    std::vector<double> ambient_intensity;
    std::string ofilename;

    std::cout << "Reading from: " << argv[1] << "\n";
    // Read input file
    std::ifstream ifile(argv[1]);
    std::string ifile_line;
    std::string token;
    if (ifile.is_open())
        while (std::getline(ifile, ifile_line)) {
            std::stringstream ss(ifile_line);
            if (std::getline(ss, token, ' ')) {
                if (token.compare("NEAR") == 0) {
                    std::getline(ss, token, ' ');
                    near = std::stof(token);
                }
                else if (token.compare("LEFT") == 0) {
                    std::getline(ss, token, ' ');
                    left = std::stof(token);
                }
                else if (token.compare("RIGHT") == 0) {
                    std::getline(ss, token, ' ');
                    right = std::stof(token);
                }
                else if (token.compare("BOTTOM") == 0) {
                    std::getline(ss, token, ' ');
                    bottom = std::stof(token);
                }
                else if (token.compare("TOP") == 0) {
                    std::getline(ss, token, ' ');
                    top = std::stof(token);
                }
                else if (token.compare("RES") == 0) {
                    std::getline(ss, token, ' ');
                    nColumns= std::stoi(token);
                    std::getline(ss, token, ' ');
                    nRows = std::stoi(token);
                }
                else if (token.compare("SPHERE") == 0) {
                    Sphere sphere;
                    std::getline(ss, token, ' ');
                    sphere.name = token;
                    for (int i = 0; i < 3; i++) {
                        std::getline(ss, token, ' ');
                        sphere.pos.push_back(std::stof(token));
                    }
                    for (int i = 0; i < 3; i++) {
                        std::getline(ss, token, ' ');
                        sphere.scl.push_back(std::stof(token));
                    }
                    for (int i = 0; i < 3; i++) {
                        std::getline(ss, token, ' ');
                        sphere.color.push_back(std::stof(token));
                    }
                    std::getline(ss, token, ' ');
                    sphere.Ka = std::stof(token);
                    std::getline(ss, token, ' ');
                    sphere.Kd = std::stof(token);
                    std::getline(ss, token, ' ');
                    sphere.Ks = std::stof(token);
                    std::getline(ss, token, ' ');
                    sphere.Kr = std::stof(token);
                    std::getline(ss, token, ' ');
                    sphere.spec = std::stoi(token);
                    sphereList.push_back(sphere);
                }
                else if (token.compare("LIGHT") == 0) {
                    Light light;
                    std::getline(ss, token, ' ');
                    light.name = token;
                    for (int i = 0; i < 3; i++) {
                        std::getline(ss, token, ' ');
                        light.pos.push_back(std::stof(token));
                    }
                    for (int i = 0; i < 3; i++) {
                        std::getline(ss, token, ' ');
                        light.color.push_back(std::stof(token));
                    }
                    lightList.push_back(light);
                }
                else if (token.compare("BACK") == 0) {
                    for (int i = 0; i < 3; i++) {
                        std::getline(ss, token, ' ');
                        bg_color.push_back(std::stof(token));
                    }
                }
                else if (token.compare("AMBIENT") == 0) {
                    for (int i = 0; i < 3; i++) {
                        std::getline(ss, token, ' ');
                        ambient_intensity.push_back(std::stof(token));
                    }
                }
                else if (token.compare("OUTPUT") == 0) {
                    std::getline(ss, token, ' ');
                    ofilename = token;
                }
            }
        }
    ifile.close();

    // Init grid
    unsigned char *pixels;
    pixels = new unsigned char [3 * nColumns * nRows];

    /*
    for (auto i : bg_color) std::cout << i << ' ';
    std::cout << '\n';
    for (auto i : ambient_intensity) std::cout << i << ' ';
    std::cout << '\n';
    std::cout << ofilename << '\n';
    */

    std::vector<double> ray_S(3, 0);
    for (int j = 0; j < nRows; j++) {
        for (int i = 0; i < nColumns; i++) {
            std::vector<double> ray_c{ (((left + right) * i) / nColumns) - left, (((bottom + top) * j) / nRows) - bottom, -near };
            std::vector<double> color = raytrace(ray_S, ray_c, 1);
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
