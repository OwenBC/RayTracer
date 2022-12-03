// RayTracer.cpp 

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#define FLOATING_POINT_ERROR    0.000001 // Try larger value if this doesn't work
#define DEPTH                   2

void save_imageP6(int Width, int Height, const char* fname, unsigned char* pixels);
void save_imageP3(int Width, int Height, const char* fname, unsigned char* pixels);
class Sphere;
class Light;

// Input file data ----------------------------------
double near, left, right, top, bottom;
int nColumns, nRows;
std::vector<Sphere> sphereList;
std::vector<Light> lightList;
std::vector<double> bg_color;
std::vector<double> ambient_intensity;
std::string ofilename;

// Linalg functions ----------------------------------

// Dot product
double dot(std::vector<double> a, std::vector<double> b) {
    double sum = 0;
    for (int i = 0; i < a.size(); i++) sum += a[i] * b[i];
    return sum;
}

// Magnitude
double mag(std::vector<double> v) {
    double sum = 0;
    for (int i = 0; i < v.size(); i++) sum += std::pow(v[i], 2);
    return std::sqrt(sum);
}

// Normalize
void normalize(std::vector<double>& v) {
    double m = mag(v);
    for (int i = 0; i < v.size(); i++) v[i] /= m;
}

// Reflect
void reflect(std::vector<double>& v, std::vector<double> n) {
    double coef = 2 * dot(n, v);
    for (int i = 0; i < 3; i++) v[i] -= coef * n[i];
}

// Invert vector
void invert(std::vector<double>& v) {
    for (int i = 0; i < 3; i++) v[i] *= -1;
}

// Geometry Classes ----------------------------------

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
private:
    void transform_ray(std::vector<double>& ray_S, std::vector<double>& ray_c) {
        for (int i = 0; i < ray_S.size(); i++) {
            ray_S[i] -= pos[i];
            ray_S[i] /= scl[i];
            ray_c[i] /= scl[i];
        }
    }
public:
    Sphere() {};
    ~Sphere() {};
    bool intersect(std::vector<double> ray_S, std::vector<double> ray_c, double min_t) {
        // Inverse transform ray
        std::vector<double> inv_ray_S(ray_S);
        std::vector<double> inv_ray_c(ray_c);
        transform_ray(inv_ray_S, inv_ray_c);

        // Check collision
        double S_dot_c = dot(inv_ray_S, inv_ray_c);
        double square_c_mag = std::pow(mag(inv_ray_c), 2);
        double discriminant = std::pow(S_dot_c, 2) - square_c_mag * (std::pow(mag(inv_ray_S), 2) - 1);
        if (discriminant > 0) {
            // Calculate t at intersections and update output if min t on ray
            double t1 = -(S_dot_c + std::sqrt(discriminant)) / square_c_mag;
            double t2 = -(S_dot_c - std::sqrt(discriminant)) / square_c_mag;
            return t1 > min_t || t2 > min_t;
        }
        return false;
    }
    void intersect(std::vector<double> ray_S, std::vector<double> ray_c, double min_t, double* t, Sphere& out) {
        // Inverse transform ray
        std::vector<double> inv_ray_S(ray_S);
        std::vector<double> inv_ray_c(ray_c);
        transform_ray(inv_ray_S, inv_ray_c);

        // Check intercept
        double S_dot_c = dot(inv_ray_S, inv_ray_c); 
        double square_c_mag = std::pow(mag(inv_ray_c), 2);
        double discriminant = std::pow(S_dot_c, 2) - square_c_mag * (std::pow(mag(inv_ray_S), 2) - 1);
        if (discriminant > 0) {
            // Calculate t at intersections and update output if min t on ray
            double t1 = -(S_dot_c + std::sqrt(discriminant)) / square_c_mag; 
            double t2 = -(S_dot_c - std::sqrt(discriminant)) / square_c_mag;
            if (t1 > min_t) {
                if (t1 < *t || *t == -1) { *t = t1; out = *this; }
            }
            else if (t2 > min_t) {
                if (t2 < *t || *t == -1) { *t = t2; out = *this; }
            }
        }
    }
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
    // Check if light is visible from a given position
    bool visible(std::vector<double> p) {
        std::vector<double> c(pos);
        for (int i = 0; i < 3; i++) c[i] -= p[i];
        normalize(c);
        for (Sphere sphere : sphereList)
            if (sphere.intersect(p, c, 0)) return false;
        return true;
    }
private:
};

// Raytrace function ----------------------------------

