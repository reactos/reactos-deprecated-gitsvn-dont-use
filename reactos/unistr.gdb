define puni
	set $i = 0
	while $i < ((PUNICODE_STRING)$addr)->Length / 2
		printf "%c", ((PUNICODE_STRING)$addr)->Buffer[$i]
		set $i = $i + 1
	end
	printf "\n"
end
