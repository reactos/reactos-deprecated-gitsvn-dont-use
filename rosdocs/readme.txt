==================
= ROSDOCS PRIMER =
================== [by your beloved librarian KJK::Hyperion]

CONCEPTS
========
    RosDocs, in its current incarnation, is a fairly complex documentation
system, but that can still be managed and authored with simple and promptly
available tools, such as text editors and file managers (yet allowing for more
sophisticated tools to be developed in the future)

    It's not vital to grasp all the concepts behind RosDocs to start authoring
documentation for the ReactOS project, since one of the tenets of RosDocs is the
strict separation between content, presentation and storage. Should any of the
three be found flawed, the other two wouldn't need to be throwed away as well

    Let's spend a few more words about content, presentation and storage:

CONTENT
-------
    There's two ways to add content to RosDocs:

 - Doxygen comments. If you're going to write reference pages (think Unix man
   pages), this is the format you should get accostumed to. It consists of
   special comments, containing markup that a tool called Doxygen can extract
   into a variety of formats. If you already know JavaDoc, or the QT comment-
   based documentation system, Doxygen supports those as well

   There's currently no strict guidelines, since RosDocs is still being planned,
   so you have a great deal of freedom. Don't abuse it, though. Avoid any
   structural markup, except paragraphs, lists, etc. And, at the moment, don't
   worry about storage as discussed in another section of this document

 - DocBook XML documents. Doxygen can be abused to write manuals and books, but
   a better long-term solution is learning the DocBook XML DTD. You can also use
   DocBook for reference pages, but Doxygen is preferred for that. Also note
   that, at the moment, reference pages are more important than guides

   No guidelines yet for DocBook, either. In general, don't write books yet.
   Limit yourself to articles. A good starting point is writing short tutorials
   for the Knowledge Base, since no earth-shaking changes are expected in that
   field

STORAGE
-------
    This is where things get tough. Remember, though, that this specification is
draft at best. Future directions include:

 - support for DocBook books
 - filling the text of hyperlinks with the title of the target topic
 - SELECT queries, resolved at compile-time, to build tables of links

    All things that, with the very strict decoupling of content and storage
outlined here, would be impossible, or involve run-time processing. For this
reason, limit yourself to reference pages and Knowledge Base articles. Guides,
overviews and examples can wait

    That said, here are the storage concepts of RosDocs:

DOMAINS
    If you are familiar with other schemes featuring domains, such as Internet
host names, or the Java third-party classes, this is nearly the same. Otherwise,
read on

    Since some package names are awfully common (like "win32"), and since third-
party contributions are encouraged, it's necessary to compartment package names
on a vendor, product and, for complex products, feature basis. Such namespaces
are encoded as follows:

[ [ <vendor-specific subdivision>. ]<scope>. ]<vendor>

    The scope can be a product or book name, as in the following examples:

 - ReactOS Platform SDK: psdk.reactos
 - GCC manual: gcc.fsf
 - Bruce Eckel's Thinking in C++: ticpp.eckel

    Further, optional subdivisions are possible:

 - GCC internals manual: guts.gcc.fsf

    Note, however, that, at the moment, only the following domains are accepted:

 - psdk.reactos (ReactOS Platform SDK)
 - ssdk.reactos (ReactOS Subsystem Development Kit)
 - ddk.reactos (ReactOS Driver Development Kit)
 - kb.reactos (ReactOS Knowledge Base)

PACKAGES
    Packages are collections of topics and indexes. They are the base unit of
storage. A package may additionally contain one or more of the following items:

 - secondary indexes
 - a table of contents
 - configuration metadata

    Packages are an interface that exposes topics and indexes, they don't 
dictate a specific implementation, neither in their "source" nor in their
"compiled" form. Possible implementations of the compiled form ("engines")
include:

 - database on a remote server
 - filesystem directory
 - compressed archive

    There's no well-defined standard for the source form yet, but it's expected
to be a derivative of DocBook XML. Third parties can obviously choose other
formats than the future standard for the source form, but official ReactOS
documentation will have to be written in the standard

    For your documentation, you're free to organize your topics in as many
packages as you like. For ReactOS books and manuals, the following packages are
defined:

 - psdk.reactos domain:

    - win32. The Win32 subsystem's structure and boot sequence; and the RPC APIs
      to the Windows and Console servers

    - winbase. All the basic Win32 calls go here. These include file, device and
      console I/O and control; registry, memory, handles, thread, process and
      service management; DLL loading; and basic error handling

    - winui. All the basic Win32 user interface services go here. These include
      windows, MDI windows, window classes, resources, hooks, DDE, keyboard and
      mouse input, and standard controls

    - wingdi. All the Win32 GDI and printer spooler interfaces go here

    - winipc. Win32 interfaces to synchronization objects, shared memory, named
      and anonymous pipes, and mailslots

    - winsec. Win32 interfaces to access control; standard access rights for
      Win32 object types; GINA API and implementation; Network Providers API
      and implementation; general security considerations and guidelines

    - windbg. Win32 debugger API; Win32 SEH support

    - psapi. Process Status Helper API

    - tlhlp. Tool Helper API

    - commdlg. Common Dialog Box Library

    - commctrl. Common Controls Library

 - ssdk.reactos domain:

    - nt. Windows NT architecture; system structure and boot sequence; RPC API
      to the Base server; the Process Environment Block; the Thread Environment
      Block; the Kernel/User Shared Data

    - ntobj. Windows NT objects and handles; overview of predefined object
      types; Object Manager basics; the system objects namespace

    - ntsec. The Windows NT security model; explanation of token objects; SIDs,
      ACEs and ACLs; generic access rights; standard access rights for kernel
      object types

    - ntseh. Structured Exception Handling internals

    - ntlpc. The Local Procedure Call protocol

    - ntdbg. Debugging interfaces, both kernel and user mode; the debugger LPC
      protocol

    - ntcalls. System calls (Nt* and Zw*), both kernel and user mode

    - ntrtl. Runtime library interfaces, both kernel and user mode; list of
      supported C runtime interfaces, both kernel and user mode

    - ntldr. The PE Loader API (Ldr*), both kernel and user mode

    - ntcsr. The Client-Server Runtime API (Csr*); server modules API and
      implementation

    - ntnls. National Language Support API(Nls*), both kernel and user mode

    - ntuser. Native User Interface (NtUser*) system calls, both kernel and user
      mode

    - w32k. Native GDI (W32k*) system calls, both kernel and user mode

    - peexe. Structures describing the PE executable format

 - ddk.reactos domain:

    - cc. The Cache Manager subsystem; architecture and API

    - cm. The Configuration Manager subsystem; architecture and API

    - ex. The Executive; architecture and API

    - hal. The Hardware Abstraction Layer; architecture and API

    - io. The I/O Manager subsystem; architecture and API

    - lpc. The Local Procedure Call subsystem; architecture and API

    - mm. The Virtual Memory Manager subsystem; architecture and API

    - ob. The Object Manager subsystem; architecture and API

    - ps. The Process Structure Manager subsystem; architecture and API

    - se. The Security Reference Monitor subsystem; architecture and API

TOPICS
    Topics are the base unit of documentation. They contain the actual content,
and are organized in physical units of storage called packages

    [placeholder]

INDEXES
    [placeholder]

TOCS
    [placeholder]

CATALOGS
    [placeholder]

AUTHORING
=========
    [placeholder]