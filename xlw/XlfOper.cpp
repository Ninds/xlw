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
\file XlfOper.cpp
\brief Implements the XlfOper class.
*/

// $Id$

#include <xlw/XlfOper.h>
#include <xlw/ERR_Macros.h>
#include <xlw/XlfException.h>
#include <xlw/XlfRef.h>

// Stop header precompilation
#ifdef _MSC_VER
#pragma hdrstop
#endif

#ifndef NDEBUG
#include <xlw/XlfOper.inl>
#endif

#if !defined(PORT_USE_OLD_IO_HEADERS)
PORT_USING_NAMESPACE(std);
#endif

/*!
This bit is currently unused by Microsoft Excel. We set it
to indicate that the LPXLOPER (passed by Excel) holds some extra
memory to be freed.

This bit is controled in ~XlfOper to know if the DLL should release
auxiliary memory or not by a call to FreeAuxiliaryMemory.
*/
int xlbitCallFreeAuxMem = 0x8000;

/*!
Shallow copy of a pointer to XLOPER.
\param lpxloper Pointer to XLOPER.
\param isExcelData Flags that tells if the auxiliary data is allocated by
Excel or by the XLL (default is true).
*/
XlfOper::XlfOper(LPXLOPER lpxloper, bool isExcelData): lpxloper_(lpxloper)
{
  if (isExcelData && lpxloper->xltype == xltypeStr)
    lpxloper_->xltype&=xlbitCallFreeAuxMem;
  return;
}

/*!
Calls Deallocate() to free the XLOPER allocated by the XLL. XLOPER allocated
by Excel remain under Excel responsability.

Calls FreeAuxiliaryMemory if the XLOPER is owned by Excel and maintains heap
allocated data. If it is not owned by Excel, the data is assumed to be deleted
elsewhere in the XLL.
*/
XlfOper::~XlfOper()
{
  if (lpxloper_->xltype & xlbitCallFreeAuxMem)
  {
    FreeAuxiliaryMemory();
    // and switch back the bit as it was originally
    lpxloper_->xltype&=xlbitCallFreeAuxMem;
  }
  Deallocate();
  return;
}

/*!
Allocates 16 bits (size of a XLOPER) on the temporary buffer
stored by XlfExcel with a call to XlfExcel::GetMemory().

\bug Each XlfOper allocation causes a call to Allocate which in turn
reserve the necessary number of bytes in the internal buffer. The
problem is that even temporary XlfOper used inside the xll function use
this internal buffer. This buffer is not freed before the next call to
the xll to ensure Excel can use the data before they are freed. This
causes a bottleneck if the function uses many temporary XlfOper (see
Deallocate()).
*/
void XlfOper::Allocate()
{
  lpxloper_ = (LPXLOPER)XlfExcel::Instance().GetMemory(sizeof(XLOPER));
  if (!lpxloper_)
    return;
  lpxloper_->xltype = xltypeNil;
  return;
}

void XlfOper::FreeAuxiliaryMemory() const
{
  int err = XlfExcel::Instance().XlfExcel::Instance().Call(xlFree, NULL, 1, (LPXLOPER)lpxloper_);
  ERR_CHECKW(err == xlretSuccess,"Call to xlFree failed");
  return;
}

/*!
\param type is an integer indicating the target type we want to coerce to.
\param result is the XLOPER where to store the output.

\todo Optimize xlType to avoid consuming the buffer.
*/
int XlfOper::Coerce(short type, XlfOper& result) const
{
  XlfOper xlType(type);
  int xlret = XlfExcel::Instance().Call(xlCoerce, (LPXLOPER)result, 2, (LPXLOPER)lpxloper_, (LPXLOPER)xlType);
  return xlret;
}

double XlfOper::AsDouble() const
{
	double d;
	int xlret = ConvertToDouble(d);
	XlfExcel::Instance().ThrowOnCriticalError(xlret);
	return d;
};

int XlfOper::ConvertToDouble(double& d) const throw()
{
  int xlret;
  if (lpxloper_->xltype == xltypeInt)
  {
	  d = lpxloper_->val.w;
      xlret=xlretSuccess;
  }
  else if (lpxloper_->xltype == xltypeNum)
  {
	  d = lpxloper_->val.num;
      xlret=xlretSuccess;
  }
  else
  {
    // Allocates tmp on the stack to avoid filling the internal buffer.
    XLOPER tmp;
    // Creates a XlfOper based on tmp.
    XlfOper cast(&tmp,false);
    // Coerces to numeric type.
    xlret = Coerce(xltypeNum,cast);
	if (xlret == xlretSuccess)
	    xlret = cast.ConvertToDouble(d);
  }
  return xlret;
};

short XlfOper::AsShort() const
{
	short s;
	int xlret = ConvertToShort(s);
	XlfExcel::Instance().ThrowOnCriticalError(xlret);
	return s;
};

int XlfOper::ConvertToShort(short& s) const throw()
{
  int xlret;
  if (lpxloper_->xltype == xltypeNum)
  {
	  s = lpxloper_->val.num;
      xlret=xlretSuccess;
  }
  else
  {
    // Allocates tmp on the stack to avoid filling the internal buffer.
    XLOPER tmp;
    // Creates a XlfOper based on tmp.
    XlfOper cast(&tmp,false);
    // Coerces to numeric type.
    xlret = Coerce(xltypeNum,cast);
	if (xlret == xlretSuccess)
	    xlret = cast.ConvertToShort(s);
  }
  return xlret;
};


