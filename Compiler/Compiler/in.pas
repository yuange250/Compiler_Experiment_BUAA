const space='a';

var n,ntemp,meanless,i:integer;
    a:array[6] of integer;
    ch:char;
procedure input;
begin
   for n:=0 to 6
   do
      begin
      read(ntemp);
      a[n]:=ntemp
      end
end;
function qsort(l,h:integer):integer;
var
 i,j,t,m:integer;
 begin
 i:=l; j:=h;
 m:=a[(i+j) / 2];
 do
 begin
 i:=i-1;

 do
   i:=i+1
 while a[i]<m;

 j:=j+1;
 do
   j:=j-1
 while m<a[j];

 if i<=j then
  begin
   t:=a[i]; a[i]:=a[j]; a[j]:=t;
   i:=i+1; j:=j-1
  end
  end
  while i<=j;
 if i<h then t:=qsort(i,h);
 if j>l then t:=qsort(l,j);
 qsort:=1
end;
begin
    input;
    meanless:=qsort(0,5)*5;
    for i:=1 to 10
    do
           write(a[i]);
   read(i)
end.
