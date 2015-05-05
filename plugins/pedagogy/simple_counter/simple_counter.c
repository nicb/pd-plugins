/*
 * $Id: simple_counter.c 26 2006-02-04 15:44:44Z nicb $
 *
 *  simple_counter: a simple counter object for Pure Data (PD)
 *  Copyright (C) 2004  Amalia De Goetzen, Marco Braggion 
 *  and the fabulous 'studenti-nerds' students group 
 *  of the Classe di Musica Elettronica, Conservatorio Pollini (Padova, Italy) 
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program (it should be in the LICENSE file included in the
 *  directory where this file was found); if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Author contacts: studenti-nerds@lists.sme-ccppd.org
 *
 */

#include <m_pd.h>

static t_class *simple_counter_class;

typedef struct _sc_core
{
	t_float start, end; 	/* lower and upper limits */	
} sc_core;

typedef struct _sc_dflt
{
	t_float start, end; 	/* stores the default values */
}sc_dflt;

typedef struct _simple_counter {
	t_object  x_obj;
	t_float counter; 		/* counter */
	sc_core curr;
	sc_dflt dflt;
} t_simple_counter;

static void output_counter(t_simple_counter *x)
{
	outlet_float(x->x_obj.ob_outlet, x->counter);
}

void simple_counter_bang(t_simple_counter *x)
{
	if(x->curr.end == -1 || x->counter < x->curr.end) 
		++x->counter;
	
	output_counter(x);
}


void simple_counter_clear(t_simple_counter *x)
{
	x->counter = x->curr.start;

	output_counter(x);
}

void simple_counter_reset(t_simple_counter *x, t_floatarg f1, t_floatarg f2)
{
        x->dflt.start = f1;
		
	x->dflt.end = f2; 

	simple_counter_clear(x);
}



void simple_counter_set(t_simple_counter *x, t_floatarg f1, t_floatarg f2)
{
	if(f2 < f1 && f2!=-1)
		post("set limits wrong: end (%.2f) < start (%.2f)", f2, f1);
			
	x->curr.start = f1;
	x->curr.end = f2 >= 0 ? f2 : -1;
	
	simple_counter_clear(x);
}

void *simple_counter_new(t_symbol *s, t_int argc, t_atom argv[])
{
	t_float start = 0, end = 0;
	t_simple_counter *x = (t_simple_counter *)pd_new(simple_counter_class);
	outlet_new(&x->x_obj, &s_float);
	switch (argc)
	{
		case 0: start = 0; 
			end = -1;
			break;
		default:
		case 2: end=atom_getfloat(&argv[1]);
		        start=atom_getfloat(&argv[0]);
			break;
		case 1: start=atom_getfloat(&argv[0]);
			end=-1;
			
	}
	
	simple_counter_set(x, start, end);	
	return (void *)x;
}

void simple_counter_setup(void) 
{
	simple_counter_class = class_new(gensym("simple_counter"), (t_newmethod)simple_counter_new, 0, 
			       sizeof(t_simple_counter), CLASS_DEFAULT,	A_GIMME, 0);
	class_addbang(simple_counter_class, simple_counter_bang);
	class_addmethod(simple_counter_class, (t_method) simple_counter_clear, gensym("clear"), 0);
	class_addmethod(simple_counter_class, (t_method) simple_counter_set, gensym("set"), A_DEFFLOAT, A_DEFFLOAT, 0);
	class_addmethod(simple_counter_class, (t_method) simple_counter_set, gensym("reset"), A_DEFFLOAT, A_DEFFLOAT, 0);
}
