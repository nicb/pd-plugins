/*
 * $Id$
 *
 *  usbport: a native USB port driver
 *  system dependent include files
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

#if !defined(_usb_sys_h_)
#	define _usb_sys_h_

#include <hid.h>

typedef struct _pd_usb_handle_
{
} pd_usb_handle;

void pd_usb_error(int errcode);

pd_usb_handle *pd_usb_open();
void pd_usb_close(pd_usb_handle *puh);

#endif /* !defined(_usb_sys_h_) */