bool XlfOper::AsBool() const
{
	bool b;
	int xlret = ConvertToBool(b);
	XlfExcel::Instance().ThrowOnCriticalError(xlret);
	return b;
};

int XlfOper::ConvertToBool(bool& b) const throw()
{
  int xlret;
  if (lpxloper_->xltype == xltypeBool)
  {
	  b = (lpxloper_->val.boolean != 0);
	  xlret = xlretSuccess;
  }
  else
  {
    // see ConvertToDouble
    XLOPER tmp;
    XlfOper cast(&tmp,false);
    xlret = Coerce(xltypeBool,cast);
	if (xlret == xlretSuccess)
		xlret = cast.ConvertToBool(b);
  }
  return xlret;
};

/*!
For character strings, the XLL allocates
the memory on its own buffer. This buffer is automatically freed
when a function of the XLL is called again. Note that coerce to
a char string is the slowest cast of all.
*/
char * XlfOper::AsString() const
{
	char * s;
	int xlret = ConvertToString(s);
	XlfExcel::Instance().ThrowOnCriticalError(xlret);
	return s;
};

int XlfOper::ConvertToString(char *& s) const throw()
{
  int xlret;
  if (lpxloper_->xltype == xltypeStr)
  {
    size_t n = lpxloper_->val.str[0];
    s = XlfExcel::Instance().GetMemory(n + 1);
    memcpy(s, lpxloper_->val.str + 1, n);
    s[n] = 0;
	xlret = xlretSuccess;
  }
  else
  {
    // see AsDouble
    XLOPER tmp;
    XlfOper cast(&tmp,false);
    xlret = Coerce(xltypeStr,cast);
	if (xlret == xlretSuccess)
	    xlret = cast.ConvertToString(s);
	// The memory for the string is allocated on Excel area
	// FreeAuxiliaryMemory lets Excel know that we no longer need it.
	cast.FreeAuxiliaryMemory();
  }
  return xlret;
}

XlfRef XlfOper::AsRef() const
{
	XlfRef r;
	int xlret = ConvertToRef(r);
	XlfExcel::Instance().ThrowOnCriticalError(xlret);
	return r;
}

int XlfOper::ConvertToRef(XlfRef& r) const throw()
{
  int xlret;
  if (lpxloper_->xltype == xltypeSRef)
  {
    r = XlfRef (lpxloper_->val.sref.ref.rwFirst,  // top
                lpxloper_->val.sref.ref.colFirst, // left
                lpxloper_->val.sref.ref.rwLast,   // bottom
                lpxloper_->val.sref.ref.colLast); // right
	xlret = xlretSuccess;
  }
  else
  {
    // see AsDouble
    XLOPER tmp;
    XlfOper cast(&tmp,false);
    xlret = Coerce(xltypeStr,cast);
	if (xlret == xlretSuccess)
	    xlret = cast.ConvertToRef(r);
  }
  return xlret;
}

XlfOper& XlfOper::Set(LPXLOPER lpxloper)
{
  ERR_CHECKX2(lpxloper != 0, XlfException,"Assignment of NULL pointer");
  lpxloper_ = lpxloper;
  return *this;
}

XlfOper& XlfOper::Set(double value)
{
  lpxloper_->xltype = xltypeNum;
  lpxloper_->val.num = value;
  return *this;
}

XlfOper& XlfOper::Set(short value)
{
  lpxloper_->xltype = xltypeInt;
  lpxloper_->val.w = value;
  return *this;
}

XlfOper& XlfOper::Set(bool value)
{
  lpxloper_->xltype = xltypeBool;
  lpxloper_->val.boolean = value;
  return *this;
}

XlfOper& XlfOper::Set(const XlfRef& range)
{
  lpxloper_->xltype = xltypeSRef;
  lpxloper_->val.sref.ref.rwFirst = range.GetRowBegin();
  lpxloper_->val.sref.ref.rwLast = range.GetRowEnd()-1;
  lpxloper_->val.sref.ref.colFirst = range.GetColBegin();
  lpxloper_->val.sref.ref.colLast = range.GetColEnd()-1;
  return *this;
}

/*!
\bug String longer than 255 characters are truncated. A warning
is issued in debug mode.
*/
XlfOper& XlfOper::Set(const char *value)
{
  lpxloper_->xltype = xltypeStr;
  int n = strlen(value);
  ERR_CHECKW2(n<256,"String too long will be truncated");
  // One byte more for NULL terminal char (allow use of strcpy)
  // and one for the std::string size (convention used by Excel)
  lpxloper_->val.str = (LPSTR)XlfExcel::Instance().GetMemory(n + 2);
  strcpy(lpxloper_->val.str + 1, value);
  // the number of character include the final \0 or not ?
  lpxloper_->val.str[0] = (BYTE)(n + 1);
  return *this;
}

XlfOper& XlfOper::SetError(WORD error)
{
  lpxloper_->xltype = xltypeErr;
  lpxloper_->val.err = error;
  return *this;
}

