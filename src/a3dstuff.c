/* a3dstuff.c */

#include <math.h>
#ifndef M_PI
#define M_PI 3.1415926535897932
#endif

#include <allegro5/allegro5.h>
#include <allegro5/a5_iio.h>

#include "a3dstuff.h"

LOCATOR cam_pos;
int current_mouse_vtx = -1;
VTX cam_vtx[MAX_VERTICES];



/* draw a line (for wireframe display) */
void 
wire_f(ALLEGRO_BITMAP *b, VTX *v1, VTX *v2)
{
   int col = MID(64, 255 + (v1->z+v2->z)/5 , 255);
   al_draw_line( v1->x, v1->y, v2->x, v2->y, al_map_rgb(col,col,col));
}


void 
morph_meshes(V3D_f * dest , V3D_f * src1, V3D_f * src2, int n_vtxs, float scale2) 
{
	
int v;
float scale1 = 1.0 - scale2;

	for (v=0; v<n_vtxs; v++)
	{
		dest[v].x = src1[v].x * scale1 + src2[v].x * scale2;
		dest[v].y = src1[v].y * scale1 + src2[v].y * scale2;
		dest[v].z = src1[v].z * scale1 + src2[v].z * scale2;

	}

}


int 
tri_is_clockwise(TRI * tris , int t)
{


       float a1= atan2( cam_vtx[tris[t].v2].y - cam_vtx[tris[t].v1].y,
                        cam_vtx[tris[t].v2].x - cam_vtx[tris[t].v1].x );

       float a2= atan2( cam_vtx[tris[t].v3].y - cam_vtx[tris[t].v1].y,
                        cam_vtx[tris[t].v3].x - cam_vtx[tris[t].v1].x );

       a2= (a2-a1);
       while (a2<0) a2+=(2 * M_PI);
       while (a2> (2*M_PI) ) a2-=(2 * M_PI);

       return ( a2 > M_PI );

}


/* 3d funcs */


void cartesian_to_polar_f ( float x , float y, float  z,
                          float * north, float * east, float * radius )

{

    * radius = sqrt( x*x + y*y + z*z);

    * east = atan2( z , x );
    * north = asin ( y / (*radius) );

}


void
polar_texture_mesh(ALLEGRO_BITMAP * tex, V3D_f * vtxs, int n_vtxs) 
{
	int v;
	int w,h;
	w= al_get_bitmap_width(tex);
	h= al_get_bitmap_height(tex);

	
    /*  get texture u & v by converting to polar co-ords */
    for (v=0 ; v < n_vtxs ; v++)
    {
    float north,east,radius;

       cartesian_to_polar_f(vtxs[v].x,vtxs[v].y,vtxs[v].z,
                          & north, & east, & radius);
       vtxs[v].u = (w /2) + east * w / ( M_PI * 2);
       vtxs[v].v = (h /2) - north * h /  M_PI ;

       vtxs[v].c = al_get_pixel(tex,vtxs[v].u,vtxs[v].v);

    }
}


void 
xy_texture_mesh(ALLEGRO_BITMAP * tex, V3D_f * vtxs, int n_vtxs) 
{

int v;
float xmin=0,ymin=0,xmax=0,ymax=0,xs,ys,xm,ym;

float va,vb;

	int w,h;
	w= al_get_bitmap_width(tex);
	h= al_get_bitmap_height(tex);


    /*  get texture u & v by converting to polar co-ords */
    for (v=0 ; v < n_vtxs ; v++)
    {
		va = vtxs[v].x; vb = -vtxs[v].z;
		if (va < xmin) xmin = va;	
		if (va > xmax) xmax = va;	
		if (vb < ymin) ymin = vb;	
		if (vb > ymax) ymax = vb;	
	}
	
	xm = (xmin + xmax) / 2.0; ym = (ymin + ymax) /2.0;
	xs = (xmax - xmin) / 1.0; ys = (ymax - ymin) /1.0; 

	for (v=0 ; v < n_vtxs ; v++)
    {
		va = vtxs[v].x; vb = -vtxs[v].z;

		vtxs[v].u = (w /2) + (va-xm) * w / xs;
		//vtxs[v].v = 16+ (tex->h /2) - (vb-ym) * tex->h / ys;
		vtxs[v].v = 128 + (vb-ym) * h / ys;

		vtxs[v].c = al_get_pixel(tex,vtxs[v].u,vtxs[v].v);

    }
}




