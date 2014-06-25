program test_case2;
	var
		a: integer;

	function prime(x:integer):boolean;
		var
			k, sum : integer;
		begin
			if x = 1 then
				prime :=false
			else
				prime :=true;
			for k:=2 to x div 2 do
			begin
		    		if (x mod k) = 0 then 
				begin
					prime := false;
				end;
			end;
		end;
begin
		writeln('please input a(>0)');
		repeat 
				readln(a);
				write('you input a =');
				writeln(a);
		until (a > 0);
		if prime(a) then
			writeln('a is prime')
		else
			writeln('a is not prime');
end.

