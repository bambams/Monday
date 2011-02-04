/* a3dstuff.h */

#ifndef A3DSTUFF_H
#define A3DSTUFF_H

#ifdef __cplusplus
   extern "C" {
#endif

#include <allegro5/allegro5.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <allegro5/a5_iio.h>

#ifdef __cplusplus
}
#endif



#include "matrix.h"


typedef struct VTX
{
   float x, y, z;
} VTX;

//#ifndef V3D_f
//included for allegro 4.9
typedef struct V3D_f                /* a 3d point (floating point version) */
{
   float x, y, z;                   /* position */
   float u, v;                      /* texture map coordinates */
   ALLEGRO_COLOR c;                           /* color */
} V3D_f;
//#endif

typedef struct EDGE
{
   int v1,v2,vmid;              /* vmid is an index to the next LOD */
} EDGE;

typedef struct TRI              /* 3 vertices makes a triangle */
{
   int v1, v2, v3;
} TRI;


typedef struct TRI_UV            /* 3 vertices makes a triangle */
{
   int p1, p2, p3;
   float u1, u2, u3;
   float v1, v2, v3;
} TRI_UV;

typedef struct TRI_UVI            /* 3 vertices makes a triangle */
{
   int p1, p2, p3;
   int uv1, uv2, uv3;
} TRI_UVI;


typedef struct UV_f
{
	float u,v;
} UV_f;

typedef struct LOCATOR
{
   float rx,ry,rz;
   float x,y,z;
   //MATRIX_f matrix;				/* stored precalculated before drawing */

} LOCATOR;


typedef struct MESH
{
	int	n_vtxs, n_edges, n_tris;
	VTX * vtxs;
	EDGE * edges;
	TRI * tris;
	ALLEGRO_BITMAP * texture;
	int gltex;
} MESH;


typedef struct VTX_N
{
	float x,y,z;
	float nx,ny,nz;		/* Normal */
	int c;
	float u,v;
} VTX_N;  

/* Used to store MD2 models */
typedef struct MESH_TUVI
{
	int	n_vtxs, n_edges, n_tris;
	VTX_N * vtxs;
	EDGE * edges;
	TRI_UVI * tris;
    UV_f *  uvs;
	ALLEGRO_BITMAP * texture;
	int gltex;
} MESH_TUVI;


typedef struct ANIMORPH {
	int f1;		
	int f2;
	float factor;	//morph factor (0.0 = 100% f1, 1.0 = 100% f2)

} ANIMORPH;


typedef struct FRAME
{   char name[16]; /* just junk */     
    VTX_N * vtxs; 
} FRAME;




#define MAX_VERTICES 10242

/* global camera stuff */

extern LOCATOR cam_pos;

extern VTX cam_vtx[];

extern int current_mouse_vtx;


/* functions */

void 
wire_f(ALLEGRO_BITMAP *b, VTX *v1, VTX *v2); /* draw a line (for wireframe display) */

void 
morph_meshes(V3D_f * dest , V3D_f * src1, V3D_f * src2, int n_vtxs, float scale1);

void
make_mesh_edges( MESH * mesh); 

int 
tri_is_clockwise(TRI * tris , int t);

void cartesian_to_polar_f ( float x , float y, float  z,
                          float * north, float * east, float * radius );

void
polar_texture_mesh(ALLEGRO_BITMAP * tex, V3D_f * vtxs, int n_vtxs);

void 
xy_texture_mesh(ALLEGRO_BITMAP * tex, V3D_f * vtxs, int n_vtxs);


/* copy a vertex array and apply a 2d transform on the x=0 plane */ 
void
trans_yz_mesh(V3D_f * dest , V3D_f * src, int n_vtxs,
			  float rot_x, float tran_y, float tran_z); 


void
draw_mesh_m( ALLEGRO_BITMAP * bmp, MESH * mesh, LOCATOR * cam_pos,  int flags );


/* older version of draw_mesh which takes individual mesh data instead of a single MESH struct */
void 
draw_mesh( ALLEGRO_BITMAP * bmp, V3D_f * vtxs,  int n_vtxs,
                              EDGE * edges,  int n_edges,
                              TRI * tris ,   int n_tris,
                              LOCATOR * cam_pos,  int mode );




/* from Allegro 4 */



extern float _persp_xscale_f, _persp_yscale_f, _persp_xoffset_f, _persp_yoffset_f;
float dot_product_f(float x1, float y_1, float z1, float x2, float y2, float z2);
void persp_project_f(float x, float y, float z, float *xout, float *yout);



/* from Allegro 4  3d.h */
float polygon_z_normal_f(AL_CONST V3D_f *v1, AL_CONST V3D_f *v2, AL_CONST V3D_f *v3);





/* flags value */

#define DRAW_MESH_EDGES		 0x02
#define DRAW_MESH_VERTEX_ID  0x04
#define DRAW_MESH_SOLID		 0x08
#define DRAW_MESH_GOURAUD	 0x10
#define DRAW_MESH_ATEX		 0x20
#define DRAW_MESH_VERTEX_NORMAL 0x40



#ifdef __cplusplus
	}
#endif

#endif 