void
check_new_edge( MESH * mesh, int v1, int v2 )
{ 
int e;

int n_edges = mesh->n_edges;
EDGE * edges = mesh->edges;
	
	for (e=0; e< n_edges; e++) {
		if  (  ( (edges[e].v1 == v1) && (edges[e].v2 == v2) ) 
			|| ( (edges[e].v1 == v2) && (edges[e].v2 == v1) ) )
			return; 
	}

	mesh->n_edges = ++n_edges;
	mesh->edges = realloc(edges , n_edges * sizeof(EDGE) );
	
	mesh->edges[e].v1 = v1;
	mesh->edges[e].v2 = v2;


}


/* fill in empty edges array */
void
make_mesh_edges( MESH * mesh) 
{

int t;
TRI * tris = mesh->tris;

	for (t=0; t< mesh->n_tris; t++) {
		
		check_new_edge( mesh, tris[t].v1, tris[t].v2); 
		check_new_edge( mesh, tris[t].v2, tris[t].v3); 
		check_new_edge( mesh, tris[t].v3, tris[t].v1); 
	}
}




/* copy a vertex array and apply a 2d transform on the x=0 plane */ 

void
trans_yz_mesh(V3D_f * dest , V3D_f * src, int n_vtxs,
			  float rot_x, float tran_y, float tran_z) 
{
int v;
float y0,z0,s0,c0;

	s0 = sin(rot_x); c0 = cos(rot_x);

	for (v=0; v< n_vtxs; v++) 
	{
		dest[v].x = src[v].x;  /* x is copied unchanged */
		y0 = src[v].y; 
		z0 = src[v].z;

		dest[v].y = c0 * y0 - s0 * z0 + tran_y;
		dest[v].z = s0 * y0 + c0 * z0 + tran_z;
	
	}
}


void
draw_triangle(  ALLEGRO_BITMAP * bmp,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3,
		ALLEGRO_COLOR c) 
{
		




}		




void 
draw_mesh_m( ALLEGRO_BITMAP * bmp, MESH * mesh, LOCATOR * cam_pos,  int flags ) 
{

int e,v,t;
float cz;

MATRIX_f matrix;

int n_vtxs = mesh->n_vtxs;
int n_edges = mesh->n_edges;
int n_tris = mesh->n_tris;
VTX * vtxs = mesh->vtxs;
EDGE * edges = mesh->edges;
TRI * tris = mesh->tris;


    get_transformation_matrix_f(&matrix, 1,
            cam_pos->rx, cam_pos->ry, - cam_pos->rz,
            cam_pos->x, cam_pos->y, - cam_pos->z);


    for(v=0; v< n_vtxs; v++) {
       apply_matrix_f(&matrix,
                      vtxs[v].x, vtxs[v].y, - vtxs[v].z,
                      &cam_vtx[v].x, &cam_vtx[v].y, &cam_vtx[v].z);


       cam_vtx[v].y *= (al_get_bitmap_width(bmp) / al_get_bitmap_height(bmp));     //aspect hack

//       cam_vtx[v].u = vtxs[v].u;
//       cam_vtx[v].v = vtxs[v].v;
//       cam_vtx[v].c = vtxs[v].c;

       persp_project_f(cam_vtx[v].x, cam_vtx[v].y, cam_vtx[v].z,
                       &cam_vtx[v].x, &cam_vtx[v].y);
    }



	
	if (flags & DRAW_MESH_SOLID) { 

		for( t=0 ; t < n_tris ; t++) {

         //if ( tri_is_clockwise(tris,t) ) {

          cz = (cam_vtx[tris[t].v1].z +
                cam_vtx[tris[t].v2].z +
                cam_vtx[tris[t].v3].z + 900) /  10;


            draw_triangle( bmp,
                 cam_vtx[tris[t].v1].x , cam_vtx[tris[t].v1].y ,
                 cam_vtx[tris[t].v2].x , cam_vtx[tris[t].v2].y ,
                 cam_vtx[tris[t].v3].x , cam_vtx[tris[t].v3].y ,
                 al_map_rgb(0,MID(0,cam_vtx[tris[t].v1].x,255),MID(64,128+cz,255) )
                 );
         // }
        }
	}

	/* hack to catchh NULL textures */
	if (!(mesh->texture) && (flags & DRAW_MESH_ATEX))
	{ 
		flags &= ~DRAW_MESH_ATEX;
		flags |= DRAW_MESH_GOURAUD;
	
	}


	if (flags & DRAW_MESH_GOURAUD) { 

        for( t=0 ; t < n_tris ; t++) {

         
		 //if ( flags & DRAW_MESH_HIDDEN || tri_is_clockwise(tris,t) ) {

	
//          triangle3d_f(bmp,POLYTYPE_GCOL | POLYTYPE_ZBUF,mesh->texture,
//                   &cam_vtx[tris[t].v1], &cam_vtx[tris[t].v2], &cam_vtx[tris[t].v3]);
          //}
        }

	}

	if (flags & DRAW_MESH_ATEX) { 

		for( t=0 ; t < n_tris ; t++) {
        
        
//         if ( !tri_is_clockwise(tris,t) ) {

//          triangle3d_f(bmp,POLYTYPE_ATEX | POLYTYPE_ZBUF,mesh->texture,
//                   &cam_vtx[tris[t].v1], &cam_vtx[tris[t].v2], &cam_vtx[tris[t].v3]);

//         }
        }

	}		

    if ( flags & DRAW_MESH_EDGES ) {
         for(e=0; e < n_edges ; e++) {
           wire_f( bmp, &cam_vtx[edges[e].v1] , &cam_vtx[edges[e].v2] );
         }
	}
	
	if (flags & DRAW_MESH_VERTEX_ID) { 
   		 for(v=0; v < n_vtxs ; v++) {
           //textprintf(bmp, font, cam_vtx[v].x - 4, cam_vtx[v].y -4 , makecol(255,255,255), "%d", v);
         }
	}

}

