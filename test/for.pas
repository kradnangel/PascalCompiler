program helloworld;
var
		a, b, c, d : integer;
begin
		b := 10 + 20;
		c := 20;
		d := 0;
		for a := b downto c do
			d := d + a;
		writeln('d = ', d);
end.
