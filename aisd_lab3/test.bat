set PROGRAM="%~1"

%PROGRAM% input\1_info_corps.txt input\1_info_faculty.txt > output\out1.txt
if ERRORLEVEL 1 goto err

echo Program testing succeeded
exit 0

:err
echo Program testing failed
exit 1