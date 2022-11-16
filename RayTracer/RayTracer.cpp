// RayTracer.cpp 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

// Sphere class
class Sphere
{
public:
    std::string name;
    std::vector<float> pos;
    std::vector<float> scl;
    std::vector<float> color;
    float Ka, Kd, Ks, Kr;
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
    std::vector<float> pos;
    std::vector<float> color;
    Light() {};
    ~Light() {};
private:
};

int main(int argc, char* argv[])
{
    // Parameter checking
    if (argc != 2) {
        std::cout << "Invalid parameters. Usage: RayTracer.exe <config-file-name>\n";
        return 1;
    }

    // Input file data
    float near, left, right, top, bottom;
    int nColumns, nRows;
    std::vector<Sphere> sphereList;
    std::vector<Light> lightList;
    std::vector<float> bg_color;
    std::vector<float> ambient_intensity;
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
