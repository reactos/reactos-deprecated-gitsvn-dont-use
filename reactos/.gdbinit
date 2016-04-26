cd output-MinGW-i386

set target-charset ASCII
set target-wide-charset UTF-16LE

python
import subprocess
import functools
import os.path

def decorator(fn):
    def outer(*args, **kwargs):
        def inner(_fn):
            return fn(_fn, *args, **kwargs)
        return inner
    return outer

@decorator
class Command(gdb.Command):
    def __init__(self, func, *, repeat=True):
        self.func = func
        self.name = func.__name__.replace("_", "-")
        self.__doc__ = func.__doc__

        super().__init__(self.name, gdb.COMMAND_USER)

        if not repeat:
            self.dont_repeat()

    @staticmethod
    def _try_eval(arg):
        try:
            return gdb.parse_and_eval(arg)
        except:
            return arg

    def invoke(self, arg, from_tty):
        args = gdb.string_to_argv(arg)
        self(*map(self._try_eval, args))

    def __call__(self, *args, **kwargs):
        self.func(*args, **kwargs)

class _PrettyPrinter_impl:
    def __init__(self, func, hint, val):
        self.func = func
        self.hint = hint
        self.val = val

    def to_string(self):
        return self.func(self.val)

    def display_hint(self):
        return self.hint

@decorator
def PrettyPrinter(func, typename, hint="string"):
    self = PrettyPrinter

    if not hasattr(self, "_pp"):
        self._pp = gdb.printing.RegexpCollectionPrettyPrinter("python")
        gdb.printing.register_pretty_printer(None, self._pp)

    ret = functools.partial(_PrettyPrinter_impl, func, hint)
    self._pp.add_printer(func.__name__, typename, ret)
    return func

def relocate(filename, offset):
    p = subprocess.Popen(["objdump", "-ph", filename], stdout = subprocess.PIPE)

    sections = []
    ImageBase = 0
    TextBase = 0

    #Fetch the image base
    line = ""
    while not line.startswith("ImageBase"):
        line = p.stdout.readline().decode("utf-8")

    ImageBase = int(line.split()[1], base=16)

    #Skip until the actual output
    while not p.stdout.readline().startswith(b"Idx"):
        pass

    def group(sub):
        while True:
            yield (next(sub), next(sub))

    for line in group(iter(p.stdout.readlines())):
        idx, name, size, vma, lma, file_off, algn = line[0].decode("utf-8").split()
        flags = set(map(str.strip, line[1].decode("utf-8").split(",")))

        #Factor out the base address
        address = int(vma, base=16) - ImageBase

        if name == ".text":
            TextBase = address
        elif 'ALLOC' in flags:
            sections.append("-s %s 0x%x" % (name, offset + address))

    return "0x%x %s" % (offset + TextBase, " ".join(sections))

@Command(repeat=False)
def add_symbol_file_at(filename, offset=0):
    """add-symbol-file-at filename [offset=0]"""
    offset = int(offset)

    if not os.path.isfile(filename):
        print("No such file %s" % (filename))
        return

    gdb.execute("add-symbol-file %s %s" % (filename, relocate(filename, offset)))

@PrettyPrinter("UNICODE_STRING")
def unicode_string(val):
    length = (val['Length'] / 2) - 1

    if length < 0:
        length = 0

    type = gdb.lookup_type("wchar_t").array(length)
    buffer = val['Buffer'].dereference()
    return buffer.cast(type)

@Command(repeat=False)
def load_sym_int(name, base):
    name = unicode_string(name).string()
    base = int(base)
    name = name.lower()

    # Skip ntoskrnl, we load it manually
    if name == "ntoskrnl.exe":
        return

    # assume kdgdb is the loaded kd module
    if name == "kdcom.dll":
        name = "kdgdb.dll"

    add_symbol_file_at("symbols/%s.dbg" % name, base)

end

define load-symbols
    # Clear existing symbols
    symbol-file

    # Manually load ntoskrnl
    add-symbol-file-at "symbols/ntoskrnl.exe.dbg" 0x80400000

    # During early init the loader is in KeLoaderBlock instead of Ps*
    set $LoadOrderListHead = &PsLoadedModuleList
    if $LoadOrderListHead->Flink == 0
        set $LoadOrderListHead = &KeLoaderBlock->LoadOrderListHead
    end

    # Iterate and load each module one-by-one
    set $cur = $LoadOrderListHead->Flink
    while $cur != $LoadOrderListHead
        set $ldr_entry = (PLDR_DATA_TABLE_ENTRY)$cur
        load-sym-int $ldr_entry->BaseDllName $ldr_entry->DllBase
        set $cur = $cur->Flink
    end
end
