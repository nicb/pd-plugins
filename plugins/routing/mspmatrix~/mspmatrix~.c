/* $Id: mspmatrix~.c 26 2006-02-04 15:44:44Z nicb $
 *
 *  *  mspmatrix~: msp matrix~ compatible object for Pure Data (PD)
 *  *  Copyright (C) 2004  Amalia De Goetzen, Marco Braggion 
 *  *  and the fabulous 'studenti-nerds' students group 
 *  *  of the Classe di Musica Elettronica, Conservatorio Pollini (Padova, Italy) 
 *  *
 *  *  This program is free software; you can redistribute it and/or modify
 *  *  it under the terms of the GNU General Public License as published by
 *  *  the Free Software Foundation; either version 2 of the License, or
 *  *  (at your option) any later version.
 *  *
 *  *  This program is distributed in the hope that it will be useful,
 *  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  *  GNU General Public License for more details.
 *  *
 *  *  You should have received a copy of the GNU General Public License
 *  *  along with this program (it should be in the LICENSE file included in the
 *  *  directory where this file was found); if not, write to the Free Software
 *  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  *
 *  *  Author contacts: studenti-nerds@lists.sme-ccppd.org
 *  *
 *  
 *  
 */
#include <m_pd.h>
#include <stdlib.h>

static t_class *mspmatrix_tilde_class;

#if defined(__GNUC__)
#	define	_INLINE_	inline
#else
#	define	_INLINE_	/* empty */
#endif	 			/* defined(__GNUC__) */

typedef struct _matrix_node
{
	t_int in, out; 		/* these must be zero-offset I/O indexes */
	
	struct _matrix_node  *previous;
	struct _matrix_node  *next;
}t_matrix_node;

typedef struct _mspmatrix_tilde
{
	t_object  x_obj;
	t_sample f;
	t_int 			inputs, outputs;
	t_matrix_node 		*connections;
	t_matrix_node		*active;
}t_mspmatrix_tilde;

_INLINE_ static t_sample *matrix_vector_input(const t_mspmatrix_tilde *x, t_signal **sp, t_int i)
{
		return sp[i]->s_vec;
}

_INLINE_ static t_sample *matrix_vector_output(const t_mspmatrix_tilde *x, t_signal **sp, t_int o)
{
		return sp[x->inputs+o]->s_vec;
}

static t_int *mspmatrix_tilde_perform(t_int *w)
{
    t_mspmatrix_tilde *x    = (t_mspmatrix_tilde *) (w[1]);
    t_signal **sp           = (t_signal **) (w[2]);
    t_int samples           = (t_int) (w[3]);

    t_matrix_node *cur	    = x->active;

    while(cur != (t_matrix_node *) NULL)
    {
	    t_int s		= samples;
	    t_sample *input 	= matrix_vector_input(x, sp, cur->in);
	    t_sample *output 	= matrix_vector_output(x, sp, cur->out);

	    while(s--) *output++ = *input++;

	    cur = cur->next;
    }
    
    return (w+4);
}

static void mspmatrix_tilde_dsp (t_mspmatrix_tilde *x, t_signal **sp)
{
    dsp_add(mspmatrix_tilde_perform, 3, x, sp, sp[0]->s_n);

}

static void matrix_initializer(t_int rows, t_int cols, t_matrix_node * matrix)
{
	t_int r = 0, c = 0;

	t_matrix_node * cur = matrix;
	
	for(r = 0; r<rows; ++r, ++cur)
		for(c = 0; c<cols; ++c, ++cur) 
		{
			cur->in = r;
			cur->out = c;
			
			cur->previous = (t_matrix_node *) NULL;
			cur->next = (t_matrix_node *) NULL;
		}
	
		
}

static void io_initializer(t_int inlets, t_int outlets, t_mspmatrix_tilde * x)
{
	t_int i = 0;
	
	for (i = 1; i < inlets; ++i)
		inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
		
	for (i = 0; i < outlets; ++i)
		outlet_new(&x->x_obj, &s_signal);
}

void *mspmatrix_tilde_new (t_symbol *s, int argc, t_atom *argv)
{
	t_int inlets = 2, outlets = 2;  
	
	t_mspmatrix_tilde *x = (t_mspmatrix_tilde*)pd_new(mspmatrix_tilde_class);

	switch (argc)
	{
		default: 
			
		case 2:
			outlets = atom_getfloat(argv+1);
		case 1:
			inlets = atom_getfloat(argv);
		case 0:

	}
	
	x->connections = malloc(sizeof(t_matrix_node)*inlets*outlets);   
	
	matrix_initializer(inlets, outlets, x->connections);
	
	x->active = (t_matrix_node *) NULL;

	io_initializer (inlets, outlets, x);

	return(void *)x;

}


static void mspmatrix_tilde_destroy(t_mspmatrix_tilde * x)
{
	free(x->connections);
}

void mspmatrix_tilde_setup (void)
{
	float f = 0;
	
	mspmatrix_tilde_class = class_new(gensym("mspmatrix~"), (t_newmethod)mspmatrix_tilde_new, 
			        (t_method)mspmatrix_tilde_destroy, 
				sizeof(t_mspmatrix_tilde), CLASS_DEFAULT, A_GIMME, 0);

	class_addmethod(mspmatrix_tilde_class, (t_method)mspmatrix_tilde_dsp, gensym("dsp"), 0);
	CLASS_MAINSIGNALIN(mspmatrix_tilde_class, t_mspmatrix_tilde, f);
}
