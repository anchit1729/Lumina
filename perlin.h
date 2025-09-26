//
// Created by Anchit Mishra on 2025-09-24.
//

#ifndef LUMINA_PERLIN_H
#define LUMINA_PERLIN_H

#include <lumina.h>
#include <vec3.h>

class perlin {
public:
    perlin() {
        for (int i = 0; i < point_count; i++)   {
//            randfloat[i] = random_double();
            randvec[i] = unit(vec3::random(-1, 1));
        }

        perlin_generate_perm(perm_x);
        perlin_generate_perm(perm_y);
        perlin_generate_perm(perm_z);
    }

    double noise(const point3& p) const {
        auto u = p.x - std::floor(p.x);
        auto v = p.y - std::floor(p.y);
        auto w = p.z - std::floor(p.z);
        // Get rid of Hermitian smoothing to implement random unit vectors on lattice points and use a dot product to
        // move the min and max off the lattice
//        // Hermitian smoothing - to help get rid of grid features
//        u = u*u*(3-2*u);
//        v = v*v*(3-2*v);
//        w = w*w*(3-2*w);

//        // Original version - no smoothing
//        auto i = int(4 * p.x) & 255;
//        auto j = int(4 * p.y) & 255;
//        auto k = int(4 * p.z) & 255;

        auto i = int(std::floor(p.x));
        auto j = int(std::floor(p.y));
        auto k = int(std::floor(p.z));
        vec3 c[2][2][2];

        for (int di=0; di<2; di++) {
            for (int dj=0; dj<2; dj++) {
                for (int dk=0; dk<2; dk++) {
                    // replace the random float with a random vector
//                    c[di][dj][dk] = randfloat[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
                    c[di][dj][dk] = randvec[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
                }
            }
        }


        // Original version - no smoothing
//        return randfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
        // Trilinear (+ Hermitian) interpolation
//        return trilinear_interp(c, u, v, w);
        // Perlin interpolation (vector version)
        return perlin_interp(c, u, v, w);
    }

    double turb(const point3& p, int depth) const {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return std::fabs(accum);
    }

private:
    static const int point_count = 256;
    vec3 randvec[point_count]; // replace float points with unit vectors.
//    double randfloat[point_count];
    int perm_x[point_count];
    int perm_y[point_count];
    int perm_z[point_count];

    static void perlin_generate_perm(int* p) {
        for (int i = 0; i < point_count; i++)   {
            p[i] = i;
        }
        permute(p, point_count);
    }

    static void permute(int* p, int n) {
        for (int i = n-1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double trilinear_interp(double c[2][2][2], double u, double v, double w) {
        auto accum = 0.0;
        for (int i = 0; i < 2; i++)  {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    accum +=  (i*u + (1-i)*(1-u))
                            * (j*v + (1-j)*(1-v))
                            * (k*w + (1-k)*(1-w))
                            * c[i][j][k];
                }
            }
        }
        return accum;
    }

    static double perlin_interp(vec3 c[2][2][2], double u, double v, double w) {
        auto uu = u*u*(3-2*u);
        auto vv = v*v*(3-2*v);
        auto ww = w*w*(3-2*w);
        auto accum = 0.0;

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    vec3 weight_v(u-i, v-j, w-k);
                    accum += (i*uu + (1-i)*(1-uu))
                            *(j*vv + (1-j)*(1-vv))
                            *(k*ww + (1-k)*(1-ww))
                            *dot(c[i][j][k], weight_v);
                }
            }
        }
        return accum;
    }
};

#endif //LUMINA_PERLIN_H
