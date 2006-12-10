//
//
//							ArgList.h
//
//
/*
 Copyright (C) 2006 Mark Joshi
 
 This file is part of XLW, a free-software/open-source C++ wrapper of the
 Excel C API - http://xlw.sourceforge.net/
 
 XLW is free software: you can redistribute it and/or modify it under the
 terms of the XLW license.  You should have received a copy of the
 license along with this program; if not, please email xlw-users@lists.sf.net
 
 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef ARG_LIST_H
#define ARG_LIST_H


#include "CellMatrix.h"
#include "MyContainers.h"
#include <map>
#include <string>
#include <vector>


void MakeLowerCase(std::string& input);

class ArgumentList
{
public:

    ArgumentList(CellMatrix cells, 
                      std::string ErrorIdentifier);


    enum ArgumentType
    {
        string, number, vector, matrix, 
		boolean, list, cells
    };

    std::string GetStructureName() const;

    const std::vector<std::pair<std::string, ArgumentType> >& GetArgumentNamesAndTypes() const; 
    
	std::string GetStringArgumentValue(const std::string& ArgumentName);
    unsigned long GetULArgumentValue(const std::string& ArgumentName); 
    double GetDoubleArgumentValue(const std::string& ArgumentName);
    MyArray GetArrayArgumentValue(const std::string& ArgumentName);
    MyMatrix GetMatrixArgumentValue(const std::string& ArgumentName);
	bool GetBoolArgumentValue(const std::string& ArgumentName);
	CellMatrix GetCellsArgumentValue(const std::string& ArgumentName);

#if !(defined(_MSC_VER)) ||  _MSC_VER > 1250 // get a proper compiler 
	ArgumentList GetArgumentListArgumentValue(const std::string& ArgumentName);
#endif


    bool IsArgumentPresent(const std::string& ArgumentName) const;

	void CheckAllUsed(const std::string& ErrorId) const;

	CellMatrix AllData() const; // makes data into a cell matrix that could be used for 
								// creating the same argumnet list
								// useful for checking the class works!
   
private:

    std::string StructureName;

    std::vector<std::pair<std::string, ArgumentType> > ArgumentNames;
    std::map<std::string,double> DoubleArguments;
    std::map<std::string,MyArray> ArrayArguments;
    std::map<std::string,MyMatrix> MatrixArguments;
    std::map<std::string,std::string> StringArguments;
	
#if !(defined(_MSC_VER)) ||  _MSC_VER > 1250 // get a proper compiler 
	std::map<std::string,ArgumentList> ListArguments;	
#endif


	std::map<std::string,CellMatrix> CellArguments;
	std::map<std::string,bool> BoolArguments;

    std::map<std::string,ArgumentType> Names;

    std::map<std::string,bool> ArgumentsUsed;

	void GenerateThrow(std::string message, unsigned long row, unsigned long column);
	void UseArgumentName(const std::string& ArgumentName); // private as no error checking performed

};



#endif
