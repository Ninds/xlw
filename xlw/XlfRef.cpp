/*
 Copyright (C) 1998, 1999, 2001, 2002 J�r�me Lecomte

 This file is part of XLW, a free-software/open-source C++ wrapper of the
 Excel C API - http://xlw.sourceforge.net/

 XLW is free software: you can redistribute it and/or modify it under the
 terms of the XLW license.  You should have received a copy of the
 license along with this program; if not, please email xlw-users@lists.sf.net

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*!
\file XlfRef.cpp
\brief Implements the XlfRef class.
*/

// $Id$

#include <xlw/XlfRef.h>
#include <xlw/XlfOper.h>
#include <xlw/ERR_Macros.h>
#include <xlw/XlfException.h>

// Stop header precompilation
#ifdef _MSC_VER
#pragma hdrstop
#endif

#if !defined(PORT_USE_OLD_IO_HEADERS)
PORT_USING_NAMESPACE(std);
#endif

#ifndef NDEBUG
#include <xlw/XlfRef.inl>
#endif

XlfOper XlfRef::operator()(WORD r, BYTE c) const
{
	ERR_CHECKX(rowbegin_ + r<rowend_ && colbegin_ + c<colend_, XlfException,"access out of range");
	XlfOper res;
	res.Set(XlfRef(rowbegin_ + r, colbegin_ + c, sheetId_));
	return res;
}
