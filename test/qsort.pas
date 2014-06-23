program test;
var
  a:array[0..10000] of integer;
  i,n:integer;
procedure qsort(l,r:integer);
var
  p,t,x,y:integer;
begin
  if l<r then
  begin
  x:=l;
  y:=r;
  t:=(l+r)/2;
  p:=a[t];
  a[t]:=a[x];
  a[x]:=p;
  while x<y do
    begin
      while (x<y)and(a[y]>=p) do
        y := y - 1;
      if (x<y)and(a[y]<p) then
        begin
          a[x]:=a[y];
          x := x + 1;
        end;
      while (x<y)and(a[x]<=p) do
        x := x + 1;
      if (x<y)and(a[x]>p) then
        begin
          a[y]:=a[x];
          y := y - 1;
        end;
    end;
  a[x]:=p;
  qsort(l,x-1);
  qsort(x+1,r);
  end;
end;
begin
  read(n);
  for i:=1 to n do
    read(a[i]);
  qsort(1,n);
  for i:=1 to n-1 do
    write(a[i],' ');
  writeln(a[n]);
end.
