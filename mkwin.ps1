

# This is a Windows PowerShell script


rm qrc_icons.cpp -erroraction 'silentlycontinue'
rm geCache.pro -erroraction 'silentlycontinue'
rm Makefile -erroraction 'silentlycontinue'


# Building the .pro file using qmake

qmake -project -o geCache.tmp

Add-Content geCache.tmp2 "`nRC_FILE = geCache.rc"
Add-Content geCache.tmp2 "`nRESOURCES = icons.qrc"
Add-Content geCache.tmp2 "`nQT += widgets"
Add-Content geCache.tmp2 "`nLIBPATH += 'C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64'"
Add-Content geCache.tmp2 "`nLIBPATH += 'C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\amd64'"
Add-Content geCache.tmp2 "`nLIBPATH += 'C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10240.0\ucrt\x64'"
Add-Content geCache.tmp2 "`nINCLUDEPATH += 'C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include'"
Add-Content geCache.tmp2 "`nINCLUDEPATH += 'C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt'"
Add-Content geCache.tmp2 "`nINCLUDEPATH += 'C:\Program Files (x86)\Windows Kits\8.1\Include\um'"
Add-Content geCache.tmp2 "`nINCLUDEPATH += 'C:\Program Files (x86)\Windows Kits\8.1\Include\shared'"
Add-Content geCache.tmp2 "`nDEFINES += _CRT_SECURE_NO_WARNINGS"
Add-Content geCache.tmp2 "`nCONFIG += exceptions"
Add-Content geCache.tmp2 "`nCONFIG += windows"
Add-Content geCache.tmp2 "`nCONFIG += console"

Get-Content geCache.tmp2, geCache.tmp | Set-Content geCache.pro

rm geCache.tmp -erroraction 'silentlycontinue'
rm geCache.tmp2 -erroraction 'silentlycontinue'


# Building the Makefile file using qmake

qmake -o Makefile

nmake

rm Makefile*
