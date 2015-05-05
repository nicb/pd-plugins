/*
 * $Id: ltas~.c 52 2006-06-01 21:30:43Z nicb $
 *
 *  ltas~: a long time-average spectrum  measurer
 *  Copyright (C) 2006  Nicola Bernardini, Roberto Bresin, Giampiero Salvi
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

#include <string.h>
#include <math.h>
#include <m_pd.h>

static t_class *ltas_tilde_class;

#define	DEFAULT_AVG_TIME	(2000)	/* 2000 msecs */

typedef struct _ltas_tilde
{
	t_object x_obj;
	t_sample f;
	size_t	 block_size;
	t_float	 avg_time;
	t_int    num_blocks;
	t_int	 cur_block;
	t_int	 oldest_block;
	t_sample **ffts;
	t_sample *lastsum;
	t_outlet *out[2];

}t_ltas_tilde;

#define BUFFER_MASK(x,o)	(x % (o->num_blocks))

static void
allocate_buffers(t_ltas_tilde *x)
{	
	t_sample **cur = (t_sample **) NULL;
	t_int n = x->num_blocks;
	size_t bufsize = 0;

	bufsize = x->block_size*sizeof(t_sample);
	x->ffts = (t_sample **) getbytes(x->num_blocks*sizeof(t_sample *));

	cur = x->ffts;
	while(n--)
	{
		*cur = (t_sample *) getbytes(bufsize);
		memset(*cur, 0, bufsize);
		++cur;
	}

	x->lastsum = (t_sample *) getbytes(bufsize);
	memset(x->lastsum, 0, bufsize);

	x->cur_block = 0;
	x->oldest_block = 1;
}

static void
free_buffers(t_ltas_tilde *x)
{	
	t_sample **cur = (t_sample **) NULL;
	t_int n = x->num_blocks;
	size_t bufsize = x->block_size*sizeof(t_sample);

	cur = x->ffts;
	while(n--)
		freebytes(*cur++, bufsize);

	freebytes(x->ffts, x->num_blocks*sizeof(t_sample *));
	freebytes(x->lastsum, bufsize);

	x->ffts = (t_sample **) NULL;
	x->lastsum = (t_sample *) NULL;
}

static void
ltas_tilde_bang(t_ltas_tilde *x)
{
	outlet_float(x->out[1], x->avg_time);
}

static t_float
block_msecs(t_int block_size, t_float sr)
{
	return (((t_float) block_size*1000)/sr);
}

static void
_ltas_change_everything(t_ltas_tilde *x, t_floatarg msecs, t_int blocksize)
{
	t_float sr = (t_float)sys_getsr();

	/* we want *at least* 1 block! */
	x->num_blocks = ((t_int)(msecs / block_msecs(blocksize, sr)))+1;
	x->block_size = blocksize;
	x->avg_time = (block_msecs(x->block_size, sr))*((t_float) x->num_blocks);

	allocate_buffers(x);
}

static void
ltas_change_everything(t_ltas_tilde *x, t_floatarg msecs, t_int blocksize)
{
	/*
	 * If we are changing something, there is no other way to
	 * actually 'resize' whatever was allocated before, but to
	 * rebuild it from scratch
	 *
	 * However, we can skip the 'freeing' step the first time
	 */
	free_buffers(x);

	_ltas_change_everything(x, msecs, blocksize);

	ltas_tilde_bang(x);
}

static void
ltas_change_avg_time(t_ltas_tilde *x, t_floatarg msecs)
{
	ltas_change_everything(x, msecs, x->block_size);
}

static void
ltas_change_blocksize(t_ltas_tilde *x, t_int new_blocksize)
{
	ltas_change_everything(x, x->avg_time, new_blocksize);
}

static void
initialize_ltas_system(t_ltas_tilde *x)
{
	x->ffts = (t_sample **) NULL;
	x->lastsum = (t_sample *) NULL;
	x->block_size = sys_getblksize();
	x->num_blocks = x->cur_block = x->oldest_block = 0;

	_ltas_change_everything(x, DEFAULT_AVG_TIME, x->block_size);
}

static void *
ltas_tilde_new(t_floatarg f)
{
	t_ltas_tilde *x = (t_ltas_tilde *)pd_new(ltas_tilde_class);

	initialize_ltas_system(x);

	inlet_new (&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
	inlet_new (&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("avgtime"));

	x->out[0] = outlet_new(&x->x_obj, &s_signal);
	x->out[1] = outlet_new(&x->x_obj, &s_float);

	return x;
}

static void
ltas_tilde_del(t_ltas_tilde *x)
{
	free_buffers(x);
}

static t_int *
ltas_tilde_perform(t_int *w)
{
	t_ltas_tilde *x = (t_ltas_tilde *) (w[1]);
	t_sample *in1 = (t_sample *) (w[2]);
	t_sample *in2 = (t_sample *) (w[3]);
	t_sample *out = (t_sample *) (w[4]);
	t_int cur_blocksize = (t_int) (w[5]);

	if(x->block_size != cur_blocksize)
		ltas_change_blocksize(x, cur_blocksize);

	t_sample *cur = *(x->ffts+x->cur_block);
	t_sample *first = *(x->ffts+x->oldest_block);
	t_sample *sum = x->lastsum;
	while (cur_blocksize--)
	{
		*cur = sqrt(((*in1)*(*in1))+((*in2)*(*in2)));
		*out = (*sum += (*cur - *first))/x->num_blocks;

		++in1; ++in2; ++out; ++cur; ++first; ++sum;
	}

	x->cur_block = BUFFER_MASK(++x->cur_block, x);
	x->oldest_block = BUFFER_MASK(++x->oldest_block, x);
	
	return (w+6);
}

void ltas_tilde_dsp(t_ltas_tilde *x, t_signal **sp)
{
	dsp_add(ltas_tilde_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

void ltas_tilde_setup (void)
{
	ltas_tilde_class = class_new(gensym("ltas~"),
		(t_newmethod) ltas_tilde_new, (t_method) ltas_tilde_del, 
		 sizeof(t_ltas_tilde), CLASS_DEFAULT, A_DEFFLOAT, 0);

	class_addmethod(ltas_tilde_class, (t_method)ltas_tilde_dsp, gensym("dsp"), 0);
	class_addmethod(ltas_tilde_class, (t_method)ltas_change_avg_time,
					gensym("avgtime"), A_DEFFLOAT, 0);
	class_addbang(ltas_tilde_class, (t_method)ltas_tilde_bang);
	CLASS_MAINSIGNALIN(ltas_tilde_class, t_ltas_tilde, f);
}
