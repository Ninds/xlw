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
\file XlfOper.inl
\brief Implements inline methods of XlfOper.
*/

// $Id$

#ifdef NDEBUG
#define INLINE inline
#else
#define INLINE
#endif

/*!
Calls Allocate
*/
INLINE XlfOper::XlfOper()
{
  Allocate();
}

INLINE XlfOper::XlfOper(const XlfOper& oper)
{
  *this = oper;
};

INLINE XlfOper::XlfOper(double value)
{
  Allocate();
  Set(value);
}

INLINE XlfOper::XlfOper(short value)
{
  Allocate();
  Set(value);
}

INLINE XlfOper::XlfOper(bool value)
{
  Allocate();
  Set(value);
}

INLINE XlfOper::XlfOper(const char *value)
{
  Allocate();
  Set(value);
}

INLINE XlfOper::XlfOper(const XlfRef& range)
{
  Allocate();
  Set(range);
}

INLINE XlfOper& XlfOper::operator=(const XlfOper& rhs)
{
  if (this != &rhs)
    lpxloper_ = rhs.lpxloper_;
  return *this;
}

INLINE XlfOper::operator LPXLOPER()
{
  return lpxloper_;
}

INLINE bool XlfOper::IsMissing() const
{
  return lpxloper_->xltype == xltypeMissing;
}

INLINE bool XlfOper::IsError() const
{
  return lpxloper_->xltype == xltypeErr;
}

INLINE int XlfOper::AsInt() const
{
  return static_cast<int>(AsDouble());
}

INLINE LPXLOPER XlfOper::GetLPXLOPER() const
{
  return lpxloper_;
}

/*!
Do nothing. May be implemented later to avoid overload of the internal
buffer by temporary XLOPER allocated by the XLL.
*/
INLINE void XlfOper::Deallocate()
{}
