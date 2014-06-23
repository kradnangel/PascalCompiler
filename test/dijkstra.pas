program haha;
var
  i,j,k,m,n,p,h,w:integer;
  a:array[0..1001,0..1001] of integer;
  f:array[0..1001] of integer;
  vis:array[0..1001] of boolean;
begin
  read(n,m);
  for i:=1 to n do
    for j:=1 to n do
      a[i,j]:=32767;
  for i:=1 to m do
    begin
      read(j,h,w);
      a[j,h]:=w;
      a[h,j]:=w;
    end;
  for i:=0 to n do
    begin
      f[i]:=32767;
      vis[i]:=true;
    end;
  f[1]:=0;
  for i:=1 to n do
    begin
      k:=0;
      for j:=1 to n do
        if (f[j]<f[k])and vis[j] then
          k:=j;
      if k=0 then
        break;
      for j:=1 to n do
        if a[k,j]+f[k]<f[j] then
          f[j]:=a[k,j]+f[k];
      vis[k]:=false;
    end;
  read(k);
  for i:=1 to k do
    begin
      read(w);
      writeln(f[w]);
    end;
end.
