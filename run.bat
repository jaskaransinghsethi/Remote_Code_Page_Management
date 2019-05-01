cd "..\OOD_PROJECT3\Debug
start Server.exe

cd "..\..\OOD_PROJECT3\WpfApp3\bin\Debug

start WpfApp3.exe localhost 8080 localhost 8082 DEMO "../../../../OOD_PROJECT3" [A](.*)
timeout 3
start WpfApp3.exe localhost 8080 localhost 8086 DEMO "../../../../OOD_PROJECT3" [G](.*)
