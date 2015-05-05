/*
 * $Id$
 *
 *  usbport: a native USB port driver
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

static t_class *usbport_class;

typedef struct _usbport
{
	t_object x_obj;
	t_outlet *outs[2];
	double start;
	double time_elapsed;
	t_float accu_value;
	t_float last_value;
}t_usbport;

#define Y_OUTPUT(x)	(x->outs[0])
#define X_OUTPUT(x)	(x->outs[1])

static void
usbport_output(t_usbport *x)
{
	outlet_float(Y_OUTPUT(x), (t_float) x->accu_value);
	outlet_float(X_OUTPUT(x), (t_float) x->time_elapsed);
}

static void
usbport_perform(t_usbport *x, t_float f)
{
	x->time_elapsed = (clock_getlogicaltime() - x->start)/1000000; /* we're talking seconds here */
	x->accu_value = (t_float)(((f + x->last_value)*x->time_elapsed)/2);
}

static void
usbport_step(t_usbport *x)
{
	x->last_value = x->accu_value;
	x->start = clock_getlogicaltime();
}

static void
usbport_float_right(t_usbport *x, t_float f)
{
	usbport_perform(x, f);
}

static void
usbport_float_left(t_usbport *x, t_float f)
{
	usbport_float_right(x, f);
	usbport_output(x);
	usbport_step(x);
}

static void
usbport_bang(t_usbport *x)
{
	usbport_output(x);
}

static void
usbport_reset(t_usbport *x)
{
	x->accu_value = 0;
	usbport_step(x);	/* resets everything else to zero */
}

static void *
usbport_new(t_floatarg f)
{
	t_usbport *x = (t_usbport *)pd_new(usbport_class);

	x->outs[0] = outlet_new(&x->x_obj, &s_float); /* integration result output */
	x->outs[1] = outlet_new(&x->x_obj, &s_float); /* number of points integrated */

	inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("right_float"), gensym("right_inlet"));

	usbport_reset(x);

	return x;
}

static void
usbport_del(t_usbport *x)
{
	/* empty? empty. */
}

void usbport_setup (void)
{
	usbport_class = class_new(gensym("usbport"),
		(t_newmethod) usbport_new, (t_method) usbport_del, 
		 sizeof(t_usbport), CLASS_DEFAULT, A_DEFFLOAT, 0);

	class_addfloat(usbport_class, usbport_float_left);
	class_addbang(usbport_class, (t_method)usbport_bang);
	class_addmethod(usbport_class, (t_method)usbport_reset, gensym("reset"), 0);
	class_addmethod(usbport_class, (t_method)usbport_float_right, gensym("right_inlet"), 0);
}
