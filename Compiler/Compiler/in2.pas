const space='0';
var a,b,mm:integer;c:char;arr:array[4] of integer;
function fib(var m:integer):integer;
var e,f,n:integer;
begin
    e:=1;f:=1;
    if m<=2 then fib:=1
    else  fib:=fib(m-1)+fib(m-2)
end
procedure sort;
var i,j,k:integer;
begin
    for i:=0 to 3 do
	for j:=i+1 to 3 do
	begin
	    if arr[i]>arr[j] then
            begin
	        k:=arr[j];
		arr[j]:=arr[i];
		arr[i]:=k
	    end
	end
end;
begin
c:=110;
b:=1;
arr[0]:=b;
arr[1]:=c+b+1;
arr[2]:=c;
arr[3]:=arr[1]+arr[0];
read(a);
if a=0 then 
      begin
          read(mm); 
	      write(fib(mm))
      end
else if a=1 then
          sort;
for mm:=0 to 3 do
	  begin
	  write(arr[mm]);
	  write(" ")
      end
end.
