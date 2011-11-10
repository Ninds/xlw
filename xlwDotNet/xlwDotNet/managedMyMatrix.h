/*
 Copyright (C) 2008 2009  Narinder S Claire
 Copyright (C) 2011 Mark P Owen

 This file is part of XLWDOTNET, a free-software/open-source C# wrapper of the
 Excel C API - http://xlw.sourceforge.net/

 XLWDOTNET is part of XLW, a free-software/open-source C++ wrapper of the
 Excel C API - http://xlw.sourceforge.net/

 XLW is free software: you can redistribute it and/or modify it under the
 terms of the XLW license.  You should have received a copy of the
 license along with this program; if not, please email xlw-users@lists.sf.net

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef XLW_DOT_NET_MYMATRIX_H
#define XLW_DOT_NET_MYMATRIX_H




using namespace System;
#include<xlw/MyContainers.h>
#include"xlwTypeBaseClass.h"


namespace xlwDotNet
{
    namespace xlwTypes
    {
        public ref class MyMatrix :public xlwTypebaseClass<xlw::MyMatrix>
        {

        public:
            MyMatrix(IntPtr theRealThing):
              xlwTypebaseClass<xlw::MyMatrix>(theRealThing,false){}

            MyMatrix(const MyMatrix^ theOther):
                xlwTypebaseClass<xlw::MyMatrix>(new xlw::MyMatrix(*(theOther->theInner)),true){}

            MyMatrix(int rows_, int cols_):
                xlwTypebaseClass<xlw::MyMatrix>(new xlw::MyMatrix(rows_,cols_),true){}


            property double default[int,int]
            {
                double get(int i,int j)
                {
                    return theInner->operator[](i)[j];
                }
                void set(int i,int j,double val)
                {
                    theInner->operator[](i)[j]=val;
                }
            }

           property int rows
            {
                int get()
                {
                    return (int)theInner->rows();
                }
            }
           property int columns
            {
                int get()
                {
                    return (int)theInner->columns();
                }
            }

           static operator array<double,2>^(MyMatrix^ myMatrix)
           {
               array<double,2>^ theCSMatrix =  gcnew array<double,2>(myMatrix->rows, myMatrix->columns);
               for(int i(0);i<myMatrix->rows;++i)
               {
                   for(int j(0);j<myMatrix->columns;++j)
                        theCSMatrix[i,j]=myMatrix->theInner->operator[](i)[j];
               }
               return theCSMatrix;
           }

           static operator MyMatrix^(array<double,2>^ theCSMatrix)
           {
               MyMatrix^ theXLWMatrix =  gcnew MyMatrix(theCSMatrix->GetLength(0),theCSMatrix->GetLength(1));
                for(int i(0);i<theCSMatrix->GetLength(0);++i)
               {
                   for(int j(0);j<theCSMatrix->GetLength(1);++j)
                       theXLWMatrix->theInner->operator[](i)[j]=theCSMatrix[i,j];
               }
                return theXLWMatrix;

           }
            static void *getInner (MyMatrix^ theArray){return theArray->theInner;}
        };

    }
}

#endif