/* magical formulae for constructing rotation matrices */
#define FLOATSINCOS(x, s, c)  _AL_SINCOS((x) * AL_PI / 128.0, s ,c)
#define floattan(x)           tan((x) * AL_PI / 128.0)


#define MAKE_ROTATION_f(x, y, z)                \
   float sin_x, cos_x;				\
   float sin_y, cos_y;				\
   float sin_z, cos_z;				\
   float sinx_siny, cosx_siny;			\
						\
   FLOATSINCOS(x, sin_x, cos_x);		\
   FLOATSINCOS(y, sin_y, cos_y);		\
   FLOATSINCOS(z, sin_z, cos_z);		\
						\
   sinx_siny = sin_x * sin_y;			\
   cosx_siny = cos_x * sin_y;




#define R00_f (cos_y * cos_z)
#define R10_f ((sinx_siny * cos_z) - (cos_x * sin_z))
#define R20_f ((cosx_siny * cos_z) + (sin_x * sin_z))

#define R01_f (cos_y * sin_z)
#define R11_f ((sinx_siny * sin_z) + (cos_x * cos_z))
#define R21_f ((cosx_siny * sin_z) - (sin_x * cos_z))

#define R02_f (-sin_y)
#define R12_f (sin_x * cos_y)
#define R22_f (cos_x * cos_y)




/* get_transformation_matrix_f:
 *  Floating point version of get_transformation_matrix().
 */
void get_transformation_matrix_f(MATRIX_f *m, float scale, float xrot, float yrot, float zrot, float x, float y, float z)
{
   MAKE_ROTATION_f(xrot, yrot, zrot);
   ASSERT(m);

   m->v[0][0] = R00_f * scale;
   m->v[0][1] = R01_f * scale;
   m->v[0][2] = R02_f * scale;

   m->v[1][0] = R10_f * scale;
   m->v[1][1] = R11_f * scale;
   m->v[1][2] = R12_f * scale;

   m->v[2][0] = R20_f * scale;
   m->v[2][1] = R21_f * scale;
   m->v[2][2] = R22_f * scale;

   m->t[0] = x;
   m->t[1] = y;
   m->t[2] = z;
}




/* cross_productf:
 *  Floating point version of cross_product().
 */
void cross_product_f(float x1, float y1, float z1, float x2, float y2, float z2, float *xout, float *yout, float *zout)
{
   ASSERT(xout);
   ASSERT(yout);
   ASSERT(zout);

   *xout = (y1 * z2) - (z1 * y2);
   *yout = (z1 * x2) - (x1 * z2);
   *zout = (x1 * y2) - (y1 * x2);
}


float dot_product_f(float x1, float y_1, float z1, float x2, float y2, float z2)
{
   return (x1 * x2) + (y_1 * y2) + (z1 * z2);
}




float _persp_xscale_f, _persp_yscale_f, _persp_xoffset_f, _persp_yoffset_f;


void persp_project_f(float x, float y, float z, float *xout, float *yout)
{
   float z1 = 1.0f / z;
   *xout = ((x * z1) * _persp_xscale_f) + _persp_xoffset_f;
   *yout = ((y * z1) * _persp_yscale_f) + _persp_yoffset_f;
}


/* apply_matrix_f:
 *  Floating point vector by matrix multiplication routine.
 */
