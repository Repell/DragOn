// 명령어		복사할 파일이 있는 위치				복사된 파일을 저장할 위치

//System Codes
copy		.\Engine\Engine_System\Codes\*.h			.\Reference\Headers\		
copy		.\Engine\Engine_System\Bin\Engine_System.dll		.\Client\Bin\
copy		.\Engine\Engine_System\Bin\Engine_System.lib		.\Reference\Library\	
copy		.\Engine\Engine_System\Bin\Engine_System.dll		.\TotalTool\Bin\

//Utility Codes
copy		.\Engine\Engine_Utility\Codes\*.h			.\Reference\Headers\
copy		.\Engine\Engine_Utility\Bin\Engine_Utility.dll	.\Client\Bin\		
copy		.\Engine\Engine_Utility\Bin\Engine_Utility.lib	.\Reference\Library\	
copy		.\Engine\Engine_Utility\Bin\Engine_Utility.dll	.\TotalTool\Bin\		

//Resource Codes
//copy		.\Engine\Engine_Resource\Codes\*.h				.\Reference\Headers\		
//copy		.\Engine\Engine_Resource\Bin\Engine_Resource.dll	.\Client\Bin\		
//copy		.\Engine\Engine_Resource\Bin\Engine_Resource.lib	.\Reference\Library\	
//copy		.\Engine\Engine_Resource\Bin\Engine_Resource.dll	.\TotalTool\Bin\		

//Headers
copy		.\Engine\Engine_Headers\*.h			.\Reference\Headers\
copy		.\Engine\Engine_Headers\*.inl			.\Reference\Headers\