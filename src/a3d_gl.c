/* a3d_gl.c */

#include <GL/glu.h>
#include <string.h>
#include <math.h>

#include "allegro5/a5_ttf.h"

#include "a3dstuff.h"


void
drawgl_V3D_f_tex(V3D_f * vtx , float w, float h)
{
	glTexCoord2f (vtx->u / w, 1.0 - vtx->v / h);
	glVertex3f(vtx->x, vtx->y, vtx->z);
	
}

void
drawgl_V3D_f_uv(V3D_f * vtx)
{
	float r,g,b;
	//ALLEGRO_COLOR c = makecol(;
	//al_unmap_rgb_f( c, &r, &g, &b);
	//glColor3f(r/255.0, g/255.0, b/255.0);
	glVertex3f(vtx->x, vtx->y, vtx->z);
	
}


void
triangle(  ALLEGRO_BITMAP * bmp,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3,
		ALLEGRO_COLOR c) 
{
		


}		




ALLEGRO_FONT * vertex_font;

void 
draw_mesh_GL( ALLEGRO_BITMAP * bmp, MESH * mesh, LOCATOR * cam_pos,  int flags ) 
{

int e,v,t;
float cz;

MATRIX_f matrix;

int n_vtxs = mesh->n_vtxs;
int n_edges = mesh->n_edges;
int n_tris = mesh->n_tris;
V3D_f * vtxs = mesh->vtxs;
EDGE * edges = mesh->edges;
TRI * tris = mesh->tris;

    vertex_font = al_ttf_load_font("media/DejaVuSans.ttf", 10, 0);

    get_transformation_matrix_f(&matrix, 1,
            cam_pos->rx, cam_pos->ry,  cam_pos->rz,
            cam_pos->x, cam_pos->y,  cam_pos->z);


    for(v=0; v< n_vtxs; v++) {

/*		apply_matrix_f(&matrix,
                      vtxs[v].x, vtxs[v].y,  vtxs[v].z,
                      &cam_vtx[v].x, &cam_vtx[v].y, &cam_vtx[v].z);

/*

       cam_vtx[v].y *= 1.3333;     //aspect hack
*/

		cam_vtx[v].x = vtxs[v].x;	
		cam_vtx[v].y = vtxs[v].y;	
		cam_vtx[v].z = vtxs[v].z;	

       //cam_vtx[v].u = vtxs[v].u;
       //cam_vtx[v].v = vtxs[v].v;
       //cam_vtx[v].c = vtxs[v].c;

//       persp_project_f(cam_vtx[v].x, cam_vtx[v].y, cam_vtx[v].z,
//                       &cam_vtx[v].x, &cam_vtx[v].y);
    }


    if ( flags & DRAW_MESH_EDGES ) {
         for(e=0; e < n_edges ; e++) {
           wire_f( bmp, &cam_vtx[edges[e].v1] , &cam_vtx[edges[e].v2] );
         }
	}
	
	if (flags & DRAW_MESH_VERTEX_ID) { 
   		 for(v=0; v < n_vtxs ; v++) {
//			char vs[]="     ";
//	   		strprintf(vs,"%d",v);
			al_font_textprintf_centre(vertex_font, cam_vtx[v].x , cam_vtx[v].y, "%d", v );
         }
	}

	
	if (flags & DRAW_MESH_SOLID) { 

		for( t=0 ; t < n_tris ; t++) {

         //if ( tri_is_clockwise(tris,t) ) {

          cz = (cam_vtx[tris[t].v1].z +
                cam_vtx[tris[t].v2].z +
                cam_vtx[tris[t].v3].z + 900) /  10;


            triangle( bmp,
                 cam_vtx[tris[t].v1].x , cam_vtx[tris[t].v1].y ,
                 cam_vtx[tris[t].v2].x , cam_vtx[tris[t].v2].y ,
                 cam_vtx[tris[t].v3].x , cam_vtx[tris[t].v3].y ,
                 al_map_rgb(0,MID(0,cam_vtx[tris[t].v1].x,255),MID(64,128+cz,255) )
                 );
         // }
        }
	}


	if (flags & DRAW_MESH_GOURAUD) { 

		glShadeModel(GL_SMOOTH);
        glBegin(GL_TRIANGLES);
        for( t=0 ; t < n_tris ; t++) {

				drawgl_V3D_f_uv(&cam_vtx[tris[t].v1]);
				drawgl_V3D_f_uv(&cam_vtx[tris[t].v2]);
				drawgl_V3D_f_uv(&cam_vtx[tris[t].v3]);

 
        }
        glEnd();

	}

	if (flags & DRAW_MESH_ATEX) { 

		glColor3f(1.0, 1.0, 1.0);   // removed because maybe this is buggering ATIs
		glBindTexture (GL_TEXTURE_2D, mesh->gltex);
		glBegin(GL_TRIANGLES);
		for( t=0 ; t < n_tris ; t++) {
			int w=512, h=512; 
		
				drawgl_V3D_f_tex(&cam_vtx[tris[t].v1], w, h);
				drawgl_V3D_f_tex(&cam_vtx[tris[t].v2], w, h);
				drawgl_V3D_f_tex(&cam_vtx[tris[t].v3], w, h);
 
        }
        glEnd();

		glBindTexture (GL_TEXTURE_2D, 0);

	}		

}