void apply_matrix_f(AL_CONST MATRIX_f *m, float x, float y, float z,
		    float *xout, float *yout, float *zout)
{
#define CALC_ROW(n) (x * m->v[(n)][0] + y * m->v[(n)][1] + z * m->v[(n)][2] + m->t[(n)])
   *xout = CALC_ROW(0);
   *yout = CALC_ROW(1);
   *zout = CALC_ROW(2);
#undef CALC_ROW
}



/*  Function : draw_mesh


*/

void draw_mesh( ALLEGRO_BITMAP * bmp, V3D_f * vtxs,  int n_vtxs,
                              EDGE * edges,  int n_edges,
                              TRI * tris ,   int n_tris,
                              LOCATOR * cam_pos,  int mode ) {

int e,v,t;
float cz;

MATRIX_f matrix;


    get_transformation_matrix_f(&matrix, 1,
            cam_pos->rx, cam_pos->ry, - cam_pos->rz,
            cam_pos->x, cam_pos->y, - cam_pos->z);


    for(v=0; v< n_vtxs; v++) {
       apply_matrix_f(&matrix,
                      vtxs[v].x, vtxs[v].y, - vtxs[v].z,
                      &cam_vtx[v].x, &cam_vtx[v].y, &cam_vtx[v].z);


       cam_vtx[v].y *= 1.3333;     //aspect hack

       //cam_vtx[v].u = vtxs[v].u;
       //cam_vtx[v].v = vtxs[v].v;
       //cam_vtx[v].c = vtxs[v].c;

       persp_project_f(cam_vtx[v].x, cam_vtx[v].y, cam_vtx[v].z,
                       &cam_vtx[v].x, &cam_vtx[v].y);
    }




    switch (mode)
    {
    case 0:   /* wire frame */

         for(e=0; e < n_edges ; e++) {
           wire_f( bmp, &cam_vtx[edges[e].v1] , &cam_vtx[edges[e].v2] );
         }
//         textprintf(bmp,font,0,0,makecol(255,255,255),"wire");

   		 for(v=0; v < n_vtxs ; v++) {
//         textprintf(bmp, font, cam_vtx[v].x - 4, cam_vtx[v].y -4 , makecol(255,255,255), "%d", v);
         }
//         textprintf(bmp,font,0,0,makecol(255,255,255),"wire");
         break;

    case 1:   /* solid triangle */

         for( t=0 ; t < n_tris ; t++) {

         //if ( tri_is_clockwise(tris,t) ) {

          cz = (cam_vtx[tris[t].v1].z +
                cam_vtx[tris[t].v2].z +
                cam_vtx[tris[t].v3].z + 900) /  10;


            draw_triangle( bmp,
                 cam_vtx[tris[t].v1].x , cam_vtx[tris[t].v1].y ,
                 cam_vtx[tris[t].v2].x , cam_vtx[tris[t].v2].y ,
                 cam_vtx[tris[t].v3].x , cam_vtx[tris[t].v3].y ,
                 al_map_rgb(0,MID(0,cam_vtx[tris[t].v1].x,255),MID(64,128+cz,255) )
                 );
         // }
        }

//        textprintf(bmp,font,0,0,makecol(255,255,255),"flat");

        break;

    case 2:   /* gouraud shaded triangle */
        for( t=0 ; t < n_tris ; t++) {

         //if ( tri_is_clockwise(tris,t) ) {



//          triangle3d_f(bmp,POLYTYPE_GCOL | POLYTYPE_ZBUF,lowerface_texture,
//                   &cam_vtx[tris[t].v1], &cam_vtx[tris[t].v2], &cam_vtx[tris[t].v3]);
          //}
        }
//        textprintf(bmp,font,0,0,makecol(255,255,255),"GCOL");
        break;

    case 3:   /* textured triangle */
        for( t=0 ; t < n_tris ; t++) {
        
//        V3D_f * poly[3];
        
 //       poly[0] = &cam_vtx[tris[t].v1];
 //       poly[1] = &cam_vtx[tris[t].v2];
 //       poly[2] = &cam_vtx[tris[t].v3];
        
//         if ( !tri_is_clockwise(tris,t) ) {
//         polygon3d_f(bmp,POLYTYPE_ATEX | POLYTYPE_ZBUF ,texture, 3, poly );
 //         }
 
//          triangle3d_f(bmp,POLYTYPE_ATEX_LIT | POLYTYPE_ZBUF,lowerface_texture,
//                   &cam_vtx[tris[t].v1], &cam_vtx[tris[t].v2], &cam_vtx[tris[t].v3]);

        }
//        textprintf(bmp,font,0,0,makecol(255,255,255),"ATEX");
        break;
    }



//    textprintf(bmp,font,300,390,makecol(255,255,255),"z= %f",cam_vtx[0].z);

}
    




