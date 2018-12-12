#ifndef VIEW_H
#define VIEW_H

#include "vector3d.h"

typedef struct view
{
	Vector3D position,
		 traget_position,
		 up_vetor;
	struct private_variables* p_vars;	
} View;

View* View_new();
void View_release(View* const);
void View_begin_2d(View* const, int const, int const);
void View_begin_3d(View* const, int const, int const);
void View_end();
#endif
