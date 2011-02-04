/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Matrix math routines.
 *
 *      By Shawn Hargreaves.
 *
 *      See readme.txt for copyright information.
 */


#ifndef ALLEGRO_MATRIX_H
#define ALLEGRO_MATRIX_H

#ifdef __cplusplus
   extern "C" {
#endif


typedef struct MATRIX_f          /* transformation matrix (floating point) */
{
   float v[3][3];                /* scaling and rotation */
   float t[3];                   /* translation */
} MATRIX_f;

extern MATRIX_f identity_matrix_f;

void get_translation_matrix_f(MATRIX_f *m, float x, float y, float z);

void get_scaling_matrix_f(MATRIX_f *m, float x, float y, float z);

void get_x_rotate_matrix_f(MATRIX_f *m, float r);
void get_y_rotate_matrix_f(MATRIX_f *m, float r);
void get_z_rotate_matrix_f(MATRIX_f *m, float r);
void get_rotation_matrix_f(MATRIX_f *m, float x, float y, float z);

void get_align_matrix_f(MATRIX_f *m, float xfront, float yfront, float zfront, float xup, float yup, float zup);

void get_vector_rotation_matrix_f(MATRIX_f *m, float x, float y, float z, float a);

void get_transformation_matrix_f(MATRIX_f *m, float scale, float xrot, float yrot, float zrot, float x, float y, float z);

void get_camera_matrix_f(MATRIX_f *m, float x, float y, float z, float xfront, float yfront, float zfront, float xup, float yup, float zup, float fov, float aspect);

void qtranslate_matrix_f(MATRIX_f *m, float x, float y, float z);
void qscale_matrix_f(MATRIX_f *m, float scale);

void matrix_mul_f(AL_CONST MATRIX_f *m1, AL_CONST MATRIX_f *m2, MATRIX_f *out);

void apply_matrix_f(AL_CONST MATRIX_f *m, float x, float y, float z, float *xout, float *yout, float *zout);


#ifdef __cplusplus
   }
#endif

//#include "inline/matrix.inl"

#endif          /* ifndef ALLEGRO_MATRIX_H */


