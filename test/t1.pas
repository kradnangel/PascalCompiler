{
	test for
	1. Basic test;
	2. the usability of integer;
	3. the usability of array;
	4. boundary of array (1-20)
	5. the usability of read
	6. the usability of writeln
	7. the usability of for
	8. the usability of if-else
}
program test_case1;
var
	fib:array[1..20] of integer;
	i : integer;
begin
	fib[1] := 1;
	fib[2] := 1;
	for i:= 3 to 20 do
		fib[i] := fib[i-1] + fib[i-2];
	writeln('Please input 1-20');	
	read(i);
	if (i <= 20) and (i >=0) then
		writeln(fib[i])
	else
		writeln('Out of boundary!');
end.