std::vector<double> raytrace(std::vector<double> ray_S, std::vector<double> ray_c, bool background, int depth_budget, double min_t) {
    std::vector<double> color{ 0, 0, 0 };

    // Check recursive depth
    if (depth_budget == 0) return color;

    // Collision check
    double t = -1;
    Sphere hit_sphere;
    for (Sphere sphere : sphereList)
        sphere.intersect(ray_S, ray_c, min_t, &t, hit_sphere);

    // Return background if no collision
    if (t == -1) return background?bg_color:color;

    // Collision point
    std::vector<double> p_col(ray_S);
    for (int i = 0; i < 3; i++) p_col[i] += ray_c[i] * t;

    // Normal
    std::vector<double> norm(p_col);
    for (int i = 0; i < 3; i++) norm[i] = (norm[i] - hit_sphere.pos[i]) / std::pow(hit_sphere.scl[i],2);
    normalize(norm);

    // Eye direction vector
    std::vector<double> eye_dir(ray_c);
    invert(eye_dir);
    normalize(eye_dir);

    // Adjusted collision point
    std::vector<double> p_col_adj(p_col);
    for (int i = 0; i < 3; i++) p_col_adj[i] += eye_dir[i] * FLOATING_POINT_ERROR;

    // Ambient
    for (int i = 0; i < 3; i++) color[i] += hit_sphere.Ka * ambient_intensity[i] * hit_sphere.color[i];

    // Diffuse/Specular
    std::vector<double> l_dir;
    std::vector<double> l_dir_r;
    for (Light light : lightList){
        if (light.visible(p_col_adj)) {
            // light direction vector
            l_dir = light.pos;
            for (int i = 0; i < 3; i++) l_dir[i] -= p_col[i];
            normalize(l_dir);

            // reflected light direction vector
            l_dir_r = l_dir;
            reflect(l_dir_r, norm);

            // Apply light
            double diff_coef = hit_sphere.Kd * dot(norm, l_dir);
            if (diff_coef < 0) {
                std::vector<double> inv_norm(norm);
                invert(inv_norm);
                diff_coef = hit_sphere.Kd * dot(inv_norm, l_dir);
            }
            double spec_coef = hit_sphere.Ks * std::pow(dot(l_dir_r, eye_dir), hit_sphere.spec);
            for (int i = 0; i < 3; i++) color[i] += light.color[i] * (diff_coef * hit_sphere.color[i] + spec_coef);
        }
    }

    // Reflection
    std::vector<double> ref_c(eye_dir);
    reflect(ref_c, norm);
    invert(ref_c);
    std::vector<double> ref_color = raytrace(p_col_adj, ref_c, false, depth_budget - 1, 0);
    for (int i = 0; i < 3; i++) color[i] += hit_sphere.Kr * ref_color[i];
    return color;
}

void next_token(std::string& t, std::stringstream& ss) {
    std::getline(ss, t, ' ');
    while (t.compare("") == 0) {
        std::getline(ss, t, ' ');
    }
}

int main(int argc, char* argv[])
{
    // Parameter checking
    if (argc != 2) {
        std::cout << "Invalid number of parameters. Usage: RayTracer.exe <config-file-name>\n";
        return 1;
    }

    std::cout << "Reading from: " << argv[1] << "\n";
    // Read input file
    std::ifstream ifile(argv[1]);
    std::string ifile_line;
    std::string token;
    if (ifile.is_open())
        while (std::getline(ifile, ifile_line)) {
            std::replace(begin(ifile_line), end(ifile_line), '\t', ' ');
            std::stringstream ss(ifile_line);
            std::getline(ss, token, ' ');
            if (token.compare("NEAR") == 0) {
                next_token(token, ss);
                near = std::stof(token);
            }
            else if (token.compare("LEFT") == 0) {
                next_token(token, ss);
                left = std::stof(token);
            }
            else if (token.compare("RIGHT") == 0) {
                next_token(token, ss);
                right = std::stof(token);
            }
            else if (token.compare("BOTTOM") == 0) {
                next_token(token, ss);
                bottom = std::stof(token);
            }
            else if (token.compare("TOP") == 0) {
                next_token(token, ss);
                top = std::stof(token);
            }
            else if (token.compare("RES") == 0) {
                next_token(token, ss);
                nColumns= std::stoi(token);
                next_token(token, ss);
                nRows = std::stoi(token);
            }
            else if (token.compare("SPHERE") == 0) {
                Sphere sphere;
                next_token(token, ss);
                sphere.name = token;
                for (int i = 0; i < 3; i++) {
                    next_token(token, ss);
                    sphere.pos.push_back(std::stof(token));
                }
                for (int i = 0; i < 3; i++) {
                    next_token(token, ss);
                    sphere.scl.push_back(std::stof(token));
                }
                for (int i = 0; i < 3; i++) {
                    next_token(token, ss);
                    sphere.color.push_back(std::stof(token));
                }
                next_token(token, ss);
                sphere.Ka = std::stof(token);
                next_token(token, ss);
                sphere.Kd = std::stof(token);
                next_token(token, ss);
                sphere.Ks = std::stof(token);
                next_token(token, ss);
                sphere.Kr = std::stof(token);
                next_token(token, ss);
                sphere.spec = std::stoi(token);
                sphereList.push_back(sphere);
            }
            else if (token.compare("LIGHT") == 0) {
                Light light;
                next_token(token, ss);
                light.name = token;
                for (int i = 0; i < 3; i++) {
                    next_token(token, ss);
                    light.pos.push_back(std::stof(token));
                }
                for (int i = 0; i < 3; i++) {
                    next_token(token, ss);
                    light.color.push_back(std::stof(token));
                }
                lightList.push_back(light);
            }
            else if (token.compare("BACK") == 0) {
                for (int i = 0; i < 3; i++) {
                    next_token(token, ss);
                    bg_color.push_back(std::stof(token));
                }
            }
            else if (token.compare("AMBIENT") == 0) {
                for (int i = 0; i < 3; i++) {
                    next_token(token, ss);
                    ambient_intensity.push_back(std::stof(token));
                }
            }
            else if (token.compare("OUTPUT") == 0) {
                next_token(token, ss);
                ofilename = token;
            }
        }
    ifile.close();

    // Init grid
    unsigned char *pixels;
    pixels = new unsigned char[3 * nColumns * nRows];

    std::vector<double> ray_S(3, 0);
    double width = std::fabs(left - right);
    double height = std::fabs(bottom - top);
    for (int j = 0; j < nRows; j++) {
        for (int i = 0; i < nColumns; i++) {
            std::vector<double> ray_c{ ((width * i) / nColumns) + left, ((height * (nRows - j)) / nRows) + bottom, -near };
            std::vector<double> color = raytrace(ray_S, ray_c, true, DEPTH, near);
            for (int k = 0; k < 3; k++)
                pixels[3 * (i + j * nColumns) + k] = ((color[k] < 1)?color[k]:1) * 255;
        }
    }

    save_imageP6(nColumns, nRows, ofilename.c_str(), pixels);
}
