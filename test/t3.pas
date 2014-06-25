program test_case3;
	type
		date = record
            		year:0..2014;
            		month:1..12;
            		day:1..31;
		end;
	var
		i,a: integer;		dt:date;
	begin
		write('Please input the year: ');	read(a);
		dt.year := a;
		write('Please input the month: ');	read(a);
		dt.month := a;
		write('Please input the data: ');	read(a);
		dt.day := a;
		write('dt.year =');
		writeln(dt.year);
		write('dt.month=');
		case dt.month of 
			1:writeln('Jar');
			2:writeln('Feb');
			3:writeln('Mar');
			4:writeln('Apr');
			5:writeln('May');
			6:writeln('Jun');
			7:writeln('Jul');
			8:writeln('Aug');
			9:writeln('Sep');
			10:writeln('Oct');
			11:writeln('Nov');
			12:writeln('Dec');
		end;
		write('dt.day  =');
		writeln(dt.day);
end.

