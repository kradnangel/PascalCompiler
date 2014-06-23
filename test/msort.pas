program test;
var
  i,j,k,m,n,p,h:integer;
  a,ax,ay:array[0..10001] of integer;
procedure msort(xx,yy:integer);
var
  x,y,mm,ii:integer;
begin
  mm:=(xx+yy-1) div 2;
  if xx<mm then
    msort(xx,mm);
  if mm+1<yy then
    msort(mm+1,yy);
  x:=mm+1-xx;
  y:=yy-mm;
  for ii:=1 to x do
    ax[ii]:=a[ii+xx-1];
  for ii:=1 to y do
    ay[ii]:=a[mm+ii];
  ax[x+1]:=32000;
  ay[y+1]:=32000;
  x:=1;  y:=1;
  for ii:=xx to yy do
    if ax[x]<=ay[y] then
      begin
        a[ii]:=ax[x];
        x:=x+1;
      end
    else
      begin
        a[ii]:=ay[y];
        y:=y+1;
      end;
end;
begin
  read(n);
  for i:=1 to n do
    read(a[i]);
  msort(1,n);
  for i:=1 to n-1 do
    write(a[i],' ');
  writeln(a[n]);
end.
