/* $Id: centroid~.c 46 2006-04-06 09:50:31Z nicb $
 *
 *  *  centroid~: a centroid calculator object for Pure Data (PD)
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
 * 	The centroid is calculated as follows:
 *	we expect to receive the output from the rfft~ object.
 * 	
 * 			\sum{i=0}{n/2}{i*(X(i))}
 * 	centroid =	-----------------------
 *			\sum{i=0}{n/2}(X(i))
 *
 * 	
 *  	X is the absolute value of the fft 
 *  	i is the bin number
 *  	n is the buffer size
 *  
 */
#include <math.h>
#include <m_pd.h>

static t_class *centroid_tilde_class;

typedef struct _centroid_tilde
{
	t_object x_obj;
	t_float centroid;
	t_float f;
}t_centroid_tilde;

void *centroid_tilde_new()
{
	t_centroid_tilde *x = (t_centroid_tilde *)pd_new(centroid_tilde_class);

	x->centroid = 0;

	inlet_new (&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);

	outlet_new(&x->x_obj, &s_float);

	return (void *)x;
}
/*
void centroid_tilde_destroy(t_centroid_tilde *x)
{
	freebytes(x->modulo,)		
}
*/
static t_sample centroid_numerator (t_sample *m, int size)
{
	t_sample result = 0;
	int i=0;
	while(size--)
	{
		result += i*(*m);
		++i; ++m;
	}
	return result;
}

static t_sample centroid_denominator (t_sample *b, int size)
{
	t_sample result = 0;
	const t_sample abs_min = 0.000000001;

	while(size--)
		result += *b++;

	if (result < abs_min)
		result = abs_min;

	return result;
}

static t_sample * centroid_modulo(t_sample *r, t_sample *i, t_sample *m, int size)
{
	int n =0;
	t_sample *result = m;
	while (n<size)
	{
		*m = (t_sample) sqrt( (double)(((*r)*(*r)) + ((*i)*(*i))) );
		++r; ++i; ++m; ++n; 
	}
 	return result;
}
/*
 * Algorithm: 
 * 1) Calculate the modulo of the complex fft signal;
 * 2) Calculate the fft bin size;
 * 3) Calculate the sum of all bin sizes multiplied by modulo [numerator];
 * 4) Calculate the sum af all moduli [denominator];
 * 5) Divide the two sums -> centroid value;
 */
t_int *centroid_tilde_perform(t_int *w) 
{
	t_centroid_tilde *x = (t_centroid_tilde *) (w[1]);
	t_sample *real = (t_sample *) (w[2]);
	t_sample *imag = (t_sample *) (w[3]);
	int size = (int) (w[4]);
	t_float sr = sys_getsr();
	t_float binsize = sr/size;
	size = size>>1;	/*we need only half a buffer*/
		
	t_sample * modulo = (t_sample *) getbytes(sizeof(t_sample)*(size)); 
	centroid_modulo(real, imag, modulo, size);
	x->centroid = (t_float) centroid_numerator(modulo, size)/centroid_denominator(modulo, size); 
	x->centroid = binsize*((x->centroid)-0.5);
	freebytes (modulo,size);
	return (w+5);
}

void centroid_tilde_dsp(t_centroid_tilde *x, t_signal **sp)
{
	dsp_add(centroid_tilde_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

void centroid_tilde_bang(t_centroid_tilde *x)
{
	outlet_float(x->x_obj.ob_outlet, x->centroid);
}

void centroid_tilde_setup (void)
{
	centroid_tilde_class = class_new(gensym("centroid~"), (t_newmethod)centroid_tilde_new, 0, 
			  sizeof(t_centroid_tilde), CLASS_DEFAULT, A_DEFFLOAT, 0);

	class_addmethod(centroid_tilde_class, (t_method)centroid_tilde_dsp, gensym("dsp"), A_CANT,0);
	class_addmethod(centroid_tilde_class, (t_method)centroid_tilde_bang, gensym("bang"), 0);
	CLASS_MAINSIGNALIN(centroid_tilde_class, t_centroid_tilde, f);
}
