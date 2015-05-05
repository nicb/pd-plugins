/*
 * $Id$
 *
 *  usbport: a native USB port driver
 *  system dependent files
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

#include <usb_sys.h>

void pd_hid_error(int errcode)
{
}

static int __pd_hid_inited__ = 0;

/*
 * product_string is a vendor:product string that gets passed to the PD module
 * as argument (or also as a message); it gets passed to this function without
 * further processing
 */
pd_hid_handle *
pd_hid_open(const char *product_string)
{
	pd_usb
	return (pd_hid_handle *) NULL;
}

void
pd_hid_close(pd_hid_handle *puh)
{
}
