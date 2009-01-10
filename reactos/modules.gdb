define modname
  set $i = 0
  while $i < ((PLDR_DATA_TABLE_ENTRY)$addr)->BaseDllName.Length / 2
    printf "%c", ((PLDR_DATA_TABLE_ENTRY)$addr)->BaseDllName.Buffer[$i]
    set $i = $i + 1
  end
end

define modaddr
  printf "%x", ((PLDR_DATA_TABLE_ENTRY)$addr)->DllBase + 0x1000
end

define modlist
  set $modlisthead = &PsLoadedModuleList
  set $flink = PsLoadedModuleList.Flink
  while $flink != $modlisthead
    set $addr = $flink
    modaddr $flink
    printf "\t"
    modname $flink
    printf "\n"
    set $flink = ((PLIST_ENTRY)$flink)->Flink
  end
end
