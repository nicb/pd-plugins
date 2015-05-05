/* $Id: mul~.c 26 2006-02-04 15:44:44Z nicb $
 *
 *  *  mul~: a ring modulator object for Pure Data (PD)
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

static t_class *mul_tilde_class;

typedef struct _mul_tilde
{
	t_object x_obj;
	t_sample f_mul;
	t_sample f;
}t_mul_tilde;

void *mul_tilde_new(t_floatarg f)
{
	t_mul_tilde *x = (t_mul_tilde *)pd_new(mul_tilde_class);

	x->f_mul = f;

	inlet_new (&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);

	outlet_new(&x->x_obj, &s_signal);

	return (void *)x;
}

t_int *mul_tilde_perform(t_int *w)
{
	t_mul_tilde *x = (t_mul_tilde *) (w[1]);
	t_sample *in1 = (t_sample *) (w[2]);
	t_sample *in2 = (t_sample *) (w[3]);
	t_sample *out = (t_sample *) (w[4]);
	int n = (int) (w[5]);

	while (n--) 
		*out++ = (*in1++)*(*in2++); /*  in1*in2 */
	
	return (w+6);
}

void mul_tilde_dsp(t_mul_tilde *x, t_signal **sp)
{
	dsp_add(mul_tilde_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

void mul_tilde_setup (void)
{
	mul_tilde_class = class_new(gensym("mul~"), (t_newmethod)mul_tilde_new, 0, 
			  sizeof(t_mul_tilde), CLASS_DEFAULT, A_DEFFLOAT, 0);

	class_addmethod(mul_tilde_class, (t_method)mul_tilde_dsp, gensym("dsp"), 0);
	CLASS_MAINSIGNALIN(mul_tilde_class, t_mul_tilde, f);
}
