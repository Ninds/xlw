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

#ifndef INC_macros_H
#define INC_macros_H

/*!
\file macros.h
\brief I am not a big fan of macros, but these make life easier.
*/

// $Id$

#include <xlw/XlfException.h>
#include <xlw/XlfExcel.h>

#if defined(_MSC_VER)
#pragma once
#endif

//! Export flag.
/*!
Export macro that tells the compiler that the function is to be exported.
*/
#define EXCEL_EXPORT __declspec(dllexport)


//! Initialization macro
/*!
Macros to be inserted at the begining and end of *each* C function
to be exported to excel.

Frees the internal buffer if needed of XlfExcel used to return
heap allocated data to Excel.
\sa XlfExcel
*/
#define EXCEL_BEGIN XlfExcel::Instance().FreeMemory(); \
try \
{

//! Clean up macro
/*!
Macro to be inserted at the end of *each* C function to be exported
to excel.

Catch exception in C function calls
Catching rules are as follow :
- try first to catch excel exception that need the framework to return to excel
asap (namely coerce, uncalculated cell, and abort). If catched, returns #NULL!.
- catch all other kind of exception. Return #VALUE! error if possible otherwise
returns #NULL!.

You could easily add your own exception here. Note that it causes all your
interface to recompile though.
*/
#define EXCEL_END \
} catch (XlfException&) { \
	return 0; \
} catch (...) { \
	return XlfException::Value(); \
}

/* as example:
#define EXCEL_END \
} catch (XlfException&) { \
	return 0; \
} catch (yourException&) { \
	exceptionHandling; \
} catch (...) { \
	return XlfException::Value(); \
}

*/

#endif