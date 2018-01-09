

xcopy /y	.\Engine\Headers\*.h					.\Reference\Headers\
xcopy /y	.\Engine\Export\*.*						.\Reference\Headers\

xcopy /y	.\Engine\System\Code\*.h				.\Reference\Headers\
xcopy /y	.\Engine\System\bin\System.lib			.\Reference\Librarys\
xcopy /y	.\Engine\System\bin\System.dll			.\Client\bin\
xcopy /y	.\Engine\System\bin\System.dll			.\MapObjTool\bin\

xcopy /y	.\Engine\Utility\Code\*.h				.\Reference\Headers\
xcopy /y	.\Engine\Utility\Code\*.hpp				.\Reference\Headers\
xcopy /y	.\Engine\Utility\bin\Utility.lib		.\Reference\Librarys\
xcopy /y	.\Engine\Utility\bin\Utility.dll		.\Client\bin\
xcopy /y	.\Engine\Utility\bin\Utility.dll		.\MapObjTool\bin\

xcopy /y	.\Engine\Resources\Code\*.h				.\Reference\Headers\
xcopy /y	.\Engine\Resources\bin\Resources.lib	.\Reference\Librarys\
xcopy /y	.\Engine\Resources\bin\Resources.dll	.\Client\bin\
xcopy /y	.\Engine\Resources\bin\Resources.dll	.\MapObjTool\bin\


xcopy /y	.\SuperClass\*.*						.\Reference\Headers\