    !include "MUI2.nsh"
	!include ".\version.nsh"
	!include "LogicLib.nsh"


;------------------------------------------------------------------------------------------------------------------------
;------------------------------------------------------------------------------------------------------------------------
;Definitions

   !define APP "xlwDotNetTemplateExtractor"
	Name "${APP}"
    OutFile "${APP}.exe"
	RequestExecutionLevel user
	BrandingText "${APP}"
	InstallDir "$DOCUMENTS\XLL_Project"


;------------------------------------------------------------------------------------------------------------------------
;------------------------------------------------------------------------------------------------------------------------
;Interface Settings

	!define MUI_PAGE_HEADER_TEXT "Select Installation Directory"
	!define MUI_PAGE_HEADER_SUBTEXT "Select the directory where the template project will be placed"
	!define MUI_DIRECTORYPAGE_TEXT_TOP "When the project is extracted, you should start by changing the name of the project folder and any relevant project files"
    !define MUI_ABORTWARNING
    !define MUI_ICON "xlw\docs\images\xlw_32x32.ico"
    !define MUI_UNICON "xlw\docs\images\xlw_32x32.ico"
	!define MUI_HEADERIMAGE

	!define MUI_HEADERIMAGE_BITMAP "xlw-site\images\logo.bmp"
	
	



;------------------------------------------------------------------------------------------------------------------------
;------------------------------------------------------------------------------------------------------------------------
;Pages

	Page custom ExtractorPage ;Custom page

    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_INSTFILES

	
;------------------------------------------------------------------------------------------------------------------------
;------------------------------------------------------------------------------------------------------------------------
;Languages

    !insertmacro MUI_LANGUAGE "English"
	
;------------------------------------------------------------------------------------------------------------------------
;------------------------------------------------------------------------------------------------------------------------

;------------------------------------------------------------------------------------------------------------------------
;------------------------------------------------------------------------------------------------------------------------

Var DIALOG

Var VS9
Var VS8
Var VS9_HYBRID
Var VS8_HYBRID

Var VS9_STATE
Var VS8_STATE
Var VS9_HYBRID_STATE
Var VS8_HYBRID_STATE

	
Function ExtractorPage

	!insertmacro MUI_HEADER_TEXT "XLW Template Extractor"  "Select the Dev environment for which the template project is to be extracted"

	GetDlgItem $0 $HWNDPARENT 1
	EnableWindow $0 0

	nsDialogs::Create 1018
	Pop $DIALOG

	GetFunctionAddress $0 RadioChanged
	
	${NSD_CreateRadioButton} 40 30 100% 10% "Visual Studio 2008 (VS9)"    
	Pop $VS9
	nsDialogs::OnClick  $VS9 $0
    ${NSD_CreateRadioButton} 40 60 100% 10% "Visual Studio 2005 (VS8)" 
	Pop $VS8
	nsDialogs::OnClick  $VS8 $0
	${NSD_CreateRadioButton} 40 90 100% 10% "Visual Studio 2008 (VS9) (Hybrid C++/C# XLL  Not Visual Studio Express)"    
	Pop $VS9_HYBRID
	nsDialogs::OnClick  $VS9_HYBRID $0
    ${NSD_CreateRadioButton} 40 120 100% 10% "Visual Studio 2005 (VS8) (Hybrid C++/C# XLL  Not Visual Studio Express)"   
	Pop $VS8_HYBRID
	nsDialogs::OnClick  $VS8_HYBRID $0

	
	nsDialogs::Show

FunctionEnd



Function RadioChanged
	Pop $0 # dir hwnd

	GetDlgItem $0 $HWNDPARENT 1
	EnableWindow $0 1

	
	${NSD_GetState} $VS9 $VS9_STATE
	${NSD_GetState} $VS8 $VS8_STATE
	${NSD_GetState} $VS9_HYBRID $VS9_HYBRID_STATE
	${NSD_GetState} $VS8_HYBRID $VS8_HYBRID_STATE
	
	
FunctionEnd

!macro projectfiles dir
		SetOutPath "$INSTDIR"
		File /nonfatal /r "xlwDotNet\Template_Projects\${dir}\*.vcproj"
		File /nonfatal /r "xlwDotNet\Template_Projects\${dir}\*.csproj"
		File /nonfatal /r "xlwDotNet\Template_Projects\${dir}\*.sln"
		CreateDirectory  "$INSTDIR\AutoGeneratedSource"
	!macroend
	
		
	!macro sourcefiles dir
		SetOutPath "$INSTDIR"
		File /nonfatal /r "xlwDotNet\Template_Projects\${dir}\*.cpp"
		File /nonfatal /r "xlwDotNet\Template_Projects\${dir}\*.h"
		File /nonfatal /r "xlwDotNet\Template_Projects\${dir}\*.cs"
		File /nonfatal /r "xlwDotNet\Template_Projects\${dir}\*.snk"
	!macroend



!macro GETDIR  STATEFLAG TDIR

	${If} ${STATEFLAG} ==  ${BST_CHECKED}
			!insertmacro projectfiles ${TDIR}
			!insertmacro sourcefiles  ${TDIR}
	${Endif}


!macroend

Section #
	!insertmacro GETDIR $VS9_STATE "VS9"
	!insertmacro GETDIR $VS8_STATE "VS8"
	!insertmacro GETDIR $VS9_HYBRID_STATE "Hybrid_Cpp_CSharp_XLLs\VS9_PRO"
	!insertmacro GETDIR $VS8_HYBRID_STATE "Hybrid_Cpp_CSharp_XLLs\VS8_PRO"
 
SectionEnd 










