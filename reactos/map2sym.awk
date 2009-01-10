#!/usr/bin/awk -f

BEGIN {
	print "	.text"
	n = 0;
}

/^[0-9a-fA-F]*[ \t]*<.*>.*/ {
	symaddr = strtonum("0x" $1) - 0x8000
	gsub(/@[0-9]*/,"")
	sym = substr($2,2,length($2)-3)
	if (provide[sym] == "")
	{
		provide[sym] = symaddr
		syms[n++] = sym
	}
}

END {
	printf("/* length %d */\n", length(syms));
	x = length(syms);
	for (i = 0; i < x; i++)
	{
		syi = syms[i];
		sai = provide[syms[syi]];
		for (j = i+1; j < x; j++)
		{
			syj = syms[j];
			saj = provide[syms[syj]];
			if (saj < sai)
			{
				syms[i] = syj;
				syms[j] = syi;
			}
		}
	}
	printf("/* sorted! */\n");
	lastaddr = 0;
	for (i = 0; i < x; i++)
	{
		sym = syms[i];
		symaddr = provide[sym];
		if (sym != "" && symaddr > lastaddr)
		{
			printf("\t.org 0x%x\n", symaddr);
			printf("\t.globl %s\n", sym);
			printf("%s:\n", sym);
			printf("\t.byte 0\n");
		}
		lastaddr = symaddr;
	}
	printf("/* done */\n");
}
