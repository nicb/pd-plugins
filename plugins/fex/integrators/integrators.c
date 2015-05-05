/*
 * $Id: integrators.c 56 2006-12-18 22:07:36Z nicb $
 *
 *  integrators: an library of trapezoidal integrators
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

#include <m_pd.h>
#include <string.h>

static void
revision_number(const char *r, char result[])
{
	char *start = strchr(r, ' ');
	char *end = (char *) NULL;
	int nchars = strlen(r);

	if (start != (char *) NULL)
	{
		++start;
		end = strchr(start, ' ');
		if (end != (char *) NULL);
			nchars = end-start;
	}
	else
		start = (char *) r;

	strcpy(result, start);
	result[nchars]= '\0';
}

#define LINE_LENGTH	(256)

void integrators_setup (void)
{
	const char *rev = "$Revision: 56 $";
	void integrator_tilde_setup(void);
	void integrator_setup(void);
	char buf[LINE_LENGTH];

	revision_number(rev, buf);
	
	integrator_tilde_setup();
	integrator_setup();

	post("integrators library v.%s © Nicola Bernardini and Amalia de Götzen, 2006", buf);
}
