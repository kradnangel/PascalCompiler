var
  i,j,k,m,n:integer;
  a:array[0..10001] of integer;
  f:array[0..10001] of integer;
procedure up(x:integer);
var
  t,q:integer;
begin
  f[0]:=f[0]+1;
  f[f[0]]:=x;
  x:=f[0];
  t:=x div 2;
  while (t>=1)and(f[x]<f[t]) do
    begin
      q:=f[x];
      f[x]:=f[t];
      f[t]:=q;
      x:=t;  t:=x div 2;
    end;
end;
function down:integer;
var
  t,q,x:integer;
begin
  down:=f[1];
  f[1]:=f[f[0]];
  f[f[0]]:=maxlongint div 2;
  f[0]:=f[0]-1;
  x:=1;
  t:=1;
  if x+x<=f[0] then
    begin
      t:=x+x;
      if f[x+x+1]<f[x+x] then
        t:=t+1;
    end;
  while f[x]>f[t] do
    begin
      q:=f[x];
      f[x]:=f[t];
      f[t]:=q;
      x:=t;
      if x+x<=f[0] then
        begin
          t:=x+x;
          if f[x+x+1]<f[x+x] then
            t:=t+1;
        end;
    end;
end;
begin
  read(n);
  for i:=1 to n do
    read(a[i]);
  for i:=1 to n+1 do
    f[i]:=32000;
  f[1]:=a[1];
  f[0]:=1;
  for i:=2 to n do
    up(a[i]);
  for i:=1 to n-1 do
    write(down);
  writeln(down);
end.
