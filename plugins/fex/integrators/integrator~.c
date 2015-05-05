/*
 * $Id: integrator~.c 53 2006-12-18 12:27:14Z nicb $
 *
 *  integrator~: an audio-rate trapezoidal integrator
 *  Copyright (C) 2006  Nicola Bernardini, Amalia de Götzen
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
 *  
 *  
 */

#include <math.h>
#include <m_pd.h>

static t_class *integrator_tilde_class;

typedef struct _integrator_tilde
{
	t_object x_obj;
	t_sample f;
	t_sample accu_value;
	t_int	num_points;
	t_sample last_value;
}t_integrator_tilde;

static void
integrator_tilde_reset(t_integrator_tilde *x)
{
	x->accu_value = x->num_points = x->last_value = 0;
}

static void *
integrator_tilde_new(t_floatarg f)
{
	t_integrator_tilde *x = (t_integrator_tilde *)pd_new(integrator_tilde_class);

	outlet_new(&x->x_obj, &s_signal); /* integration result output */
	outlet_new(&x->x_obj, &s_signal); /* number of points integrated */

	return x;
}

static void
integrator_tilde_del(t_integrator_tilde *x)
{
	/* empty? empty. */
}

static t_int *
integrator_tilde_perform(t_int *w)
{
	t_integrator_tilde *x = (t_integrator_tilde *) (w[1]);
	t_sample *in1 = (t_sample *) (w[2]);
	t_sample *out1 = (t_sample *) (w[3]);
	t_sample *out2 = (t_sample *) (w[4]);
	t_int cur_blocksize = (t_int) (w[5]);
	t_float speriod = 1/sys_getsr(); 					/* period in seconds */

	while (cur_blocksize--)
	{
		t_sample tpz_area = (((*in1)+x->last_value)*speriod)/2;
		x->accu_value += (t_float) tpz_area;
		x->last_value = *in1;
		*out1 = x->accu_value;							/* integrated value */
		*out2 = (x->num_points)*speriod;				/* time past (in seconds) */
		++in1; ++out1; ++out2; ++(x->num_points);
	}

	return (w+6);
}

void integrator_tilde_dsp(t_integrator_tilde *x, t_signal **sp)
{
	dsp_add(integrator_tilde_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

void integrator_tilde_setup (void)
{
	integrator_tilde_class = class_new(gensym("integrator~"),
		(t_newmethod) integrator_tilde_new, (t_method) integrator_tilde_del, 
		 sizeof(t_integrator_tilde), CLASS_DEFAULT, A_DEFFLOAT, 0);

	class_addmethod(integrator_tilde_class, (t_method)integrator_tilde_dsp, gensym("dsp"), 0);
	class_addmethod(integrator_tilde_class, (t_method)integrator_tilde_reset, gensym("reset"), 0);
	CLASS_MAINSIGNALIN(integrator_tilde_class, t_integrator_tilde, f);
}
