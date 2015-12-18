const space='a';

var n,ntemp,meanless,i:integer;
    a:array[6] of integer;
    ch:char;
procedure input;
begin
   n:=2*ntemp+2*ntemp;
   a[0]:=space;
   for n:=1 to 6
   do
      begin
      read(ntemp);
      a[n]:=ntemp;
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
   begin
   i:=i+1;
   if i>h then
	   t:=m
          else
           t:=a[i]
   end
 while a[i]<m;

 j:=j+1;
 do
   begin
   j:=j-1;
   if j>=l then
           t:=a[j]
   else
	   t:=m
   end	   
 while m<t;

 if i<=j then
  begin
   t:=a[i]; a[i]:=a[j]; a[j]:=t;
   i:=i+1; j:=j-1
  end
  end
  while i<=j;
 if i<h then t:=qsort(i,h);
 if j>l then t:=qsort(l,j);
 qsort:=1;
end;
begin
    input;
    meanless:=qsort(0,6)*5;
    for i:=0 to 6
    do
           write(" ",a[i]);
   read(i)
end.
