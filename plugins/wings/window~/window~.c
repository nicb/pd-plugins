/* $Id: window~.c 43 2006-03-23 11:09:03Z mb $
 * window~.c: 
 * 
 * 1] block sized windowing of data signal
 * 2] default: hanning window
 *
 * left input: 	if audio -> signal input
 * 		if control -> message w/ window name
 *
 * output:	windowed audio signal
 *
 * Window is tabulated at 	1] creation time 
 * 				2] when receiving a message
 * 				3] when changing block size
 *
 * 				
  *  *  *  Copyright (C) 2006  Amalia De Goetzen, Marco Braggion 
  *  *  *  and the fabulous 'studenti-nerds' students group 
  *  *  *  of the Classe di Musica Elettronica, Conservatorio Pollini (Padova, Italy) 
  *  *  *
  *  *  *  This program is free software; you can redistribute it and/or modify
  *  *  *  it under the terms of the GNU General Public License as published by
  *  *  *  the Free Software Foundation; either version 2 of the License, or
  *  *  *  (at your option) any later version.
  *  *  *
  *  *  *  This program is distributed in the hope that it will be useful,
  *  *  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  *  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  *  *  GNU General Public License for more details.
  *  *  *
  *  *  *  You should have received a copy of the GNU General Public License
  *  *  *  along with this program (it should be in the LICENSE file included in the
  *  *  *  directory where this file was found); if not, write to the Free Software
  *  *  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *  *  *
  *  *  *  Author contacts: studenti-nerds@lists.sme-ccppd.org
  *                     
 */
#include <math.h>
#include <m_pd.h>

static t_class * window_tilde_class;
struct _t_window_type;

typedef void (*t_wcf)(struct _t_window_type *);

typedef struct _t_window_type
{
	size_t size;		/*window size*/
	t_wcf create;	/*window creator function pointer*/	
	t_sample *w;	/*window table pointer*/
}t_window_type;

typedef struct _t_window_tilde
{
	t_object x_obj;
	t_window_type win;
	t_float f;
}t_window_tilde;

static void create_hanning(t_window_type *w)
{
	t_int i = 0;

	t_sample *p = w->w;
	
	for(i=0; i < w->size; ++i, ++p)
		*p = 0.5 * (1-cos(2*M_PI*(i/w->size)));
}

static void create_window(t_window_type *w, t_wcf fun)
{
	size_t oldsize = w->size;
			
	w->size = (size_t) sys_getblksize();
		
	w->create = fun;
		
	w->w = (t_sample *) resizebytes(w->w, oldsize, w->size);

	(*(w->create))(w);
}


t_int *window_tilde_perform(t_int *w)
{
	        
	t_window_tilde *x = (t_window_tilde *) (w[1]);
	t_sample *in = (t_sample *) (w[2]);
	t_sample *out = (t_sample *) (w[3]);
	int n = (int) (w[4]);
	t_sample *win = x->win.w;

	if (n != x->win.size)			
		create_window(&x->win, x->win.create);

	while (n--)
		*out++ = (*in++)*(*win++); 

	return (w+5);
}

static void window_tilde_select(t_window_tilde *x)
{

}	

void window_tilde_dsp(t_window_tilde *x, t_signal **sp)
{
	        dsp_add(window_tilde_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

static void init_window(t_window_type *w)
{
	w->size = 0;

	w->create = (t_wcf) NULL;

	w->w = (t_sample *) NULL;
}

static void *window_tilde_new (t_symbol *s)
{
	t_window_tilde *x = (t_window_tilde *)pd_new(window_tilde_class);
	
	outlet_new(&x->x_obj, &s_signal);

	init_window (&x->win); 
		
	create_window(&x->win, create_hanning); 
	
	return(void *)x;
}

static void window_tilde_destroy(t_window_tilde * x)
{
	freebytes(x->win.w, x->win.size);

	init_window(&x->win);
}

void window_tilde_setup (void)
{
	window_tilde_class = class_new(gensym("window~"), (t_newmethod)window_tilde_new,
				(t_method) window_tilde_destroy,sizeof(t_window_tilde), CLASS_DEFAULT, A_DEFFLOAT, 0);

	class_addmethod(window_tilde_class, (t_method)window_tilde_dsp, gensym("dsp"), A_CANT,0);
	class_addmethod(window_tilde_class, (t_method)window_tilde_select, gensym("select"), 0);
	CLASS_MAINSIGNALIN(window_tilde_class, t_window_tilde, f);
}
