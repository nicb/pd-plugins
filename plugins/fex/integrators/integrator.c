/*
 * $Id: integrator.c 55 2006-12-18 22:04:32Z nicb $
 *
 *  integrator: an control-rate trapezoidal integrator
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

static t_class *integrator_class;

typedef struct _integrator
{
	t_object x_obj;
	t_outlet *outs[2];
	double start;
	double time_elapsed;
	t_float accu_value;
	t_float last_value;
}t_integrator;

#define Y_OUTPUT(x)	(x->outs[0])
#define X_OUTPUT(x)	(x->outs[1])

static void
integrator_output(t_integrator *x)
{
	outlet_float(Y_OUTPUT(x), (t_float) x->accu_value);
	outlet_float(X_OUTPUT(x), (t_float) x->time_elapsed);
}

static void
integrator_perform(t_integrator *x, t_float f)
{
	x->time_elapsed = (clock_getlogicaltime() - x->start)/1000000; /* we're talking seconds here */
	x->accu_value = (t_float)(((f + x->last_value)*x->time_elapsed)/2);
}

static void
integrator_step(t_integrator *x)
{
	x->last_value = x->accu_value;
	x->start = clock_getlogicaltime();
}

static void
integrator_float_right(t_integrator *x, t_float f)
{
	integrator_perform(x, f);
}

static void
integrator_float_left(t_integrator *x, t_float f)
{
	integrator_float_right(x, f);
	integrator_output(x);
	integrator_step(x);
}

static void
integrator_bang(t_integrator *x)
{
	integrator_output(x);
}

static void
integrator_reset(t_integrator *x)
{
	x->accu_value = 0;
	integrator_step(x);	/* resets everything else to zero */
}

static void *
integrator_new(t_floatarg f)
{
	t_integrator *x = (t_integrator *)pd_new(integrator_class);

	x->outs[0] = outlet_new(&x->x_obj, &s_float); /* integration result output */
	x->outs[1] = outlet_new(&x->x_obj, &s_float); /* number of points integrated */

	inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("right_float"), gensym("right_inlet"));

	integrator_reset(x);

	return x;
}

static void
integrator_del(t_integrator *x)
{
	/* empty? empty. */
}

void integrator_setup (void)
{
	integrator_class = class_new(gensym("integrator"),
		(t_newmethod) integrator_new, (t_method) integrator_del, 
		 sizeof(t_integrator), CLASS_DEFAULT, A_DEFFLOAT, 0);

	class_addfloat(integrator_class, integrator_float_left);
	class_addbang(integrator_class, (t_method)integrator_bang);
	class_addmethod(integrator_class, (t_method)integrator_reset, gensym("reset"), 0);
	class_addmethod(integrator_class, (t_method)integrator_float_right, gensym("right_inlet"), 0);
}
