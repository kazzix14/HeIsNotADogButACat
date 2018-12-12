#ifndef VIEW_H
#define VIEW_H

typedef struct view
{
	struct private_varibles* p_vars;	
} View;

void View_construct(View* const);
void View_deconstruct(View* const);
void View_begin_2d(View* const, int const, int const);
void View_begin_3d(View* const, int const, int const);
void View_end();
void View_set_position(View* const, Vector* const);
void View_set_target_position(View* const, Vector* const);
void View_set_up_vector(View* const, Vector* const);
#endif
