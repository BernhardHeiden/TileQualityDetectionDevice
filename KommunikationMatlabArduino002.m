% Initialize Serial object
a = serialport('COM3',9600,'Timeout',10);
x = 0;
pause(3.0) %5 sec Pause
writeline(a,"x") % wenn arduino x erhält startet Förderband
%b = read(a,1,"char");
%disp (b)
while (x == 0)
b = read(a,1,"char");
disp (b)
if (b == 'e')
x = 1;
disp (x)
end
if (b == 's')
disp(a)
Bildverarbeitung002; %go to Bildverarbeitung002
pause(1.0)
load("variable.mat")
if (blackVb3 > 145518 && whiteVb3 > 1493108 )
disp('Correct one')
%arduino send number to arduino
%pause(3.0)
writeline(a,"1") % c = correct
else
disp('Incorrect one')
%arduino send number to arduino
%pause(3.0)
writeline(a,"0") % i = incorrect
end
disp (b)
end
pause(0.1) % wait for 100ms and then loop
end
clear